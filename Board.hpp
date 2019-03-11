
#pragma once

#include "Color.hpp"
#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct Position
{
  int x;
  int y;

  /**
   * @brief Construct a new Position object
   * 
   * @param x the column
   * @param y the row
   */
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
  std::vector<Position>                   movements;
  int                                     nb_queen{0};

public:
  static inline const int QUEEN = -1;
  static inline const int FREE = 0;

  Board() noexcept = default;

  int& operator()(Position const& pos);
  int  operator()(Position const& pos) const;

  void     add_queen(Position const& pos);
  void     remove_queen(Position const& pos);
  Position remove_last_queen();

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

  /**
   * @brief find a solution starting from current board
   * return false if no (more) solution can be found (we are at the end of possibilities)
   */
  bool find_solution(bool step = false);

private:
  void add_view(Position const& pos);
  void add_views(Position const& pos, std::function<void(Position& p)> move);

  void remove_view(Position const& pos);
  void remove_views(Position const& pos, std::function<void(Position& p)> move);

  std::size_t count_free_in(int row) const;
  /**
   * @brief move the queen (on p) in the next position possible on the same line
   * 
   * @param p 
   * @return true if the queen can and has been moved
   * @return false if there was no possibilities on this line
   */
  bool move_queen(Position& p);
  /**
   * @brief move the queens in the next position
   * 
   * @param p the position of the last queen (the down most queen)
   * @return true if a next position has been found
   * @return false otherwise
   */
  bool next(Position& p);

  Position bottomest_queen(int y_from = size - 1) const;

  template <std::size_t s>
  friend std::ostream& operator<<(std::ostream& out, Board<s> const& b);

  static inline const std::array<std::function<void(Position& p)>, 8> queen_movement = {
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
    movements.push_back(pos);
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
    movements.erase(std::remove(std::begin(movements), std::end(movements), pos),
                    std::end(movements));
    nb_queen--;
  }
  else
  {
    throw std::runtime_error("No queen at " + pos.to_string());
  }
}

template <std::size_t size>
Position Board<size>::remove_last_queen()
{
  if (!movements.empty())
  {
    Position p = movements.back();
    remove_queen(movements.back());
    return p;
  }
  else
  {
    throw std::runtime_error("No more queen to remove");
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
  movements.clear();
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

template <std::size_t size>
bool Board<size>::move_queen(Position& p)
{
  remove_queen(p);
  p.x++;
  while (p.x < size && !is_free(p))
  {
    p.x++;
  }

  if (p.x < size)
  {
    add_queen(p);
    return true;
  }
  else
  {
    return false;
  }
}

template <std::size_t size>
std::size_t Board<size>::count_free_in(int row) const
{
  std::size_t nb = 0;

  for (Position p{0, row}; p.x < size; p.x++)
  {
    if (is_free(p))
    {
      nb++;
    }
  }
  return nb;
}

template <std::size_t size>
bool Board<size>::next(Position& p)
{
  if (get_nb_queen() == 0)
  {
    p = {0, 0};
    add_queen(p);
    return true;
  }
  else
  {
    Position p_tmp{0, p.y + 1};
    while (p_tmp.x < size && p_tmp.y < size && !is_free(p_tmp))
    {
      p_tmp.x++;
    }

    if (p_tmp.x < size && p_tmp.y < size)
    {
      p = p_tmp;
      add_queen(p);
    }
    else
    {
      while (get_nb_queen() > 0 && !move_queen(p))
      {
        p = bottomest_queen(--p.y);
      }
    }

    return get_nb_queen() > 0;
  }
}

template <std::size_t size>
Position Board<size>::bottomest_queen(int y_from) const
{
  Position p{0, y_from};

  // NOTE: can be optimized with movements vector instead of walking through all the board
  while (p.y >= 0 && !is_queen(p))
  {
    while (p.x < size && !is_queen(p))
    {
      p.x++;
    }
    if (p.x >= size)
    {
      p.x = 0;
      p.y--;
    }
  }
  if (p.y < 0)
  {
    p = {-1, -1};
  }
  return p;
}

template <std::size_t size>
bool Board<size>::find_solution(bool step)
{
  Position p = bottomest_queen();

  if (step)
  {
    return next(p);
  }
  else
  {
    if (is_win())
    {
      next(p);
    }

    while (!is_win() && next(p))
    {
    }
    return is_win();
  }
}
