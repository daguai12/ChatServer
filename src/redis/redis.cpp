#include "../../include/server/redis/redis.hpp"
#include <hiredis/hiredis.h>
#include <iostream>
#include <thread>

Redis::Redis()
{

}

Redis::~Redis()
{

}


//连接Redis服务器
bool Redis::connect()
{
  //负责publish发布消息的上下文连接
  _publish_context = redisConnect("127.0.0.1",6379);
  if(nullptr == _publish_context)
  {
    cerr << "connect redis failed!" << endl;
    return false;
  }

  //负责subscribe订阅消息的上下文连接
  _subcribe_context = redisConnect("127.0.0.1", 6379);
  if(nullptr == _subcribe_context)
  {
    cerr << "connect redis failed!" << endl;
    return false;
  }

  //在单独的线程中，监听通道上的事件，有消息给业务层上报
  thread t([&](){
    observer_channel_message();
  });

  t.detach();

  cout << "connect redis-server success!" << endl;

  return true;
}

//向redis指定的通道channel发布消息
bool Redis::publish(int channel,string message)
{
  redisReply* reply = (redisReply*)redisCommand(_publish_context,"PUBLISH %d %d",channel,message.c_str());
  if(nullptr == reply)
  {
    cerr << "publish command failied!" << endl;
  }
  freeReplyObject(reply);
  return true;
}

//向redis指定的通道subsribe订阅消息
bool Redis::subcribe(int channel)
{
  //SUBCRIBE命令本身会造成线程阻塞等待通道里面发生消息，这里只做通道订阅，不接受通道信息
  //通道信息的接收专门在observer_channel_message函数中独立线程中进行
  //只负责发送命令，不阻塞接受redis server响应消息，否则和notifyMsg线程抢占响应资源
  if(REDIS_ERR == redisAppendCommand(this->_subcribe_context,"SUBCRIBE %d",channel))
  {
    cerr << "subscribe command failed!" << endl;
  }
  //redisBufferWrite可以循环发送缓冲区，知道缓冲区数据发送完毕（done被置1）
  int done = 0;
  while(!done)
  {
    if(REDIS_ERR == redisBufferWrite(this->_subcribe_context,&done))
    {
      cerr << "subscribe command failed!" << endl;
      return false;
    }
  }
  return true;
}

//向redis指定的通道ubsubscribe取消订阅消息
bool Redis::unsubscribe(int channel)
{
  if(REDIS_ERR == redisAppendCommand(this->_subcribe_context,"UNSUBSCRIBE %d",channel))
  {
    cerr << "unsubscribe command failed!" << endl;
    return false;
  }
  //redisBufferWrite可以循环发送缓冲区，直到缓冲区数据发生完毕（done被置为1）
  int done = 0;
  while(!done)
  {
    if(REDIS_ERR == redisBufferWrite(this->_subcribe_context,&done))
    {
      cerr << "unsubscribe commadn failed!" << endl;
      return false;
    }
  }
  return true;
}

//在独立的线程中接受订阅消息通道中的信息
void Redis::observer_channel_message()
{
  redisReply* reply = nullptr;
  while(REDIS_OK == redisGetReply(this->_subcribe_context, (void**)&reply))
  {
    //订阅收到的消息是一个三元组
    if(reply != nullptr && reply->element[1] != nullptr && reply->element[2]->str != nullptr)
    {
      //给业务层上报通道上发送的消息
      _notify_message_handler(atoi(reply->element[1]->str),reply->element[2]->str);
    }
    freeReplyObject(reply);
  }
  cerr << ">>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<" << endl;
}

//初始化向业务层上报通道消息的回掉对象
void Redis::init_notify_handler(function<void(int,string)> fn)
{
  this->_notify_message_handler = fn;
}

