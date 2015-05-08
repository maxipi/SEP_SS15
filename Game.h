//------------------------------------------------------------------------------
// Game.h
//
// Group: Group 9, study assistant Philip Loibl
//
// Authors:
// Lorenz Leitner (1430211)
// Verena Niederwanger (1430778)
// Markus Pichler (1331070)
//------------------------------------------------------------------------------
//

#ifndef GAME_H
#define GAME_H

#include "Color.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>

class Tile;
class Position;

//------------------------------------------------------------------------------
// Game Class
// Class concerning the active game
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

    //--------------------------------------------------------------------------
    // counts the tiles
    //
    int tile_counter_;

    //--------------------------------------------------------------------------
    // userInputToCommand method
    // parses the user input as commands
    //
    // @param user_input input entered from the user
    //
    int userInputToCommand(std::vector<std::string> &user_input);

    //--------------------------------------------------------------------------
    // Private copy constructor
    //
    Game(const Game& original);

    //--------------------------------------------------------------------------
    // Private assignment operator
    //
    Game& operator=(const Game& original);

    std::string output_filename_;

  public:

    signed int max_x_;
    signed int max_y_;
    signed int min_x_;
    signed int min_y_;


    //--------------------------------------------------------------------------
    // setMaximas method
    // sets the maximum coordinates
    // @param reference position which is used as a reference to set coordinates
    //
    void setMaximas(Position reference);

    //--------------------------------------------------------------------------
    // Constructor
    //
    Game();

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Game();

    //--------------------------------------------------------------------------
    // map
    // Store tiles
    //
    std::map<Position*, Tile*> field;

    //--------------------------------------------------------------------------
    // run method
    // Starts the game
    // @param file_name file name to write file
    // @param graphic_mode used to test if grapic mode is enabled
    //
    void run();

    //--------------------------------------------------------------------------
    // toggle Player method
    // Switches the active player
    //
    void togglePlayer();

    //--------------------------------------------------------------------------
    // riseNumberOfTiles method
    // Increases the tile counter
    //
    void riseNumberOfTiles();

    //--------------------------------------------------------------------------
    // Setter Methods
    //

    //--------------------------------------------------------------------------
    // setStartTile method
    // Sets the first tile
    //
    // @param starttile the first tile
    //
    void setStartTile(Tile* starttile);

    //--------------------------------------------------------------------------
    // setRunning method
    // Toggles if the game is run
    //
    // @param running true if game shall be run, false if it should stop
    //
    void setRunning(bool running);

    //--------------------------------------------------------------------------
    // Getter Methods
    //

    //--------------------------------------------------------------------------
    // getActivePlayer method
    // Gets the current active player
    //
    // @return Color activeplayer_
    //
    Color getActivePlayer();

    //--------------------------------------------------------------------------
    // getNumberOfTiles method
    // Gets the current number of tiles
    //
    // @return int tile_counter
    //
    int getNumberOfTiles();

    //--------------------------------------------------------------------------
    // freeTiles method
    // deletes tiles
    //
    // @param field map of tiles
    //
    void freeTiles(std::map<Position*, Tile*> field);



    void printTiles();


    int moveID;
    std::string getOutputFilename();
    void setOutputFilename(std::string);
};
#endif // GAME_H