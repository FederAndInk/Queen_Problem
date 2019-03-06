
#pragma once

#include "Color.hpp"
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
  Position(int row, char col) : x(std::toupper(col) - 'A'), y(row - 1) {}

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
  std::array<std::array<int, size>, size> board{{0}};
  int                                     nb_queen{0};

public:
  static inline const int QUEEN = -1;
  static inline const int FREE = 0;

  int& operator()(Position const& pos);
  int  operator()(Position const& pos) const;

  void add_queen(Position const& pos);
  void remove_queen(Position const& pos);

  bool is_free(Position const& pos) const;

  bool is_queen(Position const& pos) const;

  int get_nb_queen() const
  {
    return nb_queen;
  }

  bool is_win() const
  {
    return get_nb_queen() == size;
  }

  void clear();

private:
  void add_view(Position const& pos);
  void add_views(Position const& pos, std::function<void(Position& p)> move);

  void remove_view(Position const& pos);
  void remove_views(Position const& pos, std::function<void(Position& p)> move);

  template <std::size_t s>
  friend std::ostream& operator<<(std::ostream& out, Board<s> const& b);

  static inline const std::vector<std::function<void(Position& p)>> queen_movement = {
      [](Position& p) { p.x++; },
      [](Position& p) { p.y++; },
      [](Position& p) { p.x--; },
      [](Position& p) { p.y--; },
      [](Position& p) {
        p.x++;
        p.y++;
      },
      [](Position& p) {
        p.x--;
        p.y--;
      },
      [](Position& p) {
        p.x++;
        p.y--;
      },
      [](Position& p) {
        p.x--;
        p.y++;
      }};
};

template <std::size_t size>
int& Board<size>::operator()(Position const& pos)
{
  return board.at(pos.y).at(pos.x);
}

template <std::size_t size>
int Board<size>::operator()(Position const& pos) const
{
  return board.at(pos.y).at(pos.x);
}

template <std::size_t size>
void Board<size>::add_queen(Position const& pos)
{
  if (is_free(pos))
  {
    (*this)(pos) = QUEEN;
    for (auto&& fn : queen_movement)
    {
      add_views(pos, fn);
    }
    nb_queen++;
  }
  else
  {
    throw std::runtime_error("No free space at " + pos.to_string());
  }
}

template <std::size_t size>
void Board<size>::remove_queen(Position const& pos)
{
  if (is_queen(pos))
  {
    (*this)(pos) = FREE;
    for (auto&& fn : queen_movement)
    {
      remove_views(pos, fn);
    }
    nb_queen--;
  }
  else
  {
    throw std::runtime_error("No queen at " + pos.to_string());
  }
}

template <std::size_t size>
bool Board<size>::is_free(Position const& pos) const
{
  return this->operator()(pos) == FREE;
}

template <std::size_t size>
bool Board<size>::is_queen(Position const& pos) const
{
  return this->operator()(pos) == QUEEN;
}

template <std::size_t size>
void Board<size>::clear()
{
  for (auto&& l : board)
  {
    l.fill(FREE);
  }
  nb_queen = 0;
}

template <std::size_t size>
void Board<size>::add_view(Position const& pos)
{
  if (!is_queen(pos))
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
void Board<size>::remove_view(Position const& pos)
{
  if (!is_free(pos) && !is_queen(pos))
  {
    (*this)(pos)--;
  }
  else
  {
    throw std::runtime_error("can't remove view at " + pos.to_string());
  }
}

template <std::size_t size>
void Board<size>::remove_views(Position const& pos, std::function<void(Position& p)> move)
{
  Position p = pos;
  move(p);
  for (; p.x < size && p.y < size && p.x >= 0 && p.y >= 0; move(p))
  {
    remove_view(p);
  }
}

template <std::size_t size>
std::ostream& operator<<(std::ostream& out, Board<size> const& b)
{
  std::cout << "\e[2J\e[H";

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
        out << ("♛" | Color::PURPLE | Color::BOLD);
      }
      else if (i > 0)
      {
        out << ("☒" | Color::RED | Color::BOLD);
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
