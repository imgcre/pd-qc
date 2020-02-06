#pragma once
#include <functional>
#include "cmsis_os.h"
//Callback::invoke()
namespace Utilities {
using namespace std;
  
template<class Function>
class Callback { //������Ҫ��һ�����߳���ִ��
  
public:
  
  //��Ҫ���߳�ģʽ�µ���, ���ﴫ����Ϣ����id
  Callback(osMessageQueueId_t queueId) : queueId(queueId) {
    
  }

  //���߳�ģʽ�µ���
  void poll() { //�߳�ʲô�����û�����, ֻҪ���û����߳��е���poll�������
    function<void()>* pCb;
    for(;;) {
      if(osMessageQueueGet(queueId, pCb, 0, osWaitForever) == osOK) {
        (*pCb)();
        delete pCb;
      }
    }
  }

  
  //���ж�ģʽ�µ���
  template<class... Args>
  void invoke(function<Function> cb, Args&&... args) {
    //NOTE: ��ôд����������
    auto pCb = new function<void()> {[=]{
      cb(forward(args)...);
    }};
    osMessageQueuePut(queueId, pCb, 0, 0);
  }
  
private:
  osMessageQueueId_t queueId;
 
};
}