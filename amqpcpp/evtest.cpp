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

    channel.declareExchange("cpp-exchange1", AMQP::fanout)
        .onSuccess([]() { cout << "desclarExchange: success" << endl; })
        .onError([](const char *message) {
             cout << "declareExchange: " << message << endl;
         });

    //设置队列的属性
    //AMQP::durable,持久化，如果启用，队列将会在server重启前后都有效
    //AMQP::autodelete,自动删除，如果启用，队列会在所有的消费者停止使用后自动删除自身
    //AMQP::passive,惰性，如果没有声明队列，那么在执行到使用的时候会导致异常，多用于检查队列是否存在
    //AMQP::exclusive，排他性，他的队列仅针对该连接而存在，并且在连接断开后会自动删除
    channel.declareQueue("cpp-test", AMQP::durable)
        .onSuccess([&connection](const std::string &name, uint32_t messagecount,
                                 uint32_t consumercount) {
             std::cout << "decalared queue: " << name << std::endl;
             connection.close();
         });
    //绑定队列
    channel.bindQueue("cpp-exchange", "cpp-test", "my-routing-key")
        .onSuccess([]() { cout << "bindQueue: success" << endl; });
    //发送消息
    channel.publish("cpp-exchange", "my-routing-key", first message"");
    
    //消费消息
    auto startCb = [](const std::string &consumertag){
        std::cout << "consume operation started" << std::endl;
    };
    auto errorCb = [](const char* message){
        std::cout << "consume operation failed " << std::endl;
    };
    auto messageCb = [&channel](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered){
        std::cout << "message received " << std::endl;
        //acknowledege the message
        channel.ack(deliveryTag);
    };
    //注意：onSuccess仅表示消费消息的操作开始，而onReceived才代表收到了数据
    
    //AMQP::Message类包含了消息的全部内容，包括真实数据，消息头，甚至exchange和routingkey。
    
    //deliveryTag是收到消息后的特定标识符，使用它返回响应。RabbitMq会在接收到响应后删除消息，
    //如果忘记执行 Channel:ack() 方法，RabbitMq会因为内存用尽而崩溃
    
    //消息的消费是持续进行的，直到使用 Channel::cancel() 停止消费。如果关闭通道或者TCP连接，消费也会停止。
    //可以通过QOS防止消费者被大量消息搞崩溃。QOS决定了消费者可以持有的未发送ack的数据量，RabbitMq会暂存后续的消息。
    //该属性通过 Channel::setQos() 设置
    channel.consume("cpp-test").onReceived(messageCb).onSuccess(startCb).onError(errorCb);
    

    ev_run(loop, 0);
    return 0;
}
