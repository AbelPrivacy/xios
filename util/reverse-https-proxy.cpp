// Build with g++:
//

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/settings.h>

#include <exception>

char* proxy_server_addr;
int proxy_server_port;
const char* backend_host;
int backend_port;

#define BUFFER_SIZE 1024 * 100

void err_sys(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int connect_backend() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) err_sys("Backend socket failed");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(backend_port);
    inet_pton(AF_INET, backend_host, &addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        err_sys("Backend connect failed");

    return sockfd;
}


int main(int argc, char** argv) {
    if (argc == 5) {
        proxy_server_addr = argv[3];
        proxy_server_port = atoi(argv[4]);
        backend_host = argv[1];
        backend_port = atoi(argv[2]);
    } else if (argc != 1) {
        return argc;
    } else {
        proxy_server_addr = "127.0.0.1";
        proxy_server_port = 443;
        backend_host = "127.0.0.1";
        backend_port = 8080;
    }

    int sockfd, clientfd;
    struct sockaddr_in addr, client;
    socklen_t client_len = sizeof(client);

    wolfSSL_Init();

    // TLS server context with any available cipher
    WOLFSSL_CTX* ctx = wolfSSL_CTX_new(wolfTLSv1_3_server_method());
    if (!ctx) err_sys("wolfSSL_CTX_new failed");

    if (wolfSSL_CTX_use_certificate_file(ctx, "util/certs/cert.pem",
                                         WOLFSSL_FILETYPE_PEM) !=
        WOLFSSL_SUCCESS)
        err_sys("Failed to load cert");

    if (wolfSSL_CTX_use_PrivateKey_file(
            ctx, "util/certs/key.pem", WOLFSSL_FILETYPE_PEM) != WOLFSSL_SUCCESS)
        err_sys("Failed to load key");

    // Use all supported ciphers (including hybrid/OQS)
    if (wolfSSL_CTX_set_cipher_list(ctx, "ALL") != WOLFSSL_SUCCESS)
        err_sys("Failed to set cipher list");


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) err_sys("Socket creation failed");

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(proxy_server_port);
    inet_pton(AF_INET, proxy_server_addr, &addr.sin_addr);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        err_sys("Bind failed");

    if (listen(sockfd, 5) < 0) err_sys("Listen failed");

    printf("Proxy listening on port %d\n", proxy_server_port);

    while (1) {
        try {
            clientfd = accept(sockfd, (struct sockaddr*)&client, &client_len);
            if (clientfd < 0)
                err_sys("Accept failed");
            else
                printf("client accepted\n");

            WOLFSSL* ssl = wolfSSL_new(ctx);
            wolfSSL_set_fd(ssl, clientfd);

            if (wolfSSL_accept(ssl) != WOLFSSL_SUCCESS) {
                fprintf(stderr, "TLS accept failed\n");
                wolfSSL_free(ssl);
                close(clientfd);
                continue;
            }

            // Forward request
            char buffer[BUFFER_SIZE];
            int n = wolfSSL_read(ssl, buffer, BUFFER_SIZE);
            if (n <= 0) {
                wolfSSL_free(ssl);
                close(clientfd);
                continue;
            }

            int backendfd = connect_backend();
            write(backendfd, buffer, n);

            // Read backend response
            int total_written, bytes_written, bytes_read;
            while ((bytes_read = read(backendfd, buffer, BUFFER_SIZE)) > 0) {
                total_written = 0;
                while (total_written < bytes_read) {
                    bytes_written = wolfSSL_write(ssl, buffer + total_written,
                                                  bytes_read - total_written);
                    if (bytes_written <= 0) {
                        fprintf(stderr, "wolfSSL_write failed: %d\n",
                                wolfSSL_get_error(ssl, bytes_written));
                        break;
                    }
                    total_written += bytes_written;
                }
            }
            close(backendfd);

            wolfSSL_shutdown(ssl);
            wolfSSL_free(ssl);
            close(clientfd);
        } catch (std::exception& e) {
            fprintf(stderr, "Exception on proxy server while listening: %s",
                    e.what());
        }
    }

    wolfSSL_CTX_free(ctx);
    wolfSSL_Cleanup();
    return 0;
}
