#pragma once
#include <functional>
#include "cmsis_os.h"
//Callback::invoke()
namespace Utilities {
using namespace std;
  
template<class Function>
class Callback { //本类需要在一个新线程中执行
  
public:
  
  //需要在线程模式下调用, 这里传递消息队列id
  Callback(osMessageQueueId_t queueId) : queueId(queueId) {
    
  }

  //在线程模式下调用
  void poll() { //线程什么的让用户定义, 只要求用户在线程中调用poll这个函数
    function<void()>* pCb;
    for(;;) {
      if(osMessageQueueGet(queueId, pCb, 0, osWaitForever) == osOK) {
        (*pCb)();
        delete pCb;
      }
    }
  }

  
  //在中断模式下调用
  template<class... Args>
  void invoke(function<Function> cb, Args&&... args) {
    //NOTE: 这么写可能有问题
    auto pCb = new function<void()> {[=]{
      cb(forward(args)...);
    }};
    osMessageQueuePut(queueId, pCb, 0, 0);
  }
  
private:
  osMessageQueueId_t queueId;
 
};
}