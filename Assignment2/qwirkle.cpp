#include "LinkedList.h"
#include "Player.h"
#include"GameFile.h"
#include <cstring>
#include<time.h>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <ctime>
using namespace std;

#define EXIT_SUCCESS 0

LinkedList initBag();
LinkedList initHand(LinkedList &linkedList, int obj[]);

void mainMenu();
string readEnvStdin();
vector<string> charSplit(const string &str);

int checkReplaceFormat(vector<string> result);
int checkReplaceLegal(vector<string> result, LinkedList linkedList);
void replaceHand(vector<string> result, LinkedList &linkedList, LinkedList &hand, int obj[]);

int checkPlaceFormat(vector<string> result);
int checkPlaceLegal(vector<string> result, LinkedList linkedList);
int palceAtVector(string tileStr, string placeStr, vector<vector<string> > &obj, Player &player);
void printVector(vector<vector<string> > obj);

int makeScore(int row, int col, vector<vector<string> > obj);
void creditMenu();

//int main(void)
//{
//   Player player1;
//   Player player2;
//   LinkedList linkedList;
//   bool checkFlag;
//   bool turnFlag;
//   int obj[72];
//   char stdin1[100];
//   char stdin2[100];
//   vector<string> result;
//   for (int i = 0; i < 72; i++)
//   {
//      obj[i] = 0;
//   }
//
//   int MAX_SIZE = 26;
//   vector<vector<string> > boardVector(MAX_SIZE, vector<string>(MAX_SIZE));
//   for (int i = 0; i < boardVector.size(); i++)
//   {
//      for (int j = 0; j < boardVector[i].size(); j++)
//      {
//         boardVector[i][j] = "  ";
//      }
//   }
//
//   player1.name = 'A';
//   player2.name = 'B';
//   player1.score = 0;
//   player2.score = 0;
//   do
//   {
//      cout << "Input the Number(1~4) to select operation: \n";
//      cin.getline(stdin1, 100);
//      switch (stdin1[0])
//      {
//      case '1':
//         cout << "1 step" << std::endl;
//         break;
//      case '2':
//         linkedList = initBag();
//         player1.hand = initHand(linkedList, obj);
//         player2.hand = initHand(linkedList, obj);
//         turnFlag = true;
//
//         do
//         {
//
//            if (turnFlag)
//            {
//               cout << endl;
//               cout << player1.name << ",it's your turn" << endl;
//               cout << "Score for " << player1.name << ":" << player1.score << endl;
//               cout << "Score for " << player2.name << ":" << player2.score << endl;
//               printVector(boardVector);
//               player1.hand.printHand();
//               do
//               {
//                  cout << ">";
//                  cin.getline(stdin2, 100);
//                  if (strncmp(stdin2, "EOF", 3) == 0)
//                  {
//                     break;
//                  }
//                  result = charSplit(stdin2);
//
//                  checkFlag = checkReplaceLegal(result, player1.hand) == 5 || checkPlaceLegal(result, player1.hand) == 8;
//                  if (checkReplaceLegal(result, player1.hand) == 5)
//                  {
//                     //
//                     replaceHand(result, linkedList, player1.hand, obj);
//                  }
//                  else if (checkPlaceLegal(result, player1.hand) == 8)
//                  {
//                     if (palceAtVector(result.at(1), result.at(3), boardVector, player1) == 1)
//                     {
//                        player1.hand.cut(result.at(1)[0], atoi(&result.at(1)[1]));
//                     }
//                     else
//                     {
//                        cout << "inlegal place position input!" << endl;
//                        checkFlag = false;
//                     }
//                  }
//                  else
//                  {
//                     cout << "inlegal input!" << endl;
//                  }
//               } while (!checkFlag);
//               turnFlag = false;
//            }
//            else
//            {
//               cout << endl;
//               cout << player2.name << ",it's your turn" << endl;
//               cout << "Score for " << player1.name << ":" << player1.score << endl;
//               cout << "Score for " << player2.name << ":" << player2.score << endl;
//               printVector(boardVector);
//               player2.hand.printHand();
//               do
//               {
//                  cout << ">";
//                  cin.getline(stdin2, 100);
//                  if (strncmp(stdin2, "EOF", 3) == 0)
//                  {
//                     break;
//                  }
//                  result = charSplit(stdin2);
//
//                  checkFlag = checkReplaceLegal(result, player2.hand) == 5 || checkPlaceLegal(result, player2.hand) == 8;
//                  if (checkReplaceLegal(result, player2.hand) == 5)
//                  {
//                     //
//                     replaceHand(result, linkedList, player2.hand, obj);
//            
//                  }
//                  else if (checkPlaceLegal(result, player2.hand) == 8)
//                  {
//                     if (palceAtVector(result.at(1), result.at(3), boardVector, player2) == 1)
//                     {
//                        player2.hand.cut(result.at(1)[0], atoi(&result.at(1)[1]));
//      
//                     }
//                     else
//                     {
//                        cout << "inlegal place position input!" << endl;
//                        checkFlag = false;
//                     }
//                  }
//                  else
//                  {
//                     cout << "inlegal input!" << endl;
//                  }
//               } while (!checkFlag);
//               turnFlag = true;
//            }
//         } while (strncmp(stdin2, "EOF", 3) != 0);
//
//         break;
//      case '3':
//         std::cout << "3 step" << std::endl;
//         break;
//      case '4':
//         std::cout << "Good bye" << std::endl;
//         break;
//      default:
//         std::cout << "Input error" << std::endl;
//      }
//   } while (stdin1[0] != '4');
//
//   return EXIT_SUCCESS;
//}

void LoadGameTest()
{
    GameFile gamefile;
    GameFile loadGameFile = gamefile.loadGame("data.txt");
    loadGameFile.saveGame("data2.txt");
}
void SaveGameTest()
{
    GameFile gamefile;

    //data for player 1
    gamefile.player[0].name = "A";
    gamefile.player[0].score = 8;
    gamefile.player[0].turn = true;
    gamefile.player[0].hand.create();
    Node* player_head = gamefile.player[0].hand.getHead();
    for (int i = 0; i < 10; i++)
    {
        Tile* tile = new Tile();
        tile->colour = RED;
        tile->shape = CIRCLE;
        Node* node = new Node(tile, NULL);
        gamefile.player[0].hand.insert(node);
    }

    //data for player 2
    gamefile.player[1].name = "B";
    gamefile.player[1].score = 6;
    gamefile.player[1].turn = false;
    gamefile.player[1].hand.create();
    player_head = gamefile.player[1].hand.getHead();
    for (int i = 0; i < 5; i++)
    {
        Tile* tile = new Tile();
        tile->colour = ORANGE;
        tile->shape = SQUARE;
        Node* node = new Node(tile, NULL);
        gamefile.player[1].hand.insert(node);
    }

    //board size
    gamefile.boardHeight = 26;
    gamefile.boardWidth = 26;
    
    //board
    gamefile.boardState = vector<vector<string>>(26, vector<string>(26, "  "));
    srand(time(NULL));
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            int rnum = rand() % 100 + 1;

            if (rnum % 2 == 0&&rnum%5==0)
            {
                gamefile.boardState[i][j] = "C4";
            }
        }
    }

    //Bag data
    gamefile.bagLinkedList.create();
    Node* bag_head = gamefile.bagLinkedList.getHead();
    for (int i = 0; i < 7; i++)
    {
        Tile* tile = new Tile();
        tile->colour = GREEN;
        tile->shape = DIAMOND;
        Node* node = new Node(tile, NULL);
        gamefile.bagLinkedList.insert(node);
    }


    //current player data
    gamefile.currentPlayer = "A";

    gamefile.saveGame("data.txt");

}

int main(void)
{
    creditMenu();
    SaveGameTest();
    
    //LoadGameTest();
}

void creditMenu()
{
    cout << "----------------------------------" << endl;
    cout << "Name:" << "Junyu Li" << endl;
    cout << "Student ID:" << "s3706335" << endl;
    cout << "Email:" <<"s3706335@student.rmit.edu.au"<< endl;
    cout << endl;

    cout << "Name:" << "Zongzhou Wang" << endl;
    cout << "Student ID:" << "s3688540" << endl;
    cout << "Email:" << "s3688540@student.rmit.edu.au" << endl;
    cout << endl;

    cout << "Name:" << "Henghao Li" << endl;
    cout << "Student ID:" << "s3798993" << endl;
    cout << "Email:" << "s3798993@student.rmit.edu.au" << endl;
    cout << "----------------------------------" << endl;
}
LinkedList initBag()
{
   LinkedList linkedList;
   Colour colourArr[] = "ROYGBP";
   Shape shapeArr[] = {1, 2, 3, 4, 5, 6};
   linkedList.create();
   for (int i = 0; i < 6; i++)
   {
      for (int j = 0; j < 6; j++)
      {
         Tile *tile1 = new Tile();
         Tile *tile2 = new Tile();

         tile1->colour = colourArr[i];
         tile1->shape = shapeArr[j];
         tile2->colour = colourArr[i];
         tile2->shape = shapeArr[j];

         Node *node1 = new Node(tile1, NULL);
         Node *node2 = new Node(tile2, NULL);
         linkedList.insert(node1);
         linkedList.insert(node2);
      }
   }
   return linkedList;
}

LinkedList initHand(LinkedList &linkedList, int obj[])
{

   Colour colourArr[] = "ROYGBP";
   Shape shapeArr[] = {1, 2, 3, 4, 5, 6};
   Tile tile[72];
   for (int i = 0; i < 6; i++)
   {
      for (int j = 0; j < 6; j++)
      {
         int pos1 = i + j * 6;
         int pos2 = i + j * 6 + 36;
         tile[pos1].colour = colourArr[i];
         tile[pos1].shape = shapeArr[j];
         tile[pos2].colour = colourArr[i];
         tile[pos2].shape = shapeArr[j];
      }
   }

   LinkedList hand;
   hand.create();

   srand((int)time(0)); // radomn seed is zero
   int k = 0;
   while (k < 6)
   {
      int randInt = rand() % 72;
      if (obj[randInt] == 0)
      {
         Tile *tileNew = new Tile();
         tileNew->colour = tile[randInt].colour;
         tileNew->shape = tile[randInt].shape;

         Node *nodeNew = new Node(tileNew, NULL);

         hand.insert(nodeNew);
         linkedList.cut(tile[randInt].colour, tile[randInt].shape);
         obj[randInt] = 1;
         k++;
      }
   }
   return hand;
}

void mainMenu()
{
   char c;
   do
   {
      std::cout << "Input the Number(1~4): \n";
      std::cin >> c;
      switch (c)
      {
      case '1':
         std::cout << "1 step" << std::endl;
         break;
      case '2':
         std::cout << "2 step" << std::endl;
         break;
      case '3':
         std::cout << "3 step" << std::endl;
         break;
      case '4':
         std::cout << "Good bye" << std::endl;
         break;
      default:
         std::cout << "Input error" << std::endl;
      }
   } while (c != '4');
}

string readEnvStdin()
{
   //TODO
   char stdi[100];
   std::cin.getline(stdi, 100);
   return stdi;
}

vector<string> charSplit(const string &str)
{
   vector<string> result;
   char *s = new char[str.length() + 1];
   strcpy(s, str.c_str());
   const char *sep = " "; //delim char
   char *p;
   p = strtok(s, sep);
   while (p)
   {
      result.push_back(p);
      p = strtok(NULL, sep);
   }
   return result;
}

int checkReplaceFormat(vector<string> result)
{
   if (result.size() != 2)
   {
      return 0;
   }
   else if (result.at(0) != "replace")
   {
      return 1;
   }
   else
   {
      switch (result.at(1)[0])
      {
      case 'R':
         break;
      case 'O':
         break;
      case 'Y':
         break;
      case 'G':
         break;
      case 'B':
         break;
      case 'P':
         break;
      default:
         return 2;
      }
      switch (result.at(1)[1])
      {
      case '1':
         break;
      case '2':
         break;
      case '3':
         break;
      case '4':
         break;
      case '5':
         break;
      case '6':
         break;
      default:
         return 3;
      }
   }
   return 4;
}

int checkReplaceLegal(vector<string> result, LinkedList linkedList)
{
   int flag = checkReplaceFormat(result);
   if (flag == 4)
   {
      char colour = result.at(1)[0];
      int shape = atoi(&result.at(1)[1]);
      Node *node = linkedList.find(colour, shape);
      if (node)
      {
         flag = 5;
      }
   }
   return flag;
}

void replaceHand(vector<string> result, LinkedList &linkedList, LinkedList &hand, int obj[])
{

   Colour colourArr[] = "ROYGBP";
   Shape shapeArr[] = {1, 2, 3, 4, 5, 6};
   Tile tile[72];
   Tile *handTile = new Tile();
   Tile *bagTile = new Tile();
   Node *handNode;
   Node *bagNode;
   for (int i = 0; i < 6; i++)
   {
      for (int j = 0; j < 6; j++)
      {
         int pos1 = i + j * 6;
         int pos2 = i + j * 6 + 36;
         tile[pos1].colour = colourArr[i];
         tile[pos1].shape = shapeArr[j];
         tile[pos2].colour = colourArr[i];
         tile[pos2].shape = shapeArr[j];
      }
   }

   srand((int)time(0)); // radomn seed is zero
   int k = 0;
   while (k < 1)
   {
      int randInt = rand() % 72;
      if (obj[randInt] == 0)
      {
         handTile->colour = tile[randInt].colour;
         handTile->shape = tile[randInt].shape;
         handNode = new Node(handTile, NULL);

         bagTile->colour = result.at(1)[0];
         bagTile->shape = atoi(&result.at(1)[1]);
         bagNode = new Node(bagTile, NULL);

         hand.cut(result.at(1)[0], atoi(&result.at(1)[1]));
         hand.insert(handNode);

         linkedList.cut(tile[randInt].colour, tile[randInt].shape);
         linkedList.insert(bagNode);
         obj[randInt] = 1;
         k++;
      }
   }

   for (int i = 0; i < 72; i++)
   {
      if (tile[i].colour == result.at(1)[0] && tile[i].shape == atoi(&result.at(1)[1]))
      {
         if (obj[i] == 1)
         {
            obj[i] = 0;
            break;
         }
      }
   }
}

int checkPlaceFormat(vector<string> result)
{
   if (result.size() != 4)
   {
      return 0;
   }
   else if (result.at(0) != "place")
   {
      return 1;
   }
   else if (result.at(2) != "at")
   {
      return 2;
   }
   else
   {
      switch (result.at(1)[0])
      {
      case 'R':
         break;
      case 'O':
         break;
      case 'Y':
         break;
      case 'G':
         break;
      case 'B':
         break;
      case 'P':
         break;
      default:
         return 3;
      }
      switch (result.at(1)[1])
      {
      case '1':
         break;
      case '2':
         break;
      case '3':
         break;
      case '4':
         break;
      case '5':
         break;
      case '6':
         break;
      default:
         return 4;
      }
      switch (result.at(3)[0])
      {
      case 'A':
         break;
      case 'B':
         break;
      case 'C':
         break;
      case 'D':
         break;
      case 'E':
         break;
      case 'F':
         break;
      case 'G':
         break;
      case 'H':
         break;
      case 'I':
         break;
      case 'J':
         break;
      case 'K':
         break;
      case 'L':
         break;
      case 'M':
         break;
      case 'N':
         break;
      case 'O':
         break;
      case 'P':
         break;
      case 'Q':
         break;
      case 'R':
         break;
      case 'S':
         break;
      case 'T':
         break;
      case 'U':
         break;
      case 'V':
         break;
      case 'W':
         break;
      case 'X':
         break;
      case 'Y':
         break;
      case 'Z':
         break;
      default:
         return 5;
      }
      switch (result.at(3)[1])
      {
      case '1':
         break;
      case '2':
         break;
      case '3':
         break;
      case '4':
         break;
      case '5':
         break;
      case '6':
         break;
      default:
         return 6;
      }
   }
   return 7;
}

int checkPlaceLegal(vector<string> result, LinkedList linkedList)
{
   int flag = checkPlaceFormat(result);
   if (flag == 7)
   {
      char colour = result.at(1)[0];
      int shape = atoi(&result.at(1)[1]);
      Node *node = linkedList.find(colour, shape);
      if (node)
      {
         flag = 8;
      }
   }
   return flag;
}

int palceAtVector(string tileStr, string placeStr, vector<vector<string> > &obj, Player &player)
{
   int flag = 0;
   for (int i = 0; i < obj.size(); i++)
   {
      for (int j = 0; j < obj[i].size(); j++)
      {
         if (obj[i][j] != "  ")
         {
            flag = 2;
         }
      }
   }
   int row = 0;
   int col = atoi(&placeStr[1]);
   switch (placeStr[0])
   {
   case 'A':
      row = 0;
      break;
   case 'B':
      row = 1;
      break;
   case 'C':
      row = 2;
      break;
   case 'D':
      row = 3;
      break;
   case 'E':
      row = 4;
      break;
   case 'F':
      row = 5;
      break;
   case 'G':
      row = 6;
      break;
   case 'H':
      row = 7;
      break;
   case 'I':
      row = 8;
      break;
   case 'J':
      row = 9;
      break;
   case 'K':
      row = 10;
      break;
   case 'L':
      row = 11;
      break;
   case 'M':
      row = 12;
      break;
   case 'N':
      row = 13;
      break;
   case 'O':
      row = 14;
      break;
   case 'P':
      row = 15;
      break;
   case 'Q':
      row = 16;
      break;
   case 'R':
      row = 17;
      break;
   case 'S':
      row = 18;
      break;
   case 'T':
      row = 19;
      break;
   case 'U':
      row = 20;
      break;
   case 'V':
      row = 21;
      break;
   case 'W':
      row = 22;
      break;
   case 'X':
      row = 23;
      break;
   case 'Y':
      row = 24;
      break;
   case 'Z':
      row = 25;
      break;
   default:
      break;
   }
   //cout << "row:" << row << ",col:" << col << endl;
   if (flag == 0)
   {
      obj[row][col] = tileStr;
      flag = 1;
   }
   else
   {
      if (row == 0)
      {
         if (col == 0)
         {
            if (obj[row][col + 1] != "  " || obj[row + 1][col] != "  ")
            {
               obj[row][col] = tileStr;
               flag = 1;
            }
         }
         else if (col == 25)
         {
            if (obj[row][col - 1] != "  " || obj[row + 1][col] != "  ")
            {
               obj[row][col] = tileStr;
               flag = 1;
            }
         }
         else
         {
            if (obj[row][col - 1] != "  " || obj[row][col + 1] != "  " || obj[row + 1][col] != "  ")
            {
               obj[row][col] = tileStr;
               flag = 1;
            }
         }
      }
      else if (row == 25)
      {
         if (col == 0)
         {
            if (obj[row][col + 1] != "  " || obj[row - 1][col] != "  ")
            {
               obj[row][col] = tileStr;
               flag = 1;
            }
         }
         else if (col == 25)
         {
            if (obj[row][col - 1] != "  " || obj[row - 1][col] != "  ")
            {
               obj[row][col] = tileStr;
               flag = 1;
            }
         }
         else
         {
            if (obj[row][col - 1] != "  " || obj[row][col + 1] != "  " || obj[row - 1][col] != "  ")
            {
               obj[row][col] = tileStr;
               flag = 1;
            }
         }
      }
      else
      {
         if (col == 0)
         {
            if (obj[row - 1][col] != "  " || obj[row + 1][col] != "  " || obj[row][col + 1] != "  ")
            {
               obj[row][col] = tileStr;
               flag = 1;
            }
         }
         else if (col == 25)
         {
            if (obj[row - 1][col] != "  " || obj[row + 1][col] != "  " || obj[row][col - 1] != "  ")
            {
               obj[row][col] = tileStr;
               flag = 1;
            }
         }
         else
         {
            if (obj[row - 1][col] != "  " || obj[row + 1][col] != "  " || obj[row][col - 1] != "  " || obj[row][col + 1] != "  ")
            {
               obj[row][col] = tileStr;
               flag = 1;
            }
         }
      }
   }
   if (flag == 1)
   {
      player.score += makeScore(row, col, obj);
   }
   return flag;
}

void printVector(std::vector<std::vector<std::string> > obj)
{
   for (int j = 0; j < obj[0].size(); j++)
   {
      if (j == 0)
      {
         std::cout << "   " << j;
      }
      else if (j < 10)
      {
         std::cout << "  " << j;
      }
      else
      {
         std::cout << " " << j;
      }
   }
   std::cout << std::endl;
   for (int j = 0; j < obj[0].size(); j++)
   {
      if (j == 0)
      {
         std::cout << "  ----";
      }
      else
      {
         std::cout << "----";
      }
   }
   std::cout << std::endl;
   for (int i = 0; i < obj.size(); i++)
   {
      switch (i)
      {
      case 0:
         std::cout << 'A' << " |";
         break;
      case 1:
         std::cout << 'B' << " |";
         break;
      case 2:
         std::cout << 'C' << " |";
         break;
      case 3:
         std::cout << 'D' << " |";
         break;
      case 4:
         std::cout << 'E' << " |";
         break;
      case 5:
         std::cout << 'F' << " |";
         break;
      case 6:
         std::cout << 'G' << " |";
         break;
      case 7:
         std::cout << 'H' << " |";
         break;
      case 8:
         std::cout << 'I' << " |";
         break;
      case 9:
         std::cout << 'J' << " |";
         break;
      case 10:
         std::cout << 'K' << " |";
         break;
      case 11:
         std::cout << 'L' << " |";
         break;
      case 12:
         std::cout << 'M' << " |";
         break;
      case 13:
         std::cout << 'N' << " |";
         break;
      case 14:
         std::cout << 'O' << " |";
         break;
      case 15:
         std::cout << 'P' << " |";
         break;
      case 16:
         std::cout << 'Q' << " |";
         break;
      case 17:
         std::cout << 'R' << " |";
         break;
      case 18:
         std::cout << 'S' << " |";
         break;
      case 19:
         std::cout << 'T' << " |";
         break;
      case 20:
         std::cout << 'U' << " |";
         break;
      case 21:
         std::cout << 'V' << " |";
         break;
      case 22:
         std::cout << 'W' << " |";
         break;
      case 23:
         std::cout << 'X' << " |";
         break;
      case 24:
         std::cout << 'Y' << " |";
         break;
      case 25:
         std::cout << 'Z' << " |";
         break;
      default:
         break;
      }
      for (int j = 0; j < obj[i].size(); j++)
      {
         std::cout << obj[i][j] << "|";
      }
      std::cout << "\n";
   }
}

int makeScore(int row, int col, vector<vector<string> > obj)
{
   int score = 0;
   int rowScore = 0;
   int colScore = 0;
   for (int colBefore = col - 1; colBefore > -1; colBefore--)
   {
      if (obj[row][colBefore] != "  ")
      {
         rowScore += 1;
      }
      else
      {
         break;
      }
   }
   for (int colAfter = col + 1; colAfter < obj[0].size(); colAfter++)
   {
      if (obj[row][colAfter] != "  ")
      {
         rowScore += 1;
      }
      else
      {
         break;
      }
   }

   for (int rowUp = row - 1; rowUp > -1; rowUp--)
   {
      if (obj[rowUp][col] != "  ")
      {
         colScore += 1;
      }
      else
      {
         break;
      }
   }
   for (int rowDown = row + 1; rowDown < obj.size(); rowDown++)
   {
      if (obj[rowDown][col] != "  ")
      {
         colScore += 1;
      }
      else
      {
         break;
      }
   }

   if (rowScore == 0 && colScore == 0)
   {
      score = 1;
   }
   else if (rowScore == 0)
   {
      score = colScore + 1;
      if (colScore == 5)
      {
         score += 6;
         cout << "qwirkle!!!" << endl;
      }
   }
   else if (colScore == 0)
   {
      score = rowScore + 1;
      if (rowScore == 5)
      {
         score += 6;
         cout << "qwirkle!!!" << endl;
      }
   }
   else
   {
      score = (rowScore + 1) + (colScore + 1);
      if (rowScore == 6)
      {
         score += 6;
         cout << "qwirkle!!!" << endl;
      }
      if (colScore == 5)
      {
         score += 6;
         cout << "qwirkle!!!" << endl;
      }
   }

   return score;
}