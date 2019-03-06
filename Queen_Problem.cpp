#include "Board.hpp"
#include "Color.hpp"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

class Queen_Problem
{
private:
  static constexpr std::size_t size = 8;

  Board<size>       b;
  bool              exit = false;
  std::stringstream out;
  std::stringstream err;

public:
  Queen_Problem() {}

  void launch()
  {
    out << "type help to see commands\n";
    while (!exit)
    {
      std::cout << b << "\n";
      std::cout << b.get_nb_queen() << " queens placed\n\n";
      if (std::string s = out.str(); s != "")
      {
        std::cout << s << "\n";
        out.str("");
      }

      if (std::string s = err.str(); s != "")
      {
        std::cout << (s | Color::RED | Color::BOLD) << "\n";
        err.str("");
      }

      action();
    }
  }

private:
  void action()
  {
    std::string cmd;
    std::cout << "> ";
    std::getline(std::cin, cmd);
    std::vector<std::string> cmd_s;
    boost::split(cmd_s, cmd, [](char c) { return c == ' '; });

    if (cmd_s[0] == "exit" || cmd_s[0] == "e" || !std::cin)
    {
      exit = true;
    }
    else if (cmd_s[0] == "help" || cmd_s[0] == "h")
    {
      help();
    }
    else if (cmd_s[0] == "clear" || cmd_s[0] == "c")
    {
      b.clear();
    }
    else if (cmd_s[0] == "place" || cmd_s[0] == "p")
    {
      auto [valid, p] = check_coord(cmd, cmd_s);
      if (valid)
      {
        try
        {
          Position p{std::stoi(cmd_s[1]), cmd_s[2][0]};
          b.add_queen(p);
          if (b.is_win())
          {
            out << Color::GREEN << "You have placed " << size << " queens!\n"
                << Color::RESET;
          }
        }
        catch (std::runtime_error const& e)
        {
          err << e.what() << "\n";
        }
        catch (std::out_of_range const& e)
        {
          err << "(" << cmd_s[1] << ", " << cmd_s[2] << ") is not a valid position\n";
        }
      }
    }
    else if (cmd_s[0] == "remove" || cmd_s[0] == "r")
    {
      auto [valid, p] = check_coord(cmd, cmd_s);
      if (valid)
      {
        try
        {
          b.remove_queen(p);
        }
        catch (std::runtime_error const& e)
        {
          err << e.what() << "\n";
        }
        catch (std::out_of_range const& e)
        {
          err << "(" << cmd_s[1] << ", " << cmd_s[2] << ") is not a valid position\n";
        }
      }
    }
    else if (cmd_s[0] == "solve" || cmd_s[0] == "s")
    {
      out << "NIY\n";
    }
    else
    {
      err << "Command '" << cmd_s[0] << "' does not exist\n";
      err << "Type help to get help\n";
    }
  }

  std::pair<bool, Position> check_coord(std::string const&              cmd,
                                        std::vector<std::string> const& cmd_s)
  {
    if (cmd_s.size() == 3)
    {
      if (cmd_s[2].size() == 1)
      {
        try
        {
          return {true, Position{std::stoi(cmd_s[1]), cmd_s[2][0]}};
        }
        catch (std::invalid_argument const& e)
        {
          err << cmd;
          err << "Argument '" << cmd_s[1] << "' invalid\n";
          err << "Type help to get help\n";
        }
      }
      else
      {
        err << cmd;
        err << "Argument '" << cmd_s[2] << "' invalid\n";
        err << "Type help to get help\n";
      }
    }
    else
    {
      err << "Command '" << cmd << "' not complete\n";
      err << "Type help to get help\n";
    }
    return {false, Position{-1, -1}};
  }

  void help()
  {
    out << ("Commands:\n" | Color::BLUE | Color::UNDERLINED);
    out << "h -- help                 get help\n";
    out << "e -- exit                 exit the program\n";
    out << "p -- place <row> <col>    place a queen at [row, col]\n";
    out << "r -- remove <row> <col>   remove a queen from [row, col]\n";
    out << "s -- solve                find solutions\n";
    out << "c -- clear                remove all queens\n";
    out << "\n";
    out << ("info:\n" | Color::PURPLE | Color::UNDERLINED);
    out << "row: [1.." << size << "]\n";
    out << "col: [A.." << (char)('A' + size - 1) << "]\n";
  }
};

int main(int argc, char const* argv[])
{
  Queen_Problem qp;
  qp.launch();

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
