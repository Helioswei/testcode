#include <iostream>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>

using namespace std;
/**
 *  Class that runs a timer
 */
class MyTimer
{
private:
    /**
     *  The actual watcher structure
     *  @var struct ev_io
     */
    struct ev_timer _timer;

    /**
     *  Pointer towards the AMQP channel
     *  @var AMQP::TcpChannel
     */
    AMQP::TcpChannel *_channel;

    /**
     *  Name of the queue
     *  @var std::string
     */
    std::string _queue;


    /**
     *  Callback method that is called by libev when the timer expires
     *  @param  loop        The loop in which the event was triggered
     *  @param  timer       Internal timer object
     *  @param  revents     The events that triggered this call
     */
    static void callback(struct ev_loop *loop, struct ev_timer *timer, int revents)
    {
        // retrieve the this pointer
        MyTimer *self = static_cast<MyTimer*>(timer->data);

        // publish a message
        self->_channel->publish("", self->_queue, "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ");
    }

public:
    /**
     *  Constructor
     *  @param  loop
     *  @param  channel
     *  @param  queue
     */
    MyTimer(struct ev_loop *loop, AMQP::TcpChannel *channel, std::string queue) : 
        _channel(channel), _queue(std::move(queue))
    {
        // initialize the libev structure
        // 1s后执行一次回调，之后每10s执行一次
        ev_timer_init(&_timer, callback, 1, 10);

        // this object is the data
        _timer.data = this;

        // and start it
        ev_timer_start(loop, &_timer);
    }
    
    /**
     *  Destructor
     */
    virtual ~MyTimer()
    {
        // @todo to be implemented
    }
};



int main() {
    auto *loop = EV_DEFAULT;
    AMQP::LibEvHandler handler(loop);

    AMQP::Address address("amqp://admin:datatom.com@192.168.50.97:31680");
    AMQP::TcpConnection connection(&handler, address);

    AMQP::TcpChannel channel(&connection);
    //设置队列的属性
    // AMQP::durable,持久化，如果启用，队列将会在server重启前后都有效
    // AMQP::autodelete,自动删除，如果启用，队列会在所有的消费者停止使用后自动删除自身
    // AMQP::passive,惰性，如果没有声明队列，那么在执行到使用的时候会导致异常，多用于检查队列是否存在
    // AMQP::exclusive，排他性，他的队列仅针对该连接而存在，并且在连接断开后会自动删除
    AMQP::Table args;
    args.set("x-max-priority", 10);  // 设置队列的优先级
    // args.set("x-max-length", 5); //设置队列的最大长度
    // args.set("x-max-length-bytes",100);//设置队列的最大自己数目

    channel.declareQueue("cpp-test", AMQP::durable, args)
        .onSuccess([&connection,&channel, loop](const std::string &name, uint32_t messagecount,
                                 uint32_t consumercount) {
             std::cout << "decalared queue: " << name << std::endl;
             auto *timer = new MyTimer(loop, &channel, name);
             //connection.close();
         })
        .onError([](const char *message) {
             cout << "decalared queue failed: " << message << endl;
         });

    ev_run(loop, 0);
    return 0;
}
