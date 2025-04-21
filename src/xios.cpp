#include "xios.hpp"

#include <netdb.h>
#include <sqlite3.h>
#include <unistd.h>
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/settings.h>

#include <cstring>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>

std::string SecureHttpClient::s_sqlitePath;
std::vector<std::string> SecureHttpClient::s_allowedProtocols;
std::vector<std::string> SecureHttpClient::s_allowedGroups;
std::vector<std::string> SecureHttpClient::s_allowedImplementations;
std::vector<std::string> SecureHttpClient::s_blockedImplementations;

void SecureHttpClient::initialize(
    std::string& sqlitePath, std::vector<std::string>& allowedProtocols,
    std::vector<std::string>& allowedImplementations,
    std::vector<std::string>& blockedImplementations,
    std::vector<std::string>& allowedGroups) {
    s_allowedProtocols = allowedProtocols;
    s_allowedImplementations = allowedImplementations;
    s_blockedImplementations = blockedImplementations;
    s_allowedGroups = allowedGroups;
    s_sqlitePath = sqlitePath;

    initWolfSSL();
    initSQLite(sqlitePath);
}

void SecureHttpClient::initWolfSSL() { wolfSSL_Init(); }

void SecureHttpClient::initSQLite(const std::string& path) {
    sqlite3* db;
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open SQLite DB");
    }

    const char* createTableSQL =
        "CREATE TABLE IF NOT EXISTS ConnectionMetadata ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "host TEXT, protocol TEXT, implementation TEXT, key_exchange TEXT, "
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) !=
        SQLITE_OK) {
        std::string err = errMsg;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        throw std::runtime_error("Failed to create table: " + err);
    }

    sqlite3_close(db);
}

void SecureHttpClient::applyProtocolRestrictions(WOLFSSL_CTX* ctx) {
    long options = 0;

    // Disable all by default, selectively enable from allowlist
    options |= WOLFSSL_OP_NO_TLSv1;
    options |= WOLFSSL_OP_NO_TLSv1_1;
    options |= WOLFSSL_OP_NO_TLSv1_2;
    options |= WOLFSSL_OP_NO_TLSv1_3;

    auto lambda = [&options](const char* proto) {
        if (std::strcmp(proto, "TLSv1.2") == 0) {
            options &= ~WOLFSSL_OP_NO_TLSv1_2;
        } else if (std::strcmp(proto, "TLSv1.3") == 0) {
            options &= ~WOLFSSL_OP_NO_TLSv1_3;
        } else if (std::strcmp(proto, "TLSv1.1") == 0) {
            options &= ~WOLFSSL_OP_NO_TLSv1_1;
        } else if (std::strcmp(proto, "TLSv1") == 0) {
            options &= ~WOLFSSL_OP_NO_TLSv1;
        }
    };

    for (const std::string& proto : SecureHttpClient::s_allowedProtocols) {
        lambda(proto.c_str());
    }

    // Apply cipher suite allowlist/blocklist
    if (!s_allowedImplementations.empty()) {
        std::stringstream cipherList;
        for (const std::string& cipher : s_allowedImplementations) {
            if (std::find(s_blockedImplementations.begin(),
                          s_blockedImplementations.end(),
                          cipher) == s_blockedImplementations.end()) {
                cipherList << cipher << ":";
            }
        }

        std::string finalCipherList = cipherList.str();
        if (!finalCipherList.empty() && finalCipherList.back() == ':') {
            finalCipherList.pop_back();  // remove trailing ':'
        }

        if (wolfSSL_CTX_set_cipher_list(ctx, finalCipherList.c_str()) !=
            WOLFSSL_SUCCESS) {
            wolfSSL_CTX_free(ctx);
            throw std::runtime_error("Failed to set cipher list");
        }
    }

    // for (const auto& group : s_allowedGroups) {
    //     word16 groupId = mapGroupNameToId(group);
    //     if (wolfSSL_CTX_UseSupportedCurve(ctx, groupId) != WOLFSSL_SUCCESS) {
    //         wolfSSL_CTX_free(ctx);
    //         throw std::runtime_error("Failed to apply supported group: " +
    //         group);
    //     }
    // }

    wolfSSL_CTX_set_options(ctx, options);
}

void SecureHttpClient::logConnectionMetadata(const std::string& host,
                                             const std::string& protocol,
                                             const std::string& tlsImpl,
                                             const std::string& keyExchange) {
    sqlite3* db;
    sqlite3_open(s_sqlitePath.c_str(), &db);

    std::stringstream ss;
    ss << "INSERT INTO ConnectionMetadata (host, protocol, implementation, "
          "key_exchange) VALUES ("
       << "'" << host << "', "
       << "'" << protocol << "', "
       << "'" << tlsImpl << "', "
       << "'" << keyExchange << "');";

    char* errMsg = nullptr;
    sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, &errMsg);

    if (errMsg) sqlite3_free(errMsg);
    sqlite3_close(db);
}

bool SecureHttpClient::isProtocolAllowed(const std::string& protocol) {
    return std::find(s_allowedProtocols.begin(), s_allowedProtocols.end(),
                     protocol) != s_allowedProtocols.end();
}

bool SecureHttpClient::isImplementationAllowed(const std::string& impl) {
    if (std::find(s_blockedImplementations.begin(),
                  s_blockedImplementations.end(),
                  impl) != s_blockedImplementations.end())
        return false;
    return s_allowedImplementations.empty() ||
           std::find(s_allowedImplementations.begin(),
                     s_allowedImplementations.end(),
                     impl) != s_allowedImplementations.end();
}

std::string SecureHttpClient::parseHost(std::string url) {
    size_t start = url.find("://") + 3;
    size_t end = url.find("/", start);
    return url.substr(start, end - start);
}

std::string SecureHttpClient::parsePath(std::string url) {
    size_t pathStart = url.find('/', url.find("://") + 3);
    return pathStart != std::string::npos ? url.substr(pathStart) : "/";
}

ParsedURL SecureHttpClient::parseURL(std::string url) {
    std::regex urlRegex(
        R"(^(?:(https?)://)?([^:/]+|\d{1,3}(?:\.\d{1,3}){3})(?::(\d+))?(\/.*)?$)");
    std::smatch match;

    if (!std::regex_match(url, match, urlRegex)) {
        throw std::runtime_error("Invalid URL format: " + url);
    }

    ParsedURL result;
    result.scheme = match[1].matched ? match[1].str() : "https";
    result.host = match[2].str();
    result.port = match[3].matched
                      ? static_cast<uint16_t>(std::stoi(match[3].str()))
                      : (result.scheme == "http" ? 80 : 443);
    result.path = match[4].matched ? match[4].str() : "/";

    return result;
}

std::tuple<WOLFSSL*, WOLFSSL_CTX*, int, ParsedURL, const char*, const char*,
           const char*>
SecureHttpClient::prepare_for_request(std::string url) {
    auto parsedURL = SecureHttpClient::parseURL(url);

    // Use highest allowed protocol
    WOLFSSL_METHOD* method = nullptr;
    if (std::find(SecureHttpClient::s_allowedProtocols.begin(),
                  SecureHttpClient::s_allowedProtocols.end(),
                  "TLSv1.3") != SecureHttpClient::s_allowedProtocols.end()) {
        method = const_cast<WOLFSSL_METHOD*>(wolfTLSv1_3_client_method());
    } else if (std::find(SecureHttpClient::s_allowedProtocols.begin(),
                         SecureHttpClient::s_allowedProtocols.end(),
                         "TLSv1.2") !=
               SecureHttpClient::s_allowedProtocols.end()) {
        method = const_cast<WOLFSSL_METHOD*>(wolfTLSv1_2_client_method());
    } else {
        throw std::runtime_error("No supported protocol in allowlist");
    }

    WOLFSSL_CTX* ctx = wolfSSL_CTX_new(method);
    if (!ctx) throw std::runtime_error("Failed to create WOLFSSL_CTX");

    SecureHttpClient::applyProtocolRestrictions(ctx);

    WOLFSSL* ssl;
    int sockfd;
    struct hostent* server = gethostbyname(parsedURL.host.c_str());
    struct sockaddr_in serv_addr {};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(parsedURL.port);
    std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    ssl = wolfSSL_new(ctx);
    wolfSSL_set_fd(ssl, sockfd);

    if (wolfSSL_connect(ssl) != WOLFSSL_SUCCESS) {
        wolfSSL_free(ssl);
        wolfSSL_CTX_free(ctx);
        close(sockfd);
        throw std::runtime_error("TLS handshake failed");
    }

    const char* cipher = wolfSSL_get_cipher_name(ssl);
    const char* protocol = wolfSSL_get_version(ssl);
    const char* keGroup =
        wolfSSL_get_curve_name(ssl);  // placeholder for KEM/DHE/EC info

    if (!SecureHttpClient::isProtocolAllowed(protocol) ||
        !SecureHttpClient::isImplementationAllowed(cipher)) {
        wolfSSL_free(ssl);
        wolfSSL_CTX_free(ctx);
        close(sockfd);
        throw std::runtime_error("Protocol or Implementation not allowed");
    }
    return std::make_tuple(ssl, ctx, sockfd, parsedURL, protocol, cipher,
                           keGroup);
}

std::string SecureHttpClient::make_request(std::string url, std::string req) {
    std::tuple<WOLFSSL*, WOLFSSL_CTX*, int, ParsedURL, const char*, const char*,
               const char*>
        request_prep = prepare_for_request(url);

    WOLFSSL* ssl = std::get<0>(request_prep);
    WOLFSSL_CTX* ctx = std::get<1>(request_prep);
    int sockfd = std::get<2>(request_prep);
    ParsedURL parsedURL = std::get<3>(request_prep);
    const char* protocol = std::get<4>(request_prep);
    const char* cipher = std::get<5>(request_prep);
    const char* keGroup = std::get<6>(request_prep);

    wolfSSL_write(ssl, req.c_str(), req.length());

    char buffer[4096];
    std::stringstream response;
    int bytes;
    while ((bytes = wolfSSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = 0;
        response << buffer;
    }

    logConnectionMetadata(parsedURL.host, protocol, cipher,
                          keGroup ? keGroup : "N/A");

    wolfSSL_free(ssl);
    wolfSSL_CTX_free(ctx);
    close(sockfd);

    return response.str();
}

std::string SecureHttpClient::get(std::string url) {
    auto parsedURL = SecureHttpClient::parseURL(url);

    std::stringstream req;
    req << "GET " << parsedURL.path << " HTTP/1.1\r\n"
        << "Host: " << parsedURL.host << "\r\n"
        << "Connection: close\r\n\r\n";

    return make_request(url, req.str());
}

std::string SecureHttpClient::post(std::string url,
                                   const std::string& payload) {
    // Similar to GET, adjust request to POST with headers and body
    throw std::runtime_error("POST not implemented yet in this snippet");
}

// Map human-readable PQC group names to wolfSSL internal IDs
int16_t SecureHttpClient::mapGroupNameToId(const std::string& group) {
    // if (group == "KYBER_LEVEL1") return WOLFSSL_KYBER_LEVEL1;
    // if (group == "KYBER_LEVEL3") return WOLFSSL_KYBER_LEVEL3;
    // if (group == "KYBER_LEVEL5") return WOLFSSL_KYBER_LEVEL5;
    // if (group == "NTRU_PRIME_LEVEL1") return WOLFSSL_NTRU_PRIME_LEVEL1;
    // if (group == "NTRU_PRIME_LEVEL3") return WOLFSSL_NTRU_PRIME_LEVEL3;
    // if (group == "NTRU_PRIME_LEVEL5") return WOLFSSL_NTRU_PRIME_LEVEL5;
    throw std::runtime_error("Unsupported group name: " + group);
}
