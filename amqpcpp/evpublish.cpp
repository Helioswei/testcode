#include <iostream>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>

using namespace std;
void aa() { cout << "desclarExchange AA: success" << endl; }
void bb(const char *message) {
    cout << "declareExchange BB: " << message << endl;
}
int main() {
    auto *loop = EV_DEFAULT;
    AMQP::LibEvHandler handler(loop);
    string exName = "cpp-exchange1";
    //string quName = "cpp-test1";
    string quName = "transcode";

    AMQP::Address address("amqp://admin:datatom.com@192.168.50.97:32550");
    AMQP::TcpConnection connection(&handler, address);

    AMQP::TcpChannel channel(&connection);

    auto succss = []() { cout << "desclarExchange: success" << endl; };
    channel.declareExchange(exName, AMQP::fanout).onSuccess(aa).onError(
        bb);

    //设置队列的属性
    // AMQP::durable,持久化，如果启用，队列将会在server重启前后都有效
    // AMQP::autodelete,自动删除，如果启用，队列会在所有的消费者停止使用后自动删除自身
    // AMQP::passive,惰性，如果没有声明队列，那么在执行到使用的时候会导致异常，多用于检查队列是否存在
    // AMQP::exclusive，排他性，他的队列仅针对该连接而存在，并且在连接断开后会自动删除
    AMQP::Table args;
    args.set("x-max-priority", 10);  // 设置队列的优先级
    // args.set("x-max-length", 5); //设置队列的最大长度
    // args.set("x-max-length-bytes",100);//设置队列的最大自己数目

    channel.declareQueue(quName, AMQP::durable, args)
        .onSuccess([&connection](const std::string &name, uint32_t messagecount,
                                 uint32_t consumercount) {
             std::cout << "decalared queue: " << name << std::endl;
             connection.close();
         })
        .onError(bb);
    //绑定队列
    channel.bindQueue(exName, quName, "my-routing-key")
        .onSuccess([]() { cout << "bindQueue: success" << endl; })
        .onError([](const char *message) {
             cout << "bindQueue Failed: " << message << endl;
         });
    //发送消息
    //string aa = "{\"taskId\":\"111\",\"taskDetail\":{\"srcpath\":\"\/root\/video1\/video-H263-AC3.avi\",\"destpath\":\"test.mp4\",\"videocodec\":\"H264\",\"audiocodec\":\"AAC\"}}";
    string aa = "{\"taskId\":\"111\",\"taskDetail\":{\"srcpath\":\"wei:video-H263-AC3.avi\",\"srchost\":\"192.168.50.186:7480\",\"srcaksk\":\"791f66218f5a090391b38ef501811ffc,18c7cf6f142b9f0e194183dac6d15926\",\"destpath\":\"wei:transcode.mp4\",\"desthost\":\"192.168.50.186:7480\",\"destaksk\":\"791f66218f5a090391b38ef501811ffc,18c7cf6f142b9f0e194183dac6d15926\",\"videocodec\":\"H264\",\"audiocodec\":\"AAC\"}}";
    string bb = "{\"taskId\":\"222\",\"taskDetail\":{\"name1\":\"weiyang\"}}";
    string dd = "{\"taskId\":\"333\",\"taskDetail\":{\"srcpath\":\"wei:video-H263-AC3.avi\",\"srchost\":\"192.168.50.186:7480\",\"srcaksk\":\"791f66218f5a090391b38ef501811ffc,18c7cf6f142b9f0e194183dac6d15926\",\"destpath\":\"wei:interce.avi\",\"desthost\":\"192.168.50.186:7480\",\"destaksk\":\"791f66218f5a090391b38ef501811ffc,18c7cf6f142b9f0e194183dac6d15926\",\"start\":\"30\",\"end\":\"50\"}}";
    string cc = "{\"taskId\":\"444\",\"taskDetail1\":{\"name1\":\"weiyang\"}}";
    string task1 = "{\"taskId\":\"555\",\"taskDetail\":{\"srcpath\":\"wei:video-H263-AC3.avi\",\"srchost\":\"192.168.50.186:7480\",\"srcaksk\":\"791f66218f5a090391b38ef501811ffc,18c7cf6f142b9f0e194183dac6d15926\",\"destpath\":\"wei:transcode1.mp4\",\"desthost\":\"192.168.50.186:7480\",\"destaksk\":\"791f66218f5a090391b38ef501811ffc,18c7cf6f142b9f0e194183dac6d15926\",\"videocodec\":\"H264\",\"audiocodec\":\"AAC\"}}";
    channel.publish(exName, "my-routing-key", aa);
    channel.publish(exName, "my-routing-key", dd);
    channel.publish(exName, "my-routing-key", bb);
    channel.publish(exName, "my-routing-key", cc);
    channel.publish(exName, "my-routing-key", task1);
    //设置消息的优先级
    //string name("five message");
    //AMQP::Envelope enve(name.c_str(), name.length());
    //enve.setPriority(10);
    //channel.publish("cpp-exchange", "my-routing-key", enve);

    ev_run(loop, 0);
    return 0;
}
