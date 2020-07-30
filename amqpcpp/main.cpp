#include <iostream>
#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>

using namespace std;
// 1,首先，写一个继承自AMQP::TcpHandler的类，它负责网络层的TCP链接，需要重写相关的函数
//其中必须重写monitor()函数，如下：
class MyTcpHandler : public AMQP::TcpHandler {
    virtual void onAttached(AMQP::TcpConnection *connection) override {}
    virtual void onConnected(AMQP::TcpConnection *connection) override {}
    virtual bool onSecured(AMQP::TcpConnection *connection,
                           const SSL *ssl) override {
        return true;
    }
    virtual void onReady(AMQP::TcpConnection *connection) override {}
    virtual void onError(AMQP::TcpConnection *, const char *message) override {}
    virtual void onClosed(AMQP::TcpConnection *connection) override {}
    virtual void onLost(AMQP::TcpConnection *connection) override {}
    virtual void onDetached(AMQP::TcpConnection *connection) override {}
    virtual void monitor(AMQP::TcpConnection *connection, int fd,
                         int flags) override {}
};

int main() {
    MyTcpHandler myHandler;

    // address of the server
    AMQP::Address address("amqp://admin:datatom.com@192.168.50.97:31680");

    // create a AMQP connection object
    AMQP::TcpConnection connection(&myHandler, address);

    // and create a channel
    AMQP::TcpChannel channel(&connection);

    // ust the channel object to call the AMQP method you like

    channel.declareExchange("mycpp", AMQP::fanout)
        .onSuccess([]() { cout << "Success" << endl; })
        .onError([](const char *message) {
             cout << "channnel error: " << message << std::endl;
         });

    channel.declareQueue("mycpp-queue");
    channel.bindQueue("mycpp", "mycpp-queue", "my-routing-kkey");
}

