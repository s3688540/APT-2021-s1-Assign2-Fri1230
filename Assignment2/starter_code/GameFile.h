#ifndef ASSIGN2_GAMEFILE_H
#define ASSIGN2_GAMEFILE_H

#include "LinkedList.h"
#include "Player.h"
#include <string>
#include<vector>
using namespace std;

class GameFile {
public:

   GameFile();
   ~GameFile();
   Player player[2]; // store the info of player1 and player2
   int boardHeight; // store the height of boardState
   int boardWidth; // // store the width of boardState
   vector<vector<string> > boardState; // store the board state of game
   LinkedList bagLinkedList; // store the bag of tiles
   string currentPlayer; // store the turn of player

   int saveGame(string fileName); // save the game info,if save succeed retrun 1,else return 0
   GameFile loadGame(string fileName); // load the game info,if load succeed return GameFile,else return NULL



};

#endif // ASSIGN2_GAMEFILE_H