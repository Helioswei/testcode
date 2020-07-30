#include <iostream>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>

using namespace std;
int main() {
    auto *loop = EV_DEFAULT;
    AMQP::LibEvHandler handler(loop);

    AMQP::Address address("amqp://admin:datatom.com@192.168.50.97:31680");
    AMQP::TcpConnection connection(&handler, address);

    AMQP::TcpChannel channel(&connection);

    channel.declareExchange("cpp-exchange", AMQP::fanout)
        .onSuccess([]() { cout << "desclarExchange: success" << endl; })
        .onError([](const char *message) {
             cout << "declareExchange: " << message << endl;
         });

    //设置队列的属性
    // AMQP::durable,持久化，如果启用，队列将会在server重启前后都有效
    // AMQP::autodelete,自动删除，如果启用，队列会在所有的消费者停止使用后自动删除自身
    // AMQP::passive,惰性，如果没有声明队列，那么在执行到使用的时候会导致异常，多用于检查队列是否存在
    // AMQP::exclusive，排他性，他的队列仅针对该连接而存在，并且在连接断开后会自动删除
    AMQP::Table args;
    args.set("x-max-priority", 10);// 设置队列的优先级
    //args.set("x-max-length", 5); //设置队列的最大长度
    //args.set("x-max-length-bytes",100);//设置队列的最大自己数目
    
    channel.declareQueue("cpp-test", AMQP::durable, args)
        .onSuccess([&connection](const std::string &name, uint32_t messagecount,
                                 uint32_t consumercount) {
             std::cout << "decalared queue: " << name << std::endl;
             connection.close();
         })
        .onError([](const char *message) {
             cout << "decalared queue failed: " << message << endl;
         });
    //绑定队列
    channel.bindQueue("cpp-exchange", "cpp-test", "my-routing-key")
        .onSuccess([]() { cout << "bindQueue: success" << endl; })
        .onError([](const char *message) {
             cout << "bindQueue Failed: " << message << endl;
         });
    //发送消息
    channel.publish("cpp-exchange", "my-routing-key",  "first message");
    channel.publish("cpp-exchange", "my-routing-key",  "second message");
    channel.publish("cpp-exchange", "my-routing-key",  "three message");
    channel.publish("cpp-exchange", "my-routing-key",  "four message");
    //设置消息的优先级
    string name("five message");
    AMQP::Envelope enve(name.c_str(), name.length());
    enve.setPriority(10);
    channel.publish("cpp-exchange", "my-routing-key",  enve);
    channel.publish("cpp-exchange", "my-routing-key",  "six message");

    ev_run(loop, 0);
    return 0;
}
