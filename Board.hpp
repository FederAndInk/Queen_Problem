
#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>

struct Position
{
  int x;
  int y;

  Position(int x, int y) : x(x), y(y) {}

  /**
   * @brief Construct a new Position object from col row (letter[A..], number[1..])
   * 
   * @param col letter in A..A+size
   * @param row in 1..size
   */
  Position(char col, int row) : x(col - 'A'), y(row - 1) {}

  std::string to_string() const
  {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }

  bool operator==(Position const& p) const
  {
    return x == p.x && y == p.y;
  }

  bool operator!=(Position const& p) const
  {
    return !(*this == p);
  }
};

template <std::size_t size>
class Board
{
private:
  std::array<std::array<int, size>, size> board;

public:
  static const int QUEEN = -1;
  static const int FREE = 0;

  Board() : board({0}) {}

  int& operator()(Position const& pos);
  int  operator()(Position const& pos) const;

  void add_queen(Position const& pos);

  bool is_free(Position const& pos) const;

private:
  void add_view(Position const& pos);
  void add_views(Position const& pos, std::function<void(Position& p)> move);

  template <std::size_t s>
  friend std::ostream& operator<<(std::ostream& out, Board<s> const& b);
};

template <std::size_t size>
int& Board<size>::operator()(Position const& pos)
{
  return board[pos.y][pos.x];
}

template <std::size_t size>
int Board<size>::operator()(Position const& pos) const
{
  return board[pos.y][pos.x];
}

template <std::size_t size>
void Board<size>::add_queen(Position const& pos)
{
  if (is_free(pos))
  {
    (*this)(pos) = QUEEN;
    add_views(pos, [](Position& p) { p.x++; });
    add_views(pos, [](Position& p) { p.y++; });
    add_views(pos, [](Position& p) { p.x--; });
    add_views(pos, [](Position& p) { p.y--; });
    add_views(pos, [](Position& p) {
      p.x++;
      p.y++;
    });
    add_views(pos, [](Position& p) {
      p.x--;
      p.y--;
    });
    add_views(pos, [](Position& p) {
      p.x++;
      p.y--;
    });
    add_views(pos, [](Position& p) {
      p.x--;
      p.y++;
    });
  }
  else
  {
    throw std::runtime_error("No free space at " + pos.to_string());
  }
}

template <std::size_t size>
bool Board<size>::is_free(Position const& pos) const
{
  return this->operator()(pos) == FREE;
}

template <std::size_t size>
void Board<size>::add_view(Position const& pos)
{
  if ((*this)(pos) != QUEEN)
  {
    (*this)(pos)++;
  }
  else
  {
    throw std::runtime_error("Queen seen at " + pos.to_string());
  }
}

template <std::size_t size>
void Board<size>::add_views(Position const& pos, std::function<void(Position& p)> move)
{
  Position p = pos;
  move(p);
  for (; p.x < size && p.y < size && p.x >= 0 && p.y >= 0; move(p))
  {
    add_view(p);
  }
}

template <std::size_t size>
std::ostream& operator<<(std::ostream& out, Board<size> const& b)
{
  std::cout << "\e[2J\e[H";

  char blocked_c[] = "\e[91m";
  char queen_c[] = "\e[95m";
  char reset_c[] = "\e[0m";

  std::cout << "  ";
  for (char no_row = 'A'; no_row < 'A' + size; no_row++)
  {
    std::cout << no_row << " ";
  }
  std::cout << "\n";

  size_t no_line = 1;
  for (auto&& l : b.board)
  {
    std::cout << no_line++ << " ";
    for (auto&& i : l)
    {
      if (i == -1)
      {
        out << queen_c << "♛" << reset_c;
      }
      else if (i > 0)
      {
        out << blocked_c << "☒" << reset_c;
      }
      else
      {
        out << "☐";
      }
      out << " ";
    }
    out << "\n";
  }

  return out;
}
