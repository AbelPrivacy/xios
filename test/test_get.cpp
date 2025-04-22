#include "../src/xios.hpp"
#include "./catch2/include/catch_amalgamated.hpp"

using namespace std;

TEST_CASE("SecureHttpClient::get performs HTTP GET correctly", "[get]") {
    std::string db = "./metadata.db";
    std::vector<std::string> proto;
    proto.push_back("TLSv1.3");
    std::vector<std::string> cipher;
    cipher.push_back("TLS13-AES256-GCM-SHA384"); //"TLS_AES_256_GCM_SHA384");
    std::vector<std::string> empty1;
    std::vector<std::string> empty2;
    // SECTION("GET request to HTTPS server returns expected response") {
        SecureHttpClient::initialize(db, proto, cipher, empty1,
                                     empty2  // or ['KYBER_LEVEL1'] if supported
        );
        std::string response = SecureHttpClient::get("localhost:1443/request/");
        REQUIRE(response.find("HTTP/1.1 200 OK") == 0);
        // REQUIRE(response.find("Server: Werkzeug/3.1.3 Python/3.13.3") != -1);
        REQUIRE(response.find("Content-Type: text/html; charset=utf-8") != -1);
        REQUIRE(response.find("Connection: close") != -1);
        REQUIRE(response.find("GET /requests/") != -1);
    // }
}