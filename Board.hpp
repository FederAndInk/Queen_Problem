
#pragma once

#include "Color.hpp"
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
  inline Position(int x, int y) : x(x), y(y) {}

  /**
   * @brief Construct a new Position object from col row (letter[A..], number[1..])
   * 
   * @param col letter in A..A+size
   * @param row in 1..size
   */
  inline Position(int row, char col) : x(std::toupper(col) - 'A'), y(row - 1) {}

  inline std::string to_string() const
  {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }

  inline bool operator==(Position const& p) const
  {
    return x == p.x && y == p.y;
  }

  inline bool operator!=(Position const& p) const
  {
    return !(*this == p);
  }
};

class Board
{
private:
  std::vector<std::vector<int>> board;
  std::vector<Position>         movements;
  int                           nb_queen{0};
  int                           size;

public:
  static inline const int QUEEN = -1;
  static inline const int FREE = 0;

  explicit Board(int size);

  int& operator()(Position const& pos);
  int  operator()(Position const& pos) const;

  void     add_queen(Position const& pos);
  void     remove_queen(Position const& pos);
  Position remove_last_queen();

  bool is_free(Position const& pos) const;

  bool is_queen(Position const& pos) const;

  inline int get_nb_queen() const
  {
    return nb_queen;
  }

  inline bool is_win() const
  {
    return get_nb_queen() == size;
  }

  inline int get_size() const
  {
    return size;
  }

  void clear();

  void resize(int new_size);

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

  Position bottomest_queen() const;
  Position bottomest_queen(int y_from) const;

  friend std::ostream& operator<<(std::ostream& out, Board const& b);

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
