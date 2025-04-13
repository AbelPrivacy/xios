#ifndef SECURE_HTTP_CLIENT_HPP
#define SECURE_HTTP_CLIENT_HPP

/*
g++ -std=c++17 -lwolfssl -lsqlite3 -o secure_http_client -c xios.cpp -I ~/homebrew/Cellar/wolfssl/5.7.6/include -L ~/homebrew/Cellar/wolfssl/5.7.6/lib/ -L ~/homebrew/Cellar/sqlite/3.49.1/lib -I ~/homebrew/Cellar/sqlite/3.49.1/include -o xios.o
*/

#include <string>
#include <vector>
#include <map>
#include <utility>


typedef struct WOLFSSL_CTX WOLFSSL_CTX;
typedef struct WOLFSSL WOLFSSL;

struct ParsedURL {
    std::string scheme;
    std::string host;
    uint16_t port;
    std::string path;
};

class SecureHttpClient {
public:
    static void initialize(std::string& sqlitePath,
                        std::vector<std::string>& allowedProtocols,
                        std::vector<std::string>& allowedImplementations,
                        std::vector<std::string>& blockedImplementations,
                        std::vector<std::string>& allowedGroups);
    static std::tuple<WOLFSSL*,
                      WOLFSSL_CTX*,
                      int,
                      ParsedURL,
                      const char*,
                      const char*,
                      const char*>
        prepare_for_request(std::string url);
    static std::string make_request(std::string url, std::string req);
    static std::string get(std::string url);
    static std::string post(std::string url, const std::string& payload);

    static std::string parseHost(std::string url);
    static std::string parsePath(std::string url);
    static ParsedURL parseURL(std::string url);

private:
    static void initWolfSSL();
    static void initSQLite(const std::string& path);
    static void applyProtocolRestrictions(WOLFSSL_CTX* ctx);
    static void logConnectionMetadata(const std::string& host,
                                       const std::string& protocol,
                                       const std::string& tlsImpl,
                                       const std::string& keyExchange);

    static bool isProtocolAllowed(const std::string& protocol);
    static bool isImplementationAllowed(const std::string& impl);
     
    static int16_t mapGroupNameToId(const std::string& group);

    static std::vector<std::string> s_allowedProtocols;
    static std::vector<std::string> s_allowedGroups;
    static std::vector<std::string> s_allowedImplementations;
    static std::vector<std::string> s_blockedImplementations;
    static std::string s_sqlitePath;
};

#endif