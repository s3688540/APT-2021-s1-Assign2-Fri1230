#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include "LinkedList.h"
#include <string>

class Player {
public:

   Player();
   ~Player();
   std::string name;
   int score; 
   LinkedList hand;
   bool turn;

};

#endif // ASSIGN2_PLAYER_H