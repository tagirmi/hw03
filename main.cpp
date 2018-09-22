#include <algorithm>
#include <iostream>
#include <map>

#include "allocator.h"
#include "container.h"

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

template<typename T>
void printContainer(const T& container)
{
  for (const auto& i : container)
    std::cout << i << std::endl;
}

constexpr size_t elementsCount = 10;

template<typename T>
void fillMap(T& map)
{
  auto generator = [i = 0] () mutable {
    auto f = factorial(i);
    return std::make_pair(i++, f);
  };
  std::generate_n(std::inserter(map, std::begin(map)), elementsCount, generator);
}

template<typename T>
void fillContainer(T& container)
{
  for (size_t i = 0; i < elementsCount; ++i)
    container.push_back(i);
}

void testStdMap()
{
  std::map<int, int> map;
  fillMap(map);

  std::cout << "std::map: " << std::endl;
  printMap(map);
  std::cout <<std::endl;
}

void testCustomAllocatorMap()
{
  using Map = std::map<int, int, std::less<int>, hw03::allocator<std::pair<int, int>, elementsCount>>;

  Map map;
  fillMap(map);

  std::cout << "std::map with custom allocator: " << std::endl;
  printMap(map);
  std::cout <<std::endl;
}

void testCustomContainer()
{
  hw03::forward_list<int> container;
  fillContainer(container);

  std::cout << "custom container: " << std::endl;
  printContainer(container);
  std::cout <<std::endl;
}

void testCustomContainerWithCustomAllocator()
{
  using CustomAllocatorAndContainer = hw03::forward_list<int, hw03::allocator<int, elementsCount>>;

  CustomAllocatorAndContainer container;
  fillContainer(container);

  std::cout << "custom container with custom allocator: " << std::endl;
  printContainer(container);
  std::cout <<std::endl;
}

} // namespace

int main(int argc, char const *argv[])
{
  try {
    testStdMap();
    testCustomAllocatorMap();
    testCustomContainer();
    testCustomContainerWithCustomAllocator();
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
