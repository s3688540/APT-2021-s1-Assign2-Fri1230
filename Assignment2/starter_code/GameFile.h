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
   bool checkIsNum(string str); //str是数字返回true,否则返回false
   bool colourIsValid(char c); //c是规定的颜色('R','0','Y','G','B','P')之一返回true,否则返回false
   bool ShapeIsValid(char s); //s是规定的形状之一(1,2,3,4,5,6)之一返回true,否则返回false
   bool rowIsValid(char r); //r在(A-Z)范围内则返回true,否则返回false
   bool boardIsValid(string colstr); //colstr小于最大列尺寸则返回true,否则返回false
   
   // -1:代表文件并不存在
   //  1:代表文件存储的数据格式错误,无法加载游戏数据
   //  0:加载成功
   int loadGame(string fileName); // load the game info,if load succeed return GameFile,else return NULL


};

#endif // ASSIGN2_GAMEFILE_H