//------------------------------------------------------------------------------
// Filename: Write.h
// Description: Used for writing binary file
// Authors: Lorenz Leitner
// Tutor: Philipp Loibl
// Group: 11574
// Created: 14.04.2015
// Last change: 14.04.2015
//------------------------------------------------------------------------------

#ifndef WRITE_H_INCLUDED
#define WRITE_H_INCLUDED

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "Position.h"
#include "Game.h"
#include "Tile.h"
#include "Command.h"
#include "WriteException.h"

class Write :public Command
{
public:

  Write();
  //~Write();

  int execute(Game& board, std::vector<std::string>& params);
};

#endif //WRITE_H_INCLUDED

