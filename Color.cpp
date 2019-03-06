#include "Color.hpp"

Color::Color(std::string const& code) : code("\e[" + code + "m") {}

Color const Color::RESET{"0"};
Color const Color::RED{"31"};
Color const Color::GREEN{"32"};
Color const Color::BLUE{"34"};
Color const Color::PURPLE{"35"};
Color const Color::CYAN{"36"};

Color const Color::BOLD{"1"};
Color const Color::UNDERLINED{"4"};
Color const Color::ITALIC{"3"};
