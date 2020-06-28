#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

using namespace std;
using namespace Pistache;

class StatsEndpoint {
   public:
    explicit StatsEndpoint(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr)) {}
    void init(size_t thr = 2);
    void start();

   private:
    void setupRoutes();
    void doRecordMetric(const Rest::Request& request,
                        Http::ResponseWriter response);
    void doGetMetric(const Rest::Request& request,
                     Http::ResponseWriter response);
    void doAuth(const Rest::Request& request, Http::ResponseWriter response);
    class Metric {
       public:
        explicit Metric(std::string name, int initialValue = 1)
            : name_(std::move(name)), value_(initialValue) {}
        int incr(int n = 1) {
            int old = value_;
            value_ += n;
            return old;
        }
        int value() const { return value_; }
        const std::string& name() const { return name_; }

       private:
        std::string name_;
        int value_;
    };
    using Lock = std::mutex;
    using Guard = std::lock_guard<Lock>;
    Lock metricsLock;
    std::vector<Metric> metrics;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

void StatsEndpoint::init(size_t thr) {
    auto opts = Http::Endpoint::options().threads(static_cast<int>(thr));
    httpEndpoint->init(opts);
    setupRoutes();
}
void StatsEndpoint::start() {
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}
void StatsEndpoint::setupRoutes() {
    using namespace Rest;
    // Routes::Post(router, "/record/:name:value?",
    //             Routes::bind(&StatsEndpoint::doRecordMetric, this));
    Routes::Post(router, "/record",
                 Routes::bind(&StatsEndpoint::doRecordMetric, this));
    Routes::Get(router, "/value/:name",
                Routes::bind(&StatsEndpoint::doGetMetric, this));
    Routes::Get(router, "/auth", Routes::bind(&StatsEndpoint::doAuth, this));
}

void StatsEndpoint::doRecordMetric(const Rest::Request& request,
                                   Http::ResponseWriter response) {
    std::cout << "doRecordMetric" << std::endl;
    cout << request.body();
    cout << "begin name" << endl;
    auto name = request.param(":name").as<std::string>();
    cout << "End name";
    Guard guard(metricsLock);
    auto it = std::find_if(
        metrics.begin(), metrics.end(),
        [&](const Metric& metric) { return metric.name() == name; });
    int val = 1;
    if (request.hasParam(":value")) {
        auto value = request.param(":value");
        val = value.as<int>();
    }

    if (it == std::end(metrics)) {
        metrics.push_back(Metric(std::move(name), val));
        response.send(Http::Code::Created, std::to_string(val));
    } else {
        auto& metric = *it;
        metric.incr(val);
        response.send(Http::Code::Ok, std::to_string(metric.value()));
    }
}

void StatsEndpoint::doGetMetric(const Rest::Request& request,
                                Http::ResponseWriter response) {
    auto name = request.param(":name").as<std::string>();

    Guard guard(metricsLock);

    auto it = std::find_if(
        metrics.begin(), metrics.end(),
        [&](const Metric& metric) { return metric.name() == name; });
    if (it == std::end(metrics)) {
        response.send(Http::Code::Not_Found, "Metric does not exist");
    } else {
        const auto& metric = *it;
        response.send(Http::Code::Ok, std::to_string(metric.value()));
    }
}

void StatsEndpoint::doAuth(const Rest::Request& request,
                           Http::ResponseWriter response) {
    auto cookies = request.cookies();
    std::cout << "Cookies: [" << std::endl;
    const std::string indent(4, ' ');
    for (const auto& c : cookies) {
        std::cout << indent << c.name << " = " << c.value << std::endl;
    }
    std::cout << "]" << std::endl;
    response.cookies().add(Http::Cookie("lang", "en-US"));
    response.send(Http::Code::Ok);
}

int main(int argc, char* argv[]) {
    Port port(9080);
    int thr = 2;

    Address addr(Ipv4::any(), port);
    cout << Ipv4::any().toString() << endl;
    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;

    StatsEndpoint stats(addr);

    stats.init(thr);
    stats.start();
}

