#pragma once
#include <utility>
#include <functional>

//TODO: ´ý²âÊÔ
namespace Utilities {
template <class T>
class Singleton {
public:
  template<class... Args>
  static T &instance(Args&&... args) {
		return instanceInternal([&]{return T(std::forward<Args>(args)...);});
	};
  
  Singleton() = default;
  Singleton(const Singleton& s) = delete;
  
private:
  static T &instanceInternal(std::function<T()> getter) {
    static T inst = getter();
    return inst;
  }
};

}