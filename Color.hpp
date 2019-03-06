#pragma once
#include <string>

class Color
{
  std::string code;

  Color(std::string const& code);

public:
  friend std::string   operator|(std::string const& str, Color const& c);
  friend std::ostream& operator<<(std::ostream& out, Color const& c);

  static Color const RESET;
  static Color const RED;
  static Color const GREEN;
  static Color const BLUE;
  static Color const PURPLE;
  static Color const CYAN;

  static Color const BOLD;
  static Color const UNDERLINED;
  static Color const ITALIC;
};

inline std::string operator|(std::string const& str, Color const& c)
{
  return c.code + str + c.RESET.code;
}

inline std::ostream& operator<<(std::ostream& out, Color const& c)
{
  return out << c.code;
}
