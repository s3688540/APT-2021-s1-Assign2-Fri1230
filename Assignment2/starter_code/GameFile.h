#ifndef ASSIGN2_GAMEFILE_H
#define ASSIGN2_GAMEFILE_H

#include "LinkedList.h"
#include "Player.h"
#include <string>
#include<vector>
#include<fstream>
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
   vector<string> charSplit(const string& str,const char* sep);
   int saveGame(string fileName); // save the game info,if save succeed retrun 1,else return 0
   bool checkIsNum(string str); // if str is number then return true, otherwise, false
   bool colourIsValid(char c); //if c inside ('R','0','Y','G','B','P') then return true, otherwise, false
   bool ShapeIsValid(char s); //if s inside (1,2,3,4,5,6) then return true, otherwise, false
   bool rowIsValid(char r); //if r inside (A-Z) then return true, otherwise, false
   bool boardIsValid(string colstr); //if colstr smaller then maximum size then return true, otherwise, false
   
   // -1:file does not exist
   //  1:format incorrect, unavailble to load
   //  0:successfully load
   int loadGame(string fileName); // load the game info,if load succeed return GameFile,else return NULL


};

#endif // ASSIGN2_GAMEFILE_H