#include "../src/xios.hpp"
#include "./catch2/include/catch_amalgamated.hpp"

TEST_CASE("parseURL() correctly parses various formats", "[parseURL]") {
    SECTION("https://example.com") {
        auto p = SecureHttpClient::parseURL("https://example.com");
        REQUIRE(p.scheme == "https");
        REQUIRE(p.host == "example.com");
        REQUIRE(p.port == 443);
        REQUIRE(p.path == "/");
    }

    SECTION("http://example.com") {
        auto p = SecureHttpClient::parseURL("http://example.com");
        REQUIRE(p.scheme == "http");
        REQUIRE(p.host == "example.com");
        REQUIRE(p.port == 80);
        REQUIRE(p.path == "/");
    }

    SECTION("example.com") {
        auto p = SecureHttpClient::parseURL("example.com");
        REQUIRE(p.scheme == "https");
        REQUIRE(p.host == "example.com");
        REQUIRE(p.port == 443);
        REQUIRE(p.path == "/");
    }

    SECTION("example.com:8080") {
        auto p = SecureHttpClient::parseURL("example.com:8080");
        REQUIRE(p.scheme == "https");
        REQUIRE(p.host == "example.com");
        REQUIRE(p.port == 8080);
        REQUIRE(p.path == "/");
    }

    SECTION("http://example.com:3000") {
        auto p = SecureHttpClient::parseURL("http://example.com:3000");
        REQUIRE(p.scheme == "http");
        REQUIRE(p.host == "example.com");
        REQUIRE(p.port == 3000);
        REQUIRE(p.path == "/");
    }

    SECTION("https://example.com:8443/path/to/resource") {
        auto p = SecureHttpClient::parseURL(
            "https://example.com:8443/path/to/resource");
        REQUIRE(p.scheme == "https");
        REQUIRE(p.host == "example.com");
        REQUIRE(p.port == 8443);
        REQUIRE(p.path == "/path/to/resource");
    }

    SECTION("192.168.0.1") {
        auto p = SecureHttpClient::parseURL("192.168.0.1");
        REQUIRE(p.scheme == "https");
        REQUIRE(p.host == "192.168.0.1");
        REQUIRE(p.port == 443);
        REQUIRE(p.path == "/");
    }

    SECTION("192.168.0.1:1234") {
        auto p = SecureHttpClient::parseURL("192.168.0.1:1234");
        REQUIRE(p.scheme == "https");
        REQUIRE(p.host == "192.168.0.1");
        REQUIRE(p.port == 1234);
        REQUIRE(p.path == "/");
    }

    SECTION("http://192.168.1.10:5000/test") {
        auto p = SecureHttpClient::parseURL("http://192.168.1.10:5000/test");
        REQUIRE(p.scheme == "http");
        REQUIRE(p.host == "192.168.1.10");
        REQUIRE(p.port == 5000);
        REQUIRE(p.path == "/test");
    }

    SECTION("www.example.com:11235/some/endpoint") {
        auto p =
            SecureHttpClient::parseURL("www.example.com:11235/some/endpoint");
        REQUIRE(p.scheme == "https");
        REQUIRE(p.host == "www.example.com");
        REQUIRE(p.port == 11235);
        REQUIRE(p.path == "/some/endpoint");
    }
}