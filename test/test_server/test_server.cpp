#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/error-ssl.h>
#include <thread>
#include <atomic>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <cassert>

std::atomic<bool> serverReady(false);
std::atomic<bool> serverShutdown(false);

void runTestTLSServer(int port = 443, const std::string& certFile = "/Users/kylemcgrath/Documents/GitHub/wolfXios/test/test_server/cert.pem", const std::string& keyFile = "/Users/kylemcgrath/Documents/GitHub/wolfXios/test/test_server/key.pem") {
    //std::thread([=]() {
        wolfSSL_Init();

        WOLFSSL_CTX* ctx = wolfSSL_CTX_new(wolfTLSv1_3_server_method());
        assert(ctx && "Failed to create server CTX");

        assert(wolfSSL_CTX_use_certificate_file(ctx, certFile.c_str(), SSL_FILETYPE_PEM) == SSL_SUCCESS);
        assert(wolfSSL_CTX_use_PrivateKey_file(ctx, keyFile.c_str(), SSL_FILETYPE_PEM) == SSL_SUCCESS);

        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        assert(sockfd >= 0 && "Failed to create socket");

        struct sockaddr_in serv_addr {};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        int optval = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        assert(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == 0);
        assert(listen(sockfd, SOMAXCONN) == 0);

        serverReady = true;

        while (!serverShutdown.load()) {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);

            struct timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            int activity = select(sockfd + 1, &readfds, nullptr, nullptr, &timeout);
            if (activity <= 0) continue;

            struct sockaddr_in client_addr {};
            socklen_t len = sizeof(client_addr);
            int clientSock = accept(sockfd, (struct sockaddr*)&client_addr, &len);
            if (clientSock < 0) continue;

            WOLFSSL* ssl = wolfSSL_new(ctx);
            wolfSSL_set_fd(ssl, clientSock);

            if (wolfSSL_accept(ssl) == SSL_SUCCESS) {
                char buffer[4096] = {0};
                wolfSSL_read(ssl, buffer, sizeof(buffer));  // Read HTTP request

                const char* httpResponse =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Length: 13\r\n"
                    "Content-Type: text/plain\r\n"
                    "\r\n"
                    "Hello, world!";

                wolfSSL_write(ssl, httpResponse, strlen(httpResponse));
            }

            wolfSSL_free(ssl);
            close(clientSock);
        }

        close(sockfd);
        wolfSSL_CTX_free(ctx);
        wolfSSL_Cleanup();
    //}).detach();
}

int main(int argc, char **argv) {
    runTestTLSServer(11111);
}