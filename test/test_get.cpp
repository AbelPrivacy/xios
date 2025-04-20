#include "test_get.h"
#include <iostream>


using namespace std;

void testGet() {
  cout << 0 << endl;
    std::string db =  "./metadata.db";
    std::vector<std::string> proto;
    proto.push_back("TLSv1.3");
    std::vector<std::string> empty0;
    std::vector<std::string> empty1;
    std::vector<std::string> empty2;
    SecureHttpClient::initialize(
      db,
      proto,
      empty0,
      empty1,
      empty2 // or ['KYBER_LEVEL1'] if supported
    );
    std::string response = SecureHttpClient::get("https://localhost/request/");
    assert(response.find("HTTP/1.1 200 OK") == 0);
    assert(response.find("Server: Werkzeug/3.1.3 Python/3.13.3") != -1);
    assert(response.find("Content-Type: text/html; charset=utf-8") != -1);
    assert(response.find("Connection: close") != -1);
    assert(response.find("GET /requests/") != -1);
    std::cout << "✅ All get() tests passed.\n";
}