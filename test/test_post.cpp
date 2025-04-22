#include "../src/xios.hpp"
#include "./catch2/include/catch_amalgamated.hpp"

TEST_CASE("SecureHttpClient::post performs HTTP POST correctly", "[post]") {
    std::string db = "./metadata.db";
    std::vector<std::string> proto = {"TLSv1.3"};
    std::vector<std::string> empty0, empty1, empty2;

    SecureHttpClient::initialize(db, proto, empty0, empty1, empty2);

    SECTION("POST request to HTTPS server returns expected response") {
        std::string payload = R"({"key": "value"})";
        std::string url = "https://localhost:1443/submit";

        std::string response = SecureHttpClient::post(url, payload);

        REQUIRE(response.find("HTTP/1.1 200 OK") == 0);
        REQUIRE(response.find("Content-Type: application/json") !=
                std::string::npos);
        REQUIRE(response.find(R"("key": "value")") !=
                std::string::npos);  // server should echo this
        REQUIRE(response.find("Connection: close") != std::string::npos);
        REQUIRE(response.find("POST /submit") != std::string::npos);
    }
}