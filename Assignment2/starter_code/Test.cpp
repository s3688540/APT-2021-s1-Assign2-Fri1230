#include "LinkedList.h"
#include "Player.h"

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cctype>
using namespace std;

#define EXIT_SUCCESS 0

#define TILE_TOTAL_NUMBER 72
#define HAND_TILE_NUMBER 6
#define BOARD_MAX_SIZE 26

string readStdin();
vector<string> charSplit(const string &str);
vector<int> initRandObj();
LinkedList initBag(vector<int> randObj);
LinkedList initHand(LinkedList &linkedList);
int addHand(LinkedList &linkedList, LinkedList &hand);
int replaceHand(vector<string> result, LinkedList &linkedList, LinkedList &hand);

bool myfunction(int i, int j) { return (i < j); }

int main(int argc, char const *argv[])
{
    Player player1;
    Player player2;
    LinkedList linkedList;
    vector<int> obj;
    vector<string> result;

    vector<vector<string> > boardVector(BOARD_MAX_SIZE, vector<string>(BOARD_MAX_SIZE));
    for (int i = 0; i < (int)boardVector.size(); i++)
    {
        for (int j = 0; j < (int)boardVector[i].size(); j++)
        {
            boardVector[i][j] = "--";
        }
    }

    for (int i = 0; i < (int)boardVector.size(); i++)
    {
        for (int j = 0; j < (int)boardVector[i].size(); j++)
        {
            cout << boardVector[i][j];
        }
        cout << endl;
    }

    if ((int)boardVector.size() > -1)
    {
        cout << "Hello,World" << endl;
    }
    else
    {
        cout << "That's too bad" << endl;
    }

    

    srand((int)time(0)); // radomn seed is zero
    int randInt = rand() % 72;
    cout << "randInt:" << randInt << endl;

    obj = initRandObj();

    linkedList = initBag(obj);
    //linkedList.print();
    linkedList.printHand();
    player1.hand = initHand(linkedList);
    player2.hand = initHand(linkedList);
    player1.hand.printHand();
    player2.hand.printHand();

    cout << "------After addHand------" << endl;
    addHand(linkedList, player1.hand);
    linkedList.printHand();
    player1.hand.printHand();


   /*
    for (int i = 0; i < TILE_TOTAL_NUMBER; i++)
    {
        cout << i + 1 << ":" << obj[i] << endl;
    }

    sort(obj.begin(), obj.end(), myfunction);

    for (int i = 0; i < TILE_TOTAL_NUMBER; i++)
    {
        cout << i + 1 << ":" << obj[i] << endl;
    }

    cout << "------Before repalce------" << endl;
    cout<<"linkedList"<<endl;
    linkedList.printHand();
    cout<<"player1 hand"<<endl;
    player1.hand.printHand();
    string stdin = readStdin();
    cout<<"stdin:"<<stdin<<endl;

    result = charSplit(stdin);
    for(int i = 0;i<(int)result.size();i++){
        cout<<"result"<<i<<":"<<result[i]<<endl;
    }

    replaceHand(result,linkedList,player1.hand);
    
    cout << "------After repalce------" << endl;
    cout<<"linkedList"<<endl;
    linkedList.printHand();
    cout<<"player1 hand"<<endl;
    player1.hand.printHand();

*/
    return 0;
}
/*
string readStdin()
{
   //TODO
   string stdin;
   getline(cin,stdin);
   return stdin;
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
*/
vector<int> initRandObj()
{
    vector<int> objRand;
    int obj[TILE_TOTAL_NUMBER];
    int flag = 0;
    int randInt;

    for (int i = 0; i < TILE_TOTAL_NUMBER; i++)
    {
        obj[i] = 0;
    }

    srand((int)time(NULL)); // radomn seed is zero
    while (flag < TILE_TOTAL_NUMBER)
    {
        randInt = rand() % TILE_TOTAL_NUMBER;
        if (obj[randInt] == 0)
        {
            objRand.push_back(randInt);
            obj[randInt] = 1;
            flag++;
        }
    }

    return objRand;
}

LinkedList initBag(vector<int> randObj)
{
    LinkedList linkedList;
    Colour colourArr[] = "ROYGBP";
    Shape shapeArr[] = {1, 2, 3, 4, 5, 6};
    Tile tile[72];

    linkedList.create(); // init linkedList

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

    //test tile
    for(int i=0;i<36;i++){
        cout<<"Tile"<<i+1<<":"<<tile[i].colour<<tile[i].shape<<"    "<<"Tile"<<i+1+36<<":"<<tile[i+36].colour<<tile[i+36].shape<<endl;
    }

    for (int i = 0; i < (int)randObj.size(); i++)
    {

        Tile *tileNew = new Tile();
        tileNew->colour = tile[randObj[i]].colour;
        tileNew->shape = tile[randObj[i]].shape;

        Node *node = new Node(tileNew, NULL);
        linkedList.insert(node);
    }
    return linkedList;
}

LinkedList initHand(LinkedList &linkedList)
{

    LinkedList hand;
    Node *head;

    head = linkedList.getHead();
    hand.create();

    srand((int)time(0)); // radomn seed is zero
    int k = 0;
    while (k < 6)
    {
        if (linkedList.size() < 1)
        {
            break;
        }
        else
        {
            Tile *tileNew = new Tile();
            tileNew->colour = head->next->tile->colour;
            tileNew->shape = head->next->tile->shape;

            Node *nodeNew = new Node(tileNew, NULL);

            hand.insert(nodeNew);
            linkedList.cut(head->next->tile->colour, head->next->tile->shape);
            head = linkedList.getHead();
            k++;
        }
    }
    return hand;
}

int addHand(LinkedList &linkedList, LinkedList &hand)
{

    int flag;
    Node *head;

    head = linkedList.getHead();

    if (linkedList.size() < 1)
    {
        flag = 0;
    }
    else
    {
        Tile *tileNew = new Tile();
        tileNew->colour = head->next->tile->colour;
        tileNew->shape = head->next->tile->shape;

        Node *nodeNew = new Node(tileNew, NULL);

        hand.insert(nodeNew);
        linkedList.cut(head->next->tile->colour, head->next->tile->shape);
        head = linkedList.getHead();
        flag = 1;
    }
    return flag;
}

/*
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
*/

int replaceHand(vector<string> result, LinkedList &linkedList, LinkedList &hand)
{
    int flag;
    Tile *handTile = new Tile();
    Tile *bagTile = new Tile();
    Node *head;
    Node *handNode;
    Node *bagNode;

    head = linkedList.getHead();

    if (linkedList.size() < 1) // linkedList size should > 0
    {
        flag = 0;
    }
    else
    {

        handTile->colour = head->next->tile->colour;
        handTile->shape = head->next->tile->shape;
        handNode = new Node(handTile, NULL);

        bagTile->colour = result.at(1)[0];
        bagTile->shape = atoi(&result.at(1)[1]);
        bagNode = new Node(bagTile, NULL);

        hand.cut(result.at(1)[0], atoi(&result.at(1)[1]));
        hand.insert(handNode);

        linkedList.cut(head->next->tile->colour, head->next->tile->shape);
        linkedList.insert(bagNode);
        flag = 1;
    }
    return flag;
} 