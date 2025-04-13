#include "test_parseURL.h"


void testParseURL() {
    {
        auto p = SecureHttpClient::parseURL("https://example.com");
        assert(p.scheme == "https");
        assert(p.host == "example.com");
        assert(p.port == 443);
        assert(p.path == "/");
    }

    {
        auto p = SecureHttpClient::parseURL("http://example.com");
        assert(p.scheme == "http");
        assert(p.host == "example.com");
        assert(p.port == 80);
        assert(p.path == "/");
    }

    {
        auto p = SecureHttpClient::parseURL("example.com");
        assert(p.scheme == "https");
        assert(p.host == "example.com");
        assert(p.port == 443);
        assert(p.path == "/");
    }

    {
        auto p = SecureHttpClient::parseURL("example.com:8080");
        assert(p.scheme == "https");
        assert(p.host == "example.com");
        assert(p.port == 8080);
        assert(p.path == "/");
    }

    {
        auto p = SecureHttpClient::parseURL("http://example.com:3000");
        assert(p.scheme == "http");
        assert(p.host == "example.com");
        assert(p.port == 3000);
        assert(p.path == "/");
    }

    {
        auto p = SecureHttpClient::parseURL("https://example.com:8443/path/to/resource");
        assert(p.scheme == "https");
        assert(p.host == "example.com");
        assert(p.port == 8443);
        assert(p.path == "/path/to/resource");
    }

    {
        auto p = SecureHttpClient::parseURL("192.168.0.1");
        assert(p.scheme == "https");
        assert(p.host == "192.168.0.1");
        assert(p.port == 443);
        assert(p.path == "/");
    }

    {
        auto p = SecureHttpClient::parseURL("192.168.0.1:1234");
        assert(p.scheme == "https");
        assert(p.host == "192.168.0.1");
        assert(p.port == 1234);
        assert(p.path == "/");
    }

    {
        auto p = SecureHttpClient::parseURL("http://192.168.1.10:5000/test");
        assert(p.scheme == "http");
        assert(p.host == "192.168.1.10");
        assert(p.port == 5000);
        assert(p.path == "/test");
    }

    {
        auto p = SecureHttpClient::parseURL("www.example.com:11235/some/endpoint");
        assert(p.scheme == "https");
        assert(p.host == "www.example.com");
        assert(p.port == 11235);
        assert(p.path == "/some/endpoint");
    }

    std::cout << "✅ All parseURL() tests passed.\n";
}