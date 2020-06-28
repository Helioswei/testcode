#include <iostream>
#include <pistache/endpoint.h>
using namespace Pistache;

struct HelloHandler : public Http::Handler {
    HTTP_PROTOTYPE(HelloHandler)
    void onRequest(const Http::Request& request, Http::ResponseWriter writer) override {
        Address add = request.address();
        std::string host = add.host();
        writer.send(Http::Code::Ok, "Hello, World!:" + host);
    }
};

int main() { Http::listenAndServe<HelloHandler>(Pistache::Address("*:9080")); }
