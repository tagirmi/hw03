#include <algorithm>
#include <iostream>
#include <map>

#include "allocator.h"

namespace {

template<typename T>
constexpr T factorial(T n)
{
  static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");

  return (n <= 1) ? 1 : n * factorial(n - 1);
}

template<typename T>
void printMap(const T& map)
{
  for (const auto& i : map)
    std::cout << i.first << ' ' << i.second << std::endl;
}

constexpr size_t elementsCount = 10;

}

using StdMap = std::map<int, int>;

using CustomAllocatorMap =
std::map<int, int, std::less<int>, hw03::allocator<std::pair<int, int>, elementsCount>>;

int main(int argc, char const *argv[])
{
  try {
    auto mapGenerator = [i = 0] () mutable {
      auto f = factorial(i);
      return std::make_pair(i++, f);
    };

    StdMap stdMap;
    std::generate_n(std::inserter(stdMap, stdMap.begin()), elementsCount, mapGenerator);

    CustomAllocatorMap customAllocatorMap;
    std::generate_n(std::inserter(customAllocatorMap, customAllocatorMap.begin()), elementsCount, mapGenerator);

    std::cout << "std::map: " << std::endl;
    printMap(stdMap);

    std::cout <<std::endl;
    std::cout << "std::map with custom allocator: " << std::endl;
    printMap(customAllocatorMap);
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
