#ifndef REDIS_H
#define REDIS_H

#include <hiredis/hiredis.h>
#include <string>
#include <functional>
using namespace std;

class Redis
{
public:
  Redis();
  ~Redis();

  //连接Redis服务器
  bool connect();

  //向redis指定的通道channel发布消息
  bool publish(int channel,string message);

  //向redis指定的通道subsribe订阅消息
  bool subcribe(int channel);

  //向redis指定的通道ubsubscribe取消订阅消息
  bool unsubscribe(int channel);

  //在独立的线程中接受订阅消息通道中的信息
  void observer_channel_message();

  //初始化向业务层上报通道消息的回掉对象
  void init_notify_handler(function<void(int,string)> fn);
private:
  //hiredis同步上下文对象，负责publish消息
  redisContext* _publish_context;

  //hiredis同步上下文对象，负责subscribe消息
  redisContext* _subcribe_context;

  //回掉操作，受到订阅的消息，给service层上报
  function<void(int,string)> _notify_message_handler;
};



#endif

