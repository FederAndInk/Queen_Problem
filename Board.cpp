#include "Board.hpp"

Board::Board(int size) : size(size), board(size, std::vector(size, FREE)) {}

int& Board::operator()(Position const& pos)
{
  return board.at(pos.y).at(pos.x);
}

int Board::operator()(Position const& pos) const
{
  return board.at(pos.y).at(pos.x);
}

void Board::add_queen(Position const& pos)
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

void Board::remove_queen(Position const& pos)
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

Position Board::remove_last_queen()
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

bool Board::is_free(Position const& pos) const
{
  return this->operator()(pos) == FREE;
}

bool Board::is_queen(Position const& pos) const
{
  return this->operator()(pos) == QUEEN;
}

void Board::clear()
{
  for (auto&& l : board)
  {
    l.assign(size, FREE);
  }
  nb_queen = 0;
  movements.clear();
}

void Board::add_view(Position const& pos)
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

void Board::add_views(Position const& pos, std::function<void(Position& p)> move)
{
  Position p = pos;
  move(p);
  for (; p.x < size && p.y < size && p.x >= 0 && p.y >= 0; move(p))
  {
    add_view(p);
  }
}

void Board::remove_view(Position const& pos)
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

void Board::remove_views(Position const& pos, std::function<void(Position& p)> move)
{
  Position p = pos;
  move(p);
  for (; p.x < size && p.y < size && p.x >= 0 && p.y >= 0; move(p))
  {
    remove_view(p);
  }
}

bool Board::move_queen(Position& p)
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

std::size_t Board::count_free_in(int row) const
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

bool Board::next(Position& p)
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

Position Board::bottomest_queen() const
{
  return bottomest_queen(size - 1);
}

Position Board::bottomest_queen(int y_from) const
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

bool Board::find_solution(bool step)
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

std::ostream& operator<<(std::ostream& out, Board const& b)
{
  std::cout << "\e[2J\e[H";

  std::cout << "  ";
  for (char no_row = 'A'; no_row < 'A' + b.get_size(); no_row++)
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
