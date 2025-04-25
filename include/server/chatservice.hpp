#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TcpConnection.h>
#include "../../thirdparty/json.hpp"
#include <unordered_map>
#include <functional>
#include <mutex>

#include "redis/redis.hpp"
#include "model/usermodel.hpp"
#include "model/offlinemessagemodel.hpp"
#include "model/friendmodel.hpp"
#include "model/groupmodel.hpp"

using namespace std;
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;


//表示处理消息的事件回掉方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr& conn,json& js,Timestamp)>;

  
// 聊天服务器业务类
class ChatService
{
public:
  //获取单例对象的接口函数
  static ChatService* instance();
  //处理登陆业务
  void login(const TcpConnectionPtr& conn,json& js,Timestamp);
  //处理注册业务
  void reg(const TcpConnectionPtr& conn,json& js,Timestamp);
  //一对一聊天业务
  void oneChat(const TcpConnectionPtr& conn,json& js,Timestamp);
  //添加好友业务
  void addFriend(const TcpConnectionPtr& conn,json&  js,Timestamp);
  //创建群组业务
  void createGroup(const TcpConnectionPtr& conn,json& js,Timestamp);
  //加入群组业务
  void addGroup(const TcpConnectionPtr& conn,json& js,Timestamp);
  //群组聊天业务
  void groupChat(const TcpConnectionPtr& conn,json& js,Timestamp);
  //处理注销业务
  void loginout(const TcpConnectionPtr& conn,json& js,Timestamp time);
  //服务器异常后，业务重置方法
  void reset();
  //处理客户端异常退出
  void clientCloseException(const TcpConnectionPtr& conn);
  //获取消息对应的处理器
  MsgHandler getHandler(int msgid);
  //从redis消息队列中获取订阅消息
  void handlerRedisSubscribeMessage(int userid,string msg);
private:
  ChatService();

  //存储消息id和其对应的业务处理方法
  unordered_map<int, MsgHandler> _msgHandlerMap;

  //存储在线用户的通信连接
  unordered_map<int, TcpConnectionPtr> _userConnMap;

  //定义互斥锁
  mutex _connMutex;
  //数据操作类对象
  UserModel _userModel;
  offlineMsgModel _offlineMsgModel;
  FriendModel _friendModel;
  GroupModel _groupModel;

  //redis操作对象
  Redis _redis;
};

#endif

