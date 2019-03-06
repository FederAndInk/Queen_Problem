#include "Board.hpp"
#include <iostream>
#include <thread>

int main(int argc, char const* argv[])
{
  Board<8> b;
  std::string s;
  std::cout << b << "\n";
  std::getline(std::cin, s);
  b.add_queen({2, 5});
  std::cout << b << "\n";
  std::getline(std::cin, s);
  b.add_queen({0, 1});
  std::cout << b << "\n";

  // for (int i = 31; i < 38; i++)
  // {
  //   printf("%d: \e[%dm\u265b\e[0m\n", i, i);
  // }
  // for (int i = 91; i < 98; i++)
  // {
  //   printf("%d: \e[%dm\u265b\e[0m\n", i, i);
  // }

  return 0;
}
