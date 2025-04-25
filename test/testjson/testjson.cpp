#include "../../thirdparty/json.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <string>

using json = nlohmann::json;

using namespace std;

void func1()
{
  json js;
  js["msg_type"] = 2;
  js["from"] = "zhang san";
  js["to"] = "li si";
  js["msg"] = "Hello ,what are you doing";

  string sendbuf = js.dump();
  cout << sendbuf.c_str() << endl;
}

void func2()
{
  json js;
  js["id"] = {1,2,3,4,5};
  js["name"] = "zhang san";

  map<string,map<string,string>> map;
  js["msg"]["zhang san"] = "hello world";
  js["msg"]["liu shuo"] = "hello china";

  js["msg"] = {{"zhang san","hello world"},{"liu shuo","hello china"}};
  cout << js << endl;
}

//json序列化示例代码3
void func3()
{
  json js;

  vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(5);

  js["list"] = vec;

  map<int,string> m;
  m.insert({1,"黄山"});
  m.insert({2,"华山"});
  m.insert({3,"泰山"});

  js["path"] = m;
  
  string sendbuf = js.dump();
  cout << sendbuf.c_str() << endl;
}

string func4()
{
  json js;
  js["msg_type"] = 2;
  js["from"] = "zhang san";
  js["to"] = "li si";
  js["msg"] = "Hello ,what are you doing";

  string sendbuf = js.dump();
  // cout << sendbuf.c_str() << endl;
  return sendbuf;
}

string func5()
{
  json js;
  js["id"] = {1,2,3,4,5};
  js["name"] = "zhang san";

  map<string,map<string,string>> map;
  js["msg"]["zhang san"] = "hello world";
  js["msg"]["liu shuo"] = "hello china";

  js["msg"] = {{"zhang san","hello world"},{"liu shuo","hello china"}};
  string sendbuf = js.dump();
  return sendbuf;
}

string func6()
{
  json js;

  vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(5);

  js["list"] = vec;

  map<int,string> m;
  m.insert({1,"黄山"});
  m.insert({2,"华山"});
  m.insert({3,"泰山"});

  js["path"] = m;
  
  string sendbuf = js.dump();
  return sendbuf;
}

int main()
{
  string recvBuf = func6();

  //数据的反序列化 json字符串 => 反序列化 数据对象
  
  // json jsbuf = json::parse(recvBuf);
  // cout << jsbuf["msg_type"] << endl;
  // cout << jsbuf["from"] << endl;
  // cout << jsbuf["to"] << endl;
  // cout << jsbuf["msg"] << endl;

  // json jsBuf = json::parse(recvBuf);
  // auto arr = jsBuf["id"];
  // cout << arr[3] << endl;
  //
  // auto arr2 = jsBuf["msg"];
  // cout << arr2["zhang san"] << endl;
  // cout << arr2["liu shuo"] << endl;

  json jsbuf = json::parse(recvBuf);
  vector<int> vec = jsbuf["list"];
  for(auto& element: vec)
  {
    cout << element << endl;
  }

  map<int,string> m = jsbuf["path"];
  for(auto& element : m)
  {
    cout << element.first << ":" << element.second << endl;
    
  }
   

  return 0;
}

