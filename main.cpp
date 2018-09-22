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

template<typename ...Args>
std::ostream& operator<<(std::ostream& stream, const std::map<Args...>& map)
{
  for (const auto& i : map)
    stream << i.first << ' ' << i.second << std::endl;
  return stream;
}

template<typename ...Args>
std::ostream& operator<<(std::ostream& stream, const hw03::forward_list<Args...>& container)
{
  for (const auto& i : container)
    stream << i << std::endl;
  return stream;
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
  std::cout << "std::map: " << std::endl << map << std::endl;
}

void testCustomAllocatorMap()
{
  using Map = std::map<int, int, std::less<int>, hw03::allocator<std::pair<int, int>, elementsCount>>;

  Map map;
  fillMap(map);
  std::cout << "std::map with custom allocator: " << std::endl << map << std::endl;
}

void testCustomContainer()
{
  hw03::forward_list<int> container;
  fillContainer(container);
  std::cout << "custom container: " << std::endl << container << std::endl;
}

void testCustomContainerWithCustomAllocator()
{
  using CustomAllocatorAndContainer = hw03::forward_list<int, hw03::allocator<int, elementsCount>>;

  CustomAllocatorAndContainer container;
  fillContainer(container);
  std::cout << "custom container with custom allocator: " << std::endl << container << std::endl;
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
