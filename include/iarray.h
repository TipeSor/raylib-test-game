#ifndef IARRAY
#define IARRAY

#include <cmath>
#include <functional>
#include <initializer_list>
#include <sstream>
#include <string>

template <typename T> class iarray {
public:
  template <typename It> iarray(It begin, It end) : arr(begin, end){};
  iarray(T *arr, size_t size) : arr(arr, arr + size) {};
  iarray(std::initializer_list<T> arr) : arr(arr) {};

  const std::function<void(double)> change = [&](double delta) {
    if (arr.empty())
      return;
    int sign = std::signbit(delta) ? -1 : 1;
    index = (index + arr.size() + sign) % arr.size();
  };

  const std::function<std::string()> str = [&]() {
    std::ostringstream oss;
    oss << arr[index];
    return oss.str();
  };

  T getSelected() { return arr.at(index); }

private:
  std::vector<T> arr;
  size_t index = 0;
};

#endif // IARRAY
