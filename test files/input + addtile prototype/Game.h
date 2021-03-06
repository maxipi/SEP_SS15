//------------------------------------------------------------------------------
// Game.h
//
// Group: Group 9, study assistant Philip Loibl
//
// Authors: Markus Pichler 1331070
// 
//------------------------------------------------------------------------------
//

#ifndef GAME_H
#define GAME_H

#include "Tile.h"

//------------------------------------------------------------------------------
// Game Class
//
class Game
{
  private:

    //--------------------------------------------------------------------------
    // Color of the active Player
    //
    Color activeplayer_;

    //--------------------------------------------------------------------------
    // Pointer of the Starttile
    //
    Tile* starttile_;

    //--------------------------------------------------------------------------
    // Bool is the game is running
    //
    bool running_;

  public:

    //--------------------------------------------------------------------------
    // Constructor
    //
    Game();

    //Game(Type side, Color orientation);

    //--------------------------------------------------------------------------
    // Copy Constructor
    // Makes a copy of another Game Object.
    // @param original Original to copy.
    //
    // Game(const Game &);

    //--------------------------------------------------------------------------
    // Run Methode
    // ...
    // @param 
    //
    void run();

    //--------------------------------------------------------------------------
    // toggle Player Methode
    // ...
    // @param 
    //
    void togglePlayer();

    //--------------------------------------------------------------------------
    // Setter Methods
    //
    void setStartTile(Tile* starttile);

    void setRunning(bool running);

    //--------------------------------------------------------------------------
    // Getter Methods
    //
    Color getActivePlayer();

    //--------------------------------------------------------------------------
    // Destructor
    //
    //~Game();
};
#endif // GAME_H