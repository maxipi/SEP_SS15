//------------------------------------------------------------------------------
// AddTile.cpp
//
// Group: Group 9, study assistant Philip Loibl
//
// Authors:
// Lorenz Leitner (1430211)
// Verena Niederwanger (1430778)
// Markus Pichler (1331070)
//------------------------------------------------------------------------------
//

#include "AddTile.h"

using std::string;

int AddTile::id_counter_ = 0;

//------------------------------------------------------------------------------
// Constructor
//
AddTile::AddTile() : Command("Addtile")
{
}

//------------------------------------------------------------------------------
AddTile::~AddTile()
{
}

//------------------------------------------------------------------------------
bool AddTile::valideInput(std::vector<string> user_input, Tile &current_tile,
  Position &current_position)
{
  try
  {
    if (user_input.size() != 3)
    {
      throw WrongParameterException();
    }
    if (current_position.parse(user_input[1]))
    {
      try
      {
        if(user_input[2].size() != 1)
        {
          throw InvalidParameterException();
        }
        current_tile.setType(user_input[2][0]);
        if(current_tile.getType() == Tile::EMPTY_T)
        {
          throw InvalidParameterException();
        }
      }
      catch (...)
      {
        throw InvalidParameterException();
      }
    }
    else
    {
      throw InvalidParameterException();
    }
    return true;
  }
  catch (InvalidParameterException& e1)
  {
    std::cout << e1.what() << std::endl;
    return false;
  }
  catch (WrongParameterException& e1)
  {
    std::cout << e1.what() << std::endl;
    return false;
  }
}

//------------------------------------------------------------------------------
int AddTile::execute(Game& board, std::vector<string>& user_input)
{
  try
  {
    Tile empty_tile(Tile::EMPTY_T,EMPTY_C);
    // number of tiles beside the setted tile
    // for auto complete
    if (board.getNumberOfTiles() == 64)
    {
      throw NotEnoughTilesException();
    }

    Position current_position;
    Tile current_tile(Tile::EMPTY_T, COLOR_RED);

    current_tile.setPlayer(board.getActivePlayer());
    // look up if the userinput is correct
    if (!(valideInput(user_input, current_tile, current_position)))
    {
      return 1;
    }

    current_tile.setWhiteId(id_counter_++);
    current_tile.setRedId(id_counter_++);

    Position center(0, 0);
    if ((board.getNumberOfTiles() == 0)
      && (current_tile.getColor() != COLOR_RED || current_position != center))
    {
      throw InvalidCoordinatesException();
    }

    for (auto& var : board.field)
    {
      if (*var.first == current_position && *var.second != empty_tile)
      {
        throw NotEmptyFieldException();
      }
    }

    // change topcolor, check if contact to other tile,
    // and check if alle connected colors match
    if(board.getNumberOfTiles() > 0 &&
       adaptTile(board.field, current_tile, current_position))
    {
      // fail
      return 2;
    }

    int win_code;
    // win by loop
    win_code = winByLoop(board.field, current_tile, current_position);


    // replace empty tile with tile
    bool replace = true;
    for (auto& var : board.field)
    {
      if (*var.first == current_position && *var.second == empty_tile)
      {
        delete var.second;
        board.field[var.first] = new Tile(current_tile);
        board.riseNumberOfTiles();
        replace = false;
      }
    }

    // set tile
    if (replace)
    {
      board.field.emplace(new Position(current_position), new Tile(current_tile));
      board.riseNumberOfTiles();

      // set size of field
      board.setMaximas(current_position);

      // insert empty tiles in field between maximas
      if (board.getNumberOfTiles() > 2)
      {
        fillEmptyTiles(board);
      }
    }

    // nobody won by loop
    if(!win_code)
    {
      // win by length
      win_code = winByLength(board, current_tile);
    }

    if(win_code)
    {
      return whoWon(win_code);
    }

    // if nobody won and tiles are not available
    if (board.getNumberOfTiles() == 64)
    {
      std::cout << "No more tiles left. Game ends in a draw!" << std::endl;
      return 4;
    }

  }
  catch (OutOfMemoryException& e1)
  {
    std::cout << e1.what() << std::endl;
  }
  catch (NotEnoughTilesException& e1)
  {
    std::cout << e1.what() << std::endl;
    return 3;
  }
  catch (InvalidCoordinatesException& e1)
  {
    std::cout << e1.what() << std::endl;
    return 2;
  }
  catch (NotEmptyFieldException& e1)
  {
    std::cout << e1.what() << std::endl;
    return 2;
  }
  return 0;
}

//------------------------------------------------------------------------------
bool AddTile::colorCheck(bool color_bool, bool &twisted, bool &lonely_tile,
                      Tile &current_tile)/*, Color c1,Tile& t2,
                      std::vector<int> &red_id_to_merge,
                      std::vector<int> &white_id_to_merge)*/
{
  try
  {
    lonely_tile = false;
    if (color_bool)
    {
      if (twisted)
      {
        throw ConnectedColorsMismatchException();
      }
      current_tile.setColor(current_tile.notTopColor());
/*
      if(c1 == COLOR_WHITE)
      {
        current_tile.setRedId(t2.getRedId());
        red_id_to_merge.push_back(current_tile.getRedId());
      }
      if(c1 == COLOR_RED)
      {
        current_tile.setWhiteId(t2.getWhiteId());
        white_id_to_merge.push_back(current_tile.getWhiteId());
      }
*/
      twisted = true;
    }
    else
    {
/*
      if(c1 == COLOR_RED)
      {
        current_tile.setRedId(t2.getRedId());
        red_id_to_merge.push_back(current_tile.getRedId());
      }
      if(c1 == COLOR_WHITE)
      {
        current_tile.setWhiteId(t2.getWhiteId());
        white_id_to_merge.push_back(current_tile.getWhiteId());
      }
*/
      twisted = true;
    }
    return true;
  }
  catch (ConnectedColorsMismatchException& e1)
  {
    std::cout << e1.what() << std::endl;
    return false;
  }
}

//------------------------------------------------------------------------------
int AddTile::adaptTile(std::map<Position*, Tile*> field,
                        Tile &current_tile, Position current_position)
{
  try
  {
    Tile empty_tile(Tile::EMPTY_T,EMPTY_C);
    // true wenn das tile angepasst wurde bzw schon richtig gelegen ist
    bool twisted = false;
    // falls angrenzend ein stein liegt
    bool lonely_tile = true;
    // positionen um den gelegten stein
    Position left(current_position.getX() - 1, current_position.getY());
    Position right(current_position.getX() + 1, current_position.getY());
    Position top(current_position.getX(), current_position.getY() - 1);
    Position bottom(current_position.getX(), current_position.getY() + 1);
/*
    std::vector<int> red_id_to_merge;
    std::vector<int> white_id_to_merge;
*/
    for (auto& var : field)
    {
      if (*var.second == empty_tile)
      {
        continue;
      }
      if(*(var.first) == left &&
         !colorCheck(var.second->getColorRight() != current_tile.getColorLeft(),
                  twisted, lonely_tile, current_tile))/*, current_tile.getColorLeft(),*var.second,
                  red_id_to_merge, white_id_to_merge))*/
      {
        return 1;
      }
      else if(*(var.first) == right &&
         !colorCheck(var.second->getColorLeft() != current_tile.getColorRight(),
                  twisted, lonely_tile, current_tile))/*,current_tile.getColorRight(),*var.second,
                  red_id_to_merge, white_id_to_merge))*/
      {
        return 1;
      }
      else if(*(var.first) == top &&
         !colorCheck(var.second->getColorButtom() != current_tile.getColorTop(),
                  twisted, lonely_tile, current_tile))/*,current_tile.getColorTop(),*var.second,
                  red_id_to_merge, white_id_to_merge))*/
      {
        return 1;
      }
      else if(*(var.first) == bottom &&
         !colorCheck(var.second->getColorTop() != current_tile.getColorButtom(),
                  twisted, lonely_tile, current_tile))/*,current_tile.getColorButtom(),*var.second,
                  red_id_to_merge, white_id_to_merge))*/
      {
        return 1;
      }
      else
      {
        continue;
      }
    }
/*
    if(red_id_to_merge.size() >= 2)
    {
      for(auto& var: field)
      {
        if(var.second->getRedId() == red_id_to_merge[1])
        {
          var.second->setRedId(red_id_to_merge[0]);
        }
      }
      if(current_tile.getRedId() == red_id_to_merge[1])
      {
        current_tile.setRedId(red_id_to_merge[0]);
      }
      if(red_id_to_merge[1] == red_id_to_merge[0])
      {
        return 9;
        //win
      }
    }
    if(white_id_to_merge.size() >= 2)
    {
      for(auto& var: field)
      {
        if(var.second->getWhiteId() == white_id_to_merge[1])
        {
          var.second->setWhiteId(white_id_to_merge[0]);
        }
      }
      if(current_tile.getWhiteId() == white_id_to_merge[1])
      {
        current_tile.setWhiteId(white_id_to_merge[0]);
      }
      if(white_id_to_merge[0] == white_id_to_merge[1])
      {
        return 8;
        //win
      }
    }
*/
    if (lonely_tile)
    {
      throw NotConnectedFieldException();
    }
    return 0;
  }
  catch (NotConnectedFieldException& e1)
  {
    std::cout << e1.what() << std::endl;
    return 1;
  }
  return 0;
}

//------------------------------------------------------------------------------
bool AddTile::completeMap(std::map<Position*, Tile*> &field,
  std::vector<string> &forAddtile)
{
  Tile empty_tile(Tile::EMPTY_T,EMPTY_C);
  for(auto& var1: field)
  {
    if(*var1.second != empty_tile)
    {
      continue;
    }
    int found_tile = 0;
    Color Li = EMPTY_C;
    Color Re = EMPTY_C;
    Color Ob = EMPTY_C;
    Color Un = EMPTY_C;

    Position current_position = *(var1.first);
    Position left(current_position.getX() - 1,current_position.getY());
    Position right(current_position.getX()+1,current_position.getY());
    Position top(current_position.getX(),current_position.getY()-1);
    Position bottom(current_position.getX(),current_position.getY()+1);
    for(auto& var2: field)
    {
      if(*var2.second == empty_tile)
      {
        continue;
      }
      if(*(var2.first) == left)
      {
        found_tile++;
        Li = var2.second->getColorRight();
      }
      else if(*(var2.first) == right)
      {
        found_tile++;
        Re = var2.second->getColorLeft();
      }
      else if(*(var2.first) == top)
      {
        found_tile++;
        Ob = var2.second->getColorButtom();
      }
      else if(*(var2.first) == bottom)
      {
        found_tile++;
        Un = var2.second->getColorTop();
      }
      else
      {
        continue;
      }
    }
    if(found_tile >= 2)
    {
      Tile expanded_tile(Li, Re, Ob, Un);
      if(expanded_tile == empty_tile)
      {
        continue;
      }
      forAddtile[1] = var1.first->toString();
      forAddtile[2] = expanded_tile.getTypeOut();
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void AddTile::fillEmptyTiles(Game& board)
{
  try
  {
    Tile empty_tile(Tile::EMPTY_T,EMPTY_C);
    bool wenn;
    for (signed int x = board.min_x_; x <= board.max_x_; x++)
    {
      for (signed int y = board.min_y_; y <= board.max_y_; y++)
      {
        wenn = true;
        Position empty_position(x, y);
        for (auto& var : board.field)
        {
          if (*var.first == empty_position)
          {
            wenn = false;
            break;
          }
        }
        if (wenn)
        {
          board.field.emplace(new Position(empty_position), new Tile(empty_tile));
        }
      }
    }
  }
  catch (OutOfMemoryException& e1)
  {
    std::cout << e1.what() << std::endl;
  }

}

//------------------------------------------------------------------------------
int AddTile::winByLength(Game& board, Tile current_tile)
{
  if(checkLineLength(board,current_tile,"white"))
  {
    return 8;
  }
  if(checkLineLength(board,current_tile,"red"))
  {
    return 9;
  }
  return 0;
}

//------------------------------------------------------------------------------
int AddTile::checkLineLength(Game& board, Tile current_tile, string color)
{
  // if a line goes over 8 lines or columns
  signed int min_x = INT_MAX;
  signed int min_y = INT_MAX;
  signed int max_x = INT_MIN;
  signed int max_y = INT_MIN;
  for(auto& var: board.field)
  {
    if(var.second->getId(color) == current_tile.getId(color))
    {
      if(var.first->getX() > max_x)
      {
        max_x = var.first->getX();
      }
      if(var.first->getY() > max_y)
      {
        max_y = var.first->getY();
      }
      if(var.first->getX() < min_x)
      {
        min_x = var.first->getX();
      }
      if(var.first->getY() < min_y)
      {
        min_y = var.first->getY();
      }
    }
  }
  if(max_x - min_x >= 7 || max_y - min_y >= 7)
  {
    return 1;
  }
  return 0;
}

//------------------------------------------------------------------------------
void AddTile::idCheck(Tile &current_tile, Color color_to_match,Tile& tile_beside,
                      std::vector<int> &red_id_to_merge,
                      std::vector<int> &white_id_to_merge)
{
  if(color_to_match == COLOR_RED)
  {
    current_tile.setRedId(tile_beside.getRedId());
    red_id_to_merge.push_back(current_tile.getRedId());
  }
  if(color_to_match == COLOR_WHITE)
  {
    current_tile.setWhiteId(tile_beside.getWhiteId());
    white_id_to_merge.push_back(current_tile.getWhiteId());
  }
}

//------------------------------------------------------------------------------
int AddTile::winByLoop(std::map<Position*, Tile*> field,
                     Tile &current_tile, Position current_position)
{
  Tile empty_tile(Tile::EMPTY_T,EMPTY_C);

  Position left(current_position.getX() - 1, current_position.getY());
  Position right(current_position.getX() + 1, current_position.getY());
  Position top(current_position.getX(), current_position.getY() - 1);
  Position bottom(current_position.getX(), current_position.getY() + 1);

  std::vector<int> red_id_to_merge;
  std::vector<int> white_id_to_merge;

  for (auto& var : field)
  {
    if (*var.second == empty_tile)
    {
      continue;
    }
    if(*(var.first) == left)
    {
       idCheck(current_tile, current_tile.getColorLeft(),*var.second,
                red_id_to_merge, white_id_to_merge);
    }

    else if(*(var.first) == right)
    {
       idCheck(current_tile,current_tile.getColorRight(),*var.second,
                red_id_to_merge, white_id_to_merge);
    }
    else if(*(var.first) == top)
    {
       idCheck(current_tile,current_tile.getColorTop(),*var.second,
                red_id_to_merge, white_id_to_merge);
    }
    else if(*(var.first) == bottom)
    {
       idCheck(current_tile,current_tile.getColorButtom(),*var.second,
                red_id_to_merge, white_id_to_merge);
    }
    else
    {
      continue;
    }
  }

  if(red_id_to_merge.size() >= 2)
  {
    for(auto& var: field)
    {
      if(var.second->getRedId() == red_id_to_merge[1])
      {
        var.second->setRedId(red_id_to_merge[0]);
      }
    }
    if(current_tile.getRedId() == red_id_to_merge[1])
    {
      current_tile.setRedId(red_id_to_merge[0]);
    }
    if(red_id_to_merge[1] == red_id_to_merge[0])
    {
      return 9;
      //win
    }
  }
  if(white_id_to_merge.size() >= 2)
  {
    for(auto& var: field)
    {
      if(var.second->getWhiteId() == white_id_to_merge[1])
      {
        var.second->setWhiteId(white_id_to_merge[0]);
      }
    }
    if(current_tile.getWhiteId() == white_id_to_merge[1])
    {
      current_tile.setWhiteId(white_id_to_merge[0]);
    }
    if(white_id_to_merge[0] == white_id_to_merge[1])
    {
      return 8;
      //win
    }
  }
  return 0;
}

//------------------------------------------------------------------------------
int AddTile::whoWon(int win_code)
{
  if(win_code == 8)
  {
    std::cout << "Player white wins!" << std::endl;
    return 5;
  }
  if(win_code == 9)
  {
    std::cout << "Player red wins!" << std::endl;
    return 5;
  }
  return 0;
}

