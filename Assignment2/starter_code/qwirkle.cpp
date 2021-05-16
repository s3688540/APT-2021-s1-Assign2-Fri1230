#include "LinkedList.h"
#include "Player.h"
#include "Student.h"
#include "GameFile.h"
#include<string.h>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cctype>
using namespace std;

#define EXIT_SUCCESS 0

#define COLOUR_ARR "ROYGBP"
#define SHAPE_ARR {1, 2, 3, 4, 5, 6}
#define TILE_TOTAL_NUMBER 72
#define COLOUR_TYPE 6
#define SHAPE_TYPE 6
#define HAND_TILE_NUMBER 6 
#define BOARD_MAX_SIZE 26
#define MAX_CHAR_NUMBER 1000

string readStdin();                                                                               // 1.
vector<string> charSplit(const string& str);                                                      // 2.
vector<int> initRandObj();                                                                        // 3.
LinkedList initBag(vector<int> randObj);                                                          // 4.
LinkedList initHand(LinkedList& linkedList);                                                      // 5.
vector<vector<string> > initBoard();                                                              // 6.
int checkReplaceFormat(vector<string> result);                                                    // 7.
int checkReplaceLegal(vector<string> result, LinkedList hand);                                    // 8.
int replaceHand(vector<string> result, LinkedList& linkedList, LinkedList& hand);                 // 9.
int checkPlaceFormat(vector<string> result);                                                      // 10.
int checkPlaceLegal(vector<string> result, LinkedList hand);                                      // 11.
int palceAtVector(string tileStr, string placeStr, vector<vector<string> >& obj, Player& player); // 12.
int checkSaveLegal(vector<string> result);
int palceObeyRule(string tileStr, int row, int col, vector<vector<string> > &obj); // 13

int makeScore(int row, int col, vector<vector<string> > obj); // 14.
int addHand(LinkedList& linkedList, LinkedList& hand);        // 15.
void printVector(vector<vector<string> > obj);                // 16.

void welcome();         // 17.
void mainMenu();        // 18.
void showStudentInfo(); // 19.
int checkPlayName(string name); // 20.
void winner(Player player1, Player player2);
void newGame(LinkedList& bagLinkedList, Player& player1, Player& player2, vector<vector<string> >& boardVector);
void loadGame(LinkedList& bagLinkedList, Player& player1, Player& player2, vector<vector<string>>& boardVector);
void saveGame(LinkedList& bagLinkedList, Player& player1, Player& player2, vector<vector<string>>& boardVector, string filename);
void play(LinkedList& bagLinkedList, Player& player1, Player& player2, vector<vector<string> >& boardVector, bool turnFlag);

int main(int argc, char const* argv[])
{
    LinkedList bagLinkedList;
    Player player1;
    Player player2;
    vector<vector<string> > boardVector;

    welcome();
    mainMenu();
    boardVector = initBoard();

    string stdins;
    do
    {
        cout << ">";
        stdins = readStdin();
        if (stdins.length() != 1)
        {
            cout << "Input length is over!You should input number 1 0r 2 or 3 or 4." << endl;
        }else{
            switch (stdins[0])
            {
            case '1':
                newGame(bagLinkedList, player1, player2, boardVector);
                mainMenu();
                break;

            case '2':
                loadGame(bagLinkedList, player1, player2, boardVector);
                mainMenu();
                break;

            case '3':
                showStudentInfo();
                mainMenu();
                break;

            case '4':
                cout << "Good bye" << endl;
                break;

            default:
                std::cout << "Sorry,input error.!!!You should input number 1 0r 2 or 3 or 4." << std::endl;
            }
        }
    } while(stdins[0] != '4' || stdins.length() != 1);


    return EXIT_SUCCESS;
}

// 1.read the input from terminal
string readStdin()
{
    //TODO
    char stdi[MAX_CHAR_NUMBER];
    cin.getline(stdi, MAX_CHAR_NUMBER);
    return stdi;
}

// 2. split the input of string by "  "
vector<string> charSplit(const string& str)
{
    vector<string> result;
    char* s = new char[str.length() + 1];
    strcpy(s, str.c_str());
    const char* sep = " "; //delim char
    char* p;
    p = strtok(s, sep);
    while (p)
    {
        result.push_back(p);
        p = strtok(NULL, sep);
    }
    return result;
}

// 3. init a random vector to init bag of tiles
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

// 4. init a bag of tiles by a random vector
LinkedList initBag(vector<int> randObj)
{
    LinkedList linkedList;
    Colour colourArr[] = COLOUR_ARR;
    Shape shapeArr[] = SHAPE_ARR;
    Tile tile[TILE_TOTAL_NUMBER];

    linkedList.create(); // init linkedList

    for (int i = 0; i < COLOUR_TYPE; i++)
    {
        for (int j = 0; j < SHAPE_TYPE; j++)
        {
            int pos1 = i + j * COLOUR_TYPE;
            int pos2 = i + j * COLOUR_TYPE + (TILE_TOTAL_NUMBER / 2);
            tile[pos1].colour = colourArr[i];
            tile[pos1].shape = shapeArr[j];
            tile[pos2].colour = colourArr[i];
            tile[pos2].shape = shapeArr[j];
        }
    }

    for (int i = 0; i < (int)randObj.size(); i++)
    {

        Tile* tileNew = new Tile();
        tileNew->colour = tile[randObj[i]].colour;
        tileNew->shape = tile[randObj[i]].shape;

        Node* node = new Node(tileNew, NULL);
        linkedList.insert(node);
    }
    return linkedList;
}

// 5. init a play hand of tiles from bag of tiles
LinkedList initHand(LinkedList& linkedList)
{

    LinkedList hand;
    Node* head;

    head = linkedList.getHead();
    hand.create();

    srand((int)time(0)); // radomn seed is zero
    int k = 0;
    while (k < HAND_TILE_NUMBER)
    {
        if (linkedList.size() < 1)
        {
            break;
        }
        else
        {
            Tile* tileNew = new Tile();
            tileNew->colour = head->next->tile->colour;
            tileNew->shape = head->next->tile->shape;

            Node* nodeNew = new Node(tileNew, NULL);

            hand.insert(nodeNew);
            linkedList.cut(head->next->tile->colour, head->next->tile->shape);
            head = linkedList.getHead();
            k++;
        }
    }
    return hand;
}

// 6. init a Board
vector<vector<string> > initBoard()
{
    vector<vector<string> > boardVector(BOARD_MAX_SIZE, vector<string>(BOARD_MAX_SIZE));
    for (int i = 0; i < (int)boardVector.size(); i++)
    {
        for (int j = 0; j < (int)boardVector[i].size(); j++)
        {
            boardVector[i][j] = "  ";
        }
    }
    return boardVector;
}

// 7. check replace format,if legal return 4,else return 0 or 1 or 2 or 3
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

// 8. check whether could replace succeed or not, if 'yes' return 5,else return 0 or 1 or 2 or 3 or 4
int checkReplaceLegal(vector<string> result, LinkedList hand)
{
    int flag = checkReplaceFormat(result);
    if (flag == 4)
    {
        char colour = result.at(1)[0];
        int shape = atoi(&result.at(1)[1]);
        Node* node = hand.find(colour, shape);
        if (node)
        {
            flag = 5;
        }
    }
    return flag;
}

// 9. check replace succeed or not, if succeed return 1,else return 0
int replaceHand(vector<string> result, LinkedList& linkedList, LinkedList& hand)
{
    int flag;
    Tile* handTile = new Tile();
    Tile* bagTile = new Tile();
    Node* head;
    Node* handNode;
    Node* bagNode;

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

// 10. check place format,if legal return 7,else return 0 or 1 or 2 or 3 or 4 or 5 or 6
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
        if (result.at(1).size() != 2)
        {
            return 3;
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
        }

        if (result.at(3).size() != 2 && result.at(3).size() != 3)
        {
            return 5;
        }
        else if (result.at(3).size() == 2)
        {
            switch (result.at(3)[1])
            {
            case '0':
                break;
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
            case '7':
                break;
            case '8':
                break;
            case '9':
                break;
            default:
                return 6;
            }
        }
        else
        {
            if (result.at(3)[1] != '1' && result.at(3)[1] != '2')
            {
                return 6;
            }
            else if (result.at(3)[1] == '1')
            {
                switch (result.at(3)[2])
                {
                case '0':
                    break;
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
                case '7':
                    break;
                case '8':
                    break;
                case '9':
                    break;
                default:
                    return 6;
                }
            }
            else
            {
                switch (result.at(3)[2])
                {
                case '0':
                    break;
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
                default:
                    return 6;
                }
            }
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
    }
    return 7;
}

// 11. check whether could place  or not, if ��yes�� return 8,else return 0 or 1 or 2 or 3 or 4 or 5 or 6 or 7
int checkPlaceLegal(vector<string> result, LinkedList hand)
{
    int flag = checkPlaceFormat(result);
    if (flag == 7)
    {
        char colour = result.at(1)[0];
        int shape = atoi(&result.at(1)[1]);
        Node* node = hand.find(colour, shape);
        if (node)
        {
            flag = 8;
        }
    }
    return flag;
}

// 12. check replace succeed or not, if succeed return 1,else return 0
int palceAtVector(string tileStr, string placeStr, vector<vector<string> > &obj, Player &player)
{
    int flag = 0;
    for (int i = 0; i < (int)obj.size(); i++)
    {
        for (int j = 0; j < (int)obj[i].size(); j++)
        {
            if (obj[i][j] != "  ")
            {
                flag = 2;
            }
        }
    }
    int row = 0;
    int col = 0;

    // transfer row
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

    // transfer col
    if (placeStr.size() == 2)
    {
        col = atoi(&placeStr[1]);
    }
    else
    {
        if (placeStr[1] == '1')
        {
            col = atoi(&placeStr[2]) + 10;
        }
        else
        {
            col = atoi(&placeStr[2]) + 20;
        }
    }
    //cout << "row:" << row << ",col:" << col << endl;
    if (flag == 0)
    {
        obj[row][col] = tileStr;
        flag = 1;
    }
    else
    {
        if (obj[row][col] == "  " && palceObeyRule(tileStr, row, col, obj) == 1)
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
    }
    if (flag == 1)
    {
        player.score += makeScore(row, col, obj);
    }
    return flag;
}

// 13. check place whether obey Rule of qwirkle,if obey retrun 1,else return 0
int palceObeyRule(string tileStr, int row, int col, vector<vector<string> > &obj)
{
    int flag = 0;
    int rowUpNUm = 0;
    int rowDownNUm = 0;
    int colBeforeNum = 0;
    int colAfterNum = 0;
    char rowUpSameTile = 0;
    char rowDownSameTile = 0;
    char colBeforeSameTile = 0;
    char colAfterSameTile = 0;

    if (row > 0)
    {

        for (int rowUp = row - 1; rowUp > -1; rowUp--)
        {
            if (obj[rowUp][col] != "  ")
            {
                rowUpNUm += 1;
                if (obj[rowUp][col] == tileStr)
                {
                    rowUpSameTile = 1;
                }
            }
            else
            {
                break;
            }
        }
    }

    if (row < BOARD_MAX_SIZE - 1)
    {
        for (int rowDown = row + 1; rowDown < (int)obj.size(); rowDown++)
        {
            if (obj[rowDown][col] != "  ")
            {
                rowDownNUm += 1;
                if (obj[rowDown][col] == tileStr)
                {
                    rowDownSameTile = 1;
                }
            }
            else
            {
                break;
            }
        }
    }

    if (col > 0)
    {
        for (int colBefore = col - 1; colBefore > -1; colBefore--)
        {
            if (obj[row][colBefore] != "  ")
            {
                colBeforeNum += 1;
                if (obj[row][colBefore] == tileStr)
                {
                    colBeforeSameTile = 1;
                }
            }
            else
            {
                break;
            }
        }
    }

    if (col < BOARD_MAX_SIZE - 1)
    {
        for (int colAfter = col + 1; colAfter < (int)obj[0].size(); colAfter++)
        {
            if (obj[row][colAfter] != "  ")
            {
                colAfterNum += 1;
                if (obj[row][colAfter] == tileStr)
                {
                    colAfterSameTile = 1;
                }
            }
            else
            {
                break;
            }
        }
    }

    //cout<<"rowUpNUm:"<<rowUpNUm<<",rowDownNUm:"<<rowDownNUm<<",colBeforeNum:"<<colBeforeNum<<",colAfterNum:"<<colAfterNum<<endl;
    // 1. row or col can't has the same tile
    if (rowUpSameTile == 1 || rowDownSameTile == 1 || colBeforeSameTile == 1 || colAfterSameTile == 1)
    {
        flag = 0;
        return flag;
    }

    //  2. the tiles in row or col should <= 6
    if (rowUpNUm == 6 || rowDownNUm == 6 || colBeforeNum == 6 || colAfterNum == 6)
    {
        flag = 0;
        return flag;
    }
    else if ((rowUpNUm + rowDownNUm >= 6) || (colBeforeNum + colBeforeNum >= 6))
    {
        flag = 0;
        return flag;
    }
    else
    { // attetion: there is no tile in row or col is the same as tile you want to place
        if (rowUpNUm == 0)  // 3.rowUpNUm == 0
        { 
            if (rowDownNUm == 0) // 3.1 rowDownNUm == 0
            {
                if (colBeforeNum == 0)  // 3.1.1 colBeforeNum == 0
                {
                    if (colAfterNum == 0)   // 3.1.1.1 colAfterNum == 0
                    {
                        flag = 1;
                        
                    }
                    else if (colAfterNum == 1)  // 3.1.1.2 colAfterNum == 1
                    {
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col + 1][0] || tileStr[1] == obj[row][col + 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else // 3.1.1.3 colAfterNum > 1
                    {

                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    
                }
                else if (colBeforeNum == 1) // 3.1.2 colBeforeNum == 1
                {
                    // should not be same as tileStr
                    if (tileStr == obj[row][col - 1])
                    {
                        flag = 0;
                        return flag;
                    }
                    if (colAfterNum == 0) // 3.1.2.1 colAfterNum == 0
                    {
                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col - 1][0] || tileStr[1] == obj[row][col - 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1)  // 3.1.2.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row should not has same tile
                        if (obj[row][col - 1] == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }
                        else
                        {
                            if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                            {
                                flag = 1;
                            }
                            else
                            {
                                flag = 0;
                                return 0;
                            }
                        }
                    }
                    else    // 3.1.2.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row should not has same tile
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (obj[row][col - 1] == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
                else    // 3.1.3 colBeforeNum > 1
                {
                    // should not be same as tileStr
                    for (int i = 1; i <= colBeforeNum; i++)
                    {
                        if (tileStr == obj[row][col - i])
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    if (colAfterNum == 0)   // 3.1.3.1 colAfterNum == 0
                    {
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1) // 3.1.3.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            if(obj[row][col+1] == obj[row][col-i]){
                                flag = 0;
                                return flag;
                            }
                        }


                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                    else    // 3.1.3.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            for(int j=1;j<colAfterNum;j++){
                                if(obj[row][col+j] == obj[row][col-i]){
                                    flag = 0;
                                    return flag;
                                }
                            }
                        }

                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
            }
            else if (rowDownNUm == 1)   //  3.2 rowDownNUm == 1
            {
                // should not be same as tileStr
                if(tileStr == obj[row+1][col]){
                    flag = 0;
                    return flag;
                }

                if(tileStr[0] != obj[row+1][col][0] && tileStr[1] != obj[row+1][col][1]){
                    flag = 0;
                    return flag;
                }

                if (colBeforeNum == 0)  // 3.2.1 colBeforeNum == 0
                {
                    if (colAfterNum == 0)   // 3.2.1.1 colAfterNum == 0
                    {
                        flag = 1;
                        
                    }
                    else if (colAfterNum == 1)  // 3.2.1.2 colAfterNum == 1
                    {
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col + 1][0] || tileStr[1] == obj[row][col + 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else // 3.2.1.3 colAfterNum > 1
                    {

                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    
                }
                else if (colBeforeNum == 1) // 3.2.2 colBeforeNum == 1
                {
                    // should not be same as tileStr
                    if (tileStr == obj[row][col - 1])
                    {
                        flag = 0;
                        return flag;
                    }
                    if (colAfterNum == 0) // 3.2.2.1 colAfterNum == 0
                    {
                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col - 1][0] || tileStr[1] == obj[row][col - 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1)  // 3.2.2.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        if (obj[row][col - 1] == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }
                        else
                        {
                            if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                            {
                                flag = 1;
                            }
                            else
                            {
                                flag = 0;
                                return 0;
                            }
                        }
                    }
                    else    // 3.2.2.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (obj[row][col - 1] == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
                else    // 3.2.3 colBeforeNum > 1
                {
                    // should not be same as tileStr
                    for (int i = 1; i <= colBeforeNum; i++)
                    {
                        if (tileStr == obj[row][col - i])
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    if (colAfterNum == 0)   // 3.2.3.1 colAfterNum == 0
                    {
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1) // 3.2.3.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            if(obj[row][col+1] == obj[row][col-i]){
                                flag = 0;
                                return flag;
                            }
                        }


                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                    else    // 3.2.3.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            for(int j=1;j<colAfterNum;j++){
                                if(obj[row][col+j] == obj[row][col-i]){
                                    flag = 0;
                                    return flag;
                                }
                            }
                        }

                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
            }
            else //  3.3 rowDownNUm > 1
            {
                // should not be same as tileStr
                for(int i=1;i<=rowDownNUm;i++){
                    if(tileStr == obj[row+i][col]){
                        flag = 0;
                        return flag;
                    }
                }

                if(!((tileStr[0] == obj[row+1][col][0] && tileStr[0] == obj[row+2][col][0]) || (tileStr[1] != obj[row+1][col][1] && tileStr[1] == obj[row+2][col][1]))){
                    flag = 0;
                    return flag;
                }

                if (colBeforeNum == 0)  // 3.3.1 colBeforeNum == 0
                {
                    if (colAfterNum == 0)   // 3.3.1.1 colAfterNum == 0
                    {
                        flag = 1;
                        
                    }
                    else if (colAfterNum == 1)  // 3.3.1.2 colAfterNum == 1
                    {
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col + 1][0] || tileStr[1] == obj[row][col + 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else // 3.3.1.3 colAfterNum > 1
                    {

                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    
                }
                else if (colBeforeNum == 1) // 3.3.2 colBeforeNum == 1
                {
                    // should not be same as tileStr
                    if (tileStr == obj[row][col - 1])
                    {
                        flag = 0;
                        return flag;
                    }
                    if (colAfterNum == 0) // 3.3.2.1 colAfterNum == 0
                    {
                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col - 1][0] || tileStr[1] == obj[row][col - 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1)  // 3.3.2.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        if (obj[row][col - 1] == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }
                        else
                        {
                            if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                            {
                                flag = 1;
                            }
                            else
                            {
                                flag = 0;
                                return 0;
                            }
                        }
                    }
                    else    // 3.3.2.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (obj[row][col - 1] == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
                else    // 3.3.3 colBeforeNum > 1
                {
                    // should not be same as tileStr
                    for (int i = 1; i <= colBeforeNum; i++)
                    {
                        if (tileStr == obj[row][col - i])
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    if (colAfterNum == 0)   // 3.3.3.1 colAfterNum == 0
                    {
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1) // 3.3.3.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            if(obj[row][col+1] == obj[row][col-i]){
                                flag = 0;
                                return flag;
                            }
                        }


                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                    else    // 3.3.3.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            for(int j=1;j<colAfterNum;j++){
                                if(obj[row][col+j] == obj[row][col-i]){
                                    flag = 0;
                                    return flag;
                                }
                            }
                        }

                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
            }
            
        }
        else if (rowUpNUm == 1) // 4.rowUpNUm == 1
        {
            // should not be same as tileStr
            if(tileStr == obj[row-1][col]){
                flag = 0;
                return 0;
            }

            if(tileStr[0] != obj[row-1][col][0] && tileStr[1] != obj[row-1][col][1]){
                flag = 0;
                return flag;
            }


            if (rowDownNUm == 0) // 4.1 rowDownNUm == 0
            {
                if (colBeforeNum == 0)  // 4.1.1 colBeforeNum == 0
                {
                    if (colAfterNum == 0)   // 4.1.1.1 colAfterNum == 0
                    {
                        flag = 1;
                        
                    }
                    else if (colAfterNum == 1)  // 4.1.1.2 colAfterNum == 1
                    {
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col + 1][0] || tileStr[1] == obj[row][col + 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else // 4.1.1.3 colAfterNum > 1
                    {

                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    
                }
                else if (colBeforeNum == 1) // 4.1.2 colBeforeNum == 1
                {
                    // should not be same as tileStr
                    if (tileStr == obj[row][col - 1])
                    {
                        flag = 0;
                        return flag;
                    }
                    if (colAfterNum == 0) // 4.1.2.1 colAfterNum == 0
                    {
                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col - 1][0] || tileStr[1] == obj[row][col - 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1)  // 4.1.2.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        if (obj[row][col - 1] == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }
                        else
                        {
                            if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                            {
                                flag = 1;
                            }
                            else
                            {
                                flag = 0;
                                return 0;
                            }
                        }
                    }
                    else    // 4.1.2.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (obj[row][col - 1] == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
                else    // 4.1.3 colBeforeNum > 1
                {
                    // should not be same as tileStr
                    for (int i = 1; i <= colBeforeNum; i++)
                    {
                        if (tileStr == obj[row][col - i])
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    if (colAfterNum == 0)   // 4.1.3.1 colAfterNum == 0
                    {
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1) // 4.1.3.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            if(obj[row][col+1] == obj[row][col-i]){
                                flag = 0;
                                return flag;
                            }
                        }


                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                    else    // 4.1.3.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            for(int j=1;j<colAfterNum;j++){
                                if(obj[row][col+j] == obj[row][col-i]){
                                    flag = 0;
                                    return flag;
                                }
                            }
                        }

                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }

            }else if(rowDownNUm == 1){ // 4.2 rowDownNUm == 1

                // should not be same as tileStr
                if(tileStr == obj[row+1][col]){
                    flag = 0;
                    return flag;
                }

                // row or col should not has same tile
                if(obj[row-1][col] == obj[row+1][col]){
                    flag = 0;
                    return flag;
                }

                if(!((tileStr[0] == obj[row-1][col][0] && tileStr[0] == obj[row+1][col][0]) || (tileStr[1] == obj[row-1][col][1] && tileStr[1] == obj[row+1][col][1]))){
                    flag = 0;
                    return flag;
                }

                if (colBeforeNum == 0)  // 4.2.1 colBeforeNum == 0
                {
                    if (colAfterNum == 0)   // 4.2.1.1 colAfterNum == 0
                    {
                        flag = 1;
                        
                    }
                    else if (colAfterNum == 1)  // 4.2.1.2 colAfterNum == 1
                    {
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col + 1][0] || tileStr[1] == obj[row][col + 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else // 4.2.1.3 colAfterNum > 1
                    {

                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    
                }
                else if (colBeforeNum == 1) // 4.2.2 colBeforeNum == 1
                {
                    // should not be same as tileStr
                    if (tileStr == obj[row][col - 1])
                    {
                        flag = 0;
                        return flag;
                    }
                    if (colAfterNum == 0) // 4.2.2.1 colAfterNum == 0
                    {
                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col - 1][0] || tileStr[1] == obj[row][col - 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1)  // 4.2.2.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        if (obj[row][col - 1] == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }
                        else
                        {
                            if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                            {
                                flag = 1;
                            }
                            else
                            {
                                flag = 0;
                                return 0;
                            }
                        }
                    }
                    else    // 4.2.2.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (obj[row][col - 1] == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
                else    // 4.2.3 colBeforeNum > 1
                {
                    // should not be same as tileStr
                    for (int i = 1; i <= colBeforeNum; i++)
                    {
                        if (tileStr == obj[row][col - i])
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    if (colAfterNum == 0)   // 4.2.3.1 colAfterNum == 0
                    {
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1) // 4.2.3.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            if(obj[row][col+1] == obj[row][col-i]){
                                flag = 0;
                                return flag;
                            }
                        }


                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                    else    // 4.2.3.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            for(int j=1;j<colAfterNum;j++){
                                if(obj[row][col+j] == obj[row][col-i]){
                                    flag = 0;
                                    return flag;
                                }
                            }
                        }

                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }

            }else{  // 4.3 rowDownNUm > 1

                // should not be same as tileStr
                for(int i=1;i<=rowDownNUm;i++){
                    if(tileStr == obj[row+i][col]){
                        flag = 0;
                        return flag;
                    }
                }

                // row or col should not be same as tileStr
                for(int i=1;i<=rowDownNUm;i++){
                    if(obj[row-1][col] == obj[row+i][col]){
                        flag = 0;
                        return flag;
                    }
                }


                if(!((tileStr[0] == obj[row-1][col][0] && tileStr[0] == obj[row+1][col][0] && tileStr[0] == obj[row+2][col][0]) || (tileStr[1] == obj[row-1][col][1] && tileStr[1] == obj[row+1][col][1] && tileStr[1] == obj[row+2][col][1]))){
                    flag = 0;
                    return flag;
                }

                if (colBeforeNum == 0)  // 4.3.1 colBeforeNum == 0
                {
                    if (colAfterNum == 0)   // 4.3.1.1 colAfterNum == 0
                    {
                        flag = 1;
                        
                    }
                    else if (colAfterNum == 1)  // 4.3.1.2 colAfterNum == 1
                    {
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col + 1][0] || tileStr[1] == obj[row][col + 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else // 4.3.1.3 colAfterNum > 1
                    {

                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    
                }
                else if (colBeforeNum == 1) // 4.3.2 colBeforeNum == 1
                {
                    // should not be same as tileStr
                    if (tileStr == obj[row][col - 1])
                    {
                        flag = 0;
                        return flag;
                    }
                    if (colAfterNum == 0) // 4.3.2.1 colAfterNum == 0
                    {
                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col - 1][0] || tileStr[1] == obj[row][col - 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1)  // 4.3.2.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        if (obj[row][col - 1] == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }
                        else
                        {
                            if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                            {
                                flag = 1;
                            }
                            else
                            {
                                flag = 0;
                                return 0;
                            }
                        }
                    }
                    else    // 4.3.2.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (obj[row][col - 1] == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
                else    // 4.3.3 colBeforeNum > 1
                {
                    // should not be same as tileStr
                    for (int i = 1; i <= colBeforeNum; i++)
                    {
                        if (tileStr == obj[row][col - i])
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    if (colAfterNum == 0)   // 4.3.3.1 colAfterNum == 0
                    {
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1) // 4.3.3.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            if(obj[row][col+1] == obj[row][col-i]){
                                flag = 0;
                                return flag;
                            }
                        }


                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                    else    // 4.3.3.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            for(int j=1;j<colAfterNum;j++){
                                if(obj[row][col+j] == obj[row][col-i]){
                                    flag = 0;
                                    return flag;
                                }
                            }
                        }

                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }

            }
            
        }
        else // 5. rowUpNUm > 1
        {
            // should not be same as tileStr
            for(int i=1;i<=rowUpNUm;i++){
                if(tileStr == obj[row-i][col]){
                    flag = 0;
                    return 0;
                }
            }

            if(!((tileStr[0] == obj[row+1][col][0] && tileStr[0] == obj[row+2][col][0]) || (tileStr[1] == obj[row+1][col][1] && tileStr[1] == obj[row+2][col][1]))){
                flag = 0;
                return flag;
            }



            if (rowDownNUm == 0) // 5.1 rowDownNUm == 0
            {
                if (colBeforeNum == 0)  // 5.1.1 colBeforeNum == 0
                {
                    if (colAfterNum == 0)   // 5.1.1.1 colAfterNum == 0
                    {
                        flag = 1;
                        
                    }
                    else if (colAfterNum == 1)  // 5.1.1.2 colAfterNum == 1
                    {
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col + 1][0] || tileStr[1] == obj[row][col + 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else // 5.1.1.3 colAfterNum > 1
                    {

                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    
                }
                else if (colBeforeNum == 1) // 5.1.2 colBeforeNum == 1
                {
                    // should not be same as tileStr
                    if (tileStr == obj[row][col - 1])
                    {
                        flag = 0;
                        return flag;
                    }
                    if (colAfterNum == 0) // 5.1.2.1 colAfterNum == 0
                    {
                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col - 1][0] || tileStr[1] == obj[row][col - 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1)  // 5.1.2.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        if (obj[row][col - 1] == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }
                        else
                        {
                            if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                            {
                                flag = 1;
                            }
                            else
                            {
                                flag = 0;
                                return 0;
                            }
                        }
                    }
                    else    // 5.1.2.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (obj[row][col - 1] == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
                else    // 5.1.3 colBeforeNum > 1
                {
                    // should not be same as tileStr
                    for (int i = 1; i <= colBeforeNum; i++)
                    {
                        if (tileStr == obj[row][col - i])
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    if (colAfterNum == 0)   // 5.1.3.1 colAfterNum == 0
                    {
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1) // 5.1.3.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            if(obj[row][col+1] == obj[row][col-i]){
                                flag = 0;
                                return flag;
                            }
                        }


                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                    else    // 5.1.3.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            for(int j=1;j<colAfterNum;j++){
                                if(obj[row][col+j] == obj[row][col-i]){
                                    flag = 0;
                                    return flag;
                                }
                            }
                        }

                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }

            }else if(rowDownNUm == 1){ // 5.2 rowDownNUm == 1

                // should not be same as tileStr
                if(tileStr == obj[row+1][col]){
                    flag = 0;
                    return flag;
                }

                // row or col should not has same tile
                for(int i=1;i<rowUpNUm;i++){
                    if(obj[row-i][col] == obj[row+1][col]){
                        flag = 0;
                        return flag;
                }
                }

                if(!((tileStr[0] == obj[row-1][col][0] && tileStr[0] == obj[row-2][col][0] && tileStr[0] == obj[row+1][col][0]) || (tileStr[1] == obj[row-1][col][1] && tileStr[1] == obj[row-2][col][1] && tileStr[1] == obj[row+1][col][1]))){
                    flag = 0;
                    return flag;
                }

                if (colBeforeNum == 0)  // 5.2.1 colBeforeNum == 0
                {
                    if (colAfterNum == 0)   // 5.2.1.1 colAfterNum == 0
                    {
                        flag = 1;
                        
                    }
                    else if (colAfterNum == 1)  // 5.2.1.2 colAfterNum == 1
                    {
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col + 1][0] || tileStr[1] == obj[row][col + 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else // 5.2.1.3 colAfterNum > 1
                    {

                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    
                }
                else if (colBeforeNum == 1) // 5.2.2 colBeforeNum == 1
                {
                    // should not be same as tileStr
                    if (tileStr == obj[row][col - 1])
                    {
                        flag = 0;
                        return flag;
                    }
                    if (colAfterNum == 0) // 5.2.2.1 colAfterNum == 0
                    {
                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col - 1][0] || tileStr[1] == obj[row][col - 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1)  // 5.2.2.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        if (obj[row][col - 1] == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }
                        else
                        {
                            if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                            {
                                flag = 1;
                            }
                            else
                            {
                                flag = 0;
                                return 0;
                            }
                        }
                    }
                    else    // 5.2.2.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (obj[row][col - 1] == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
                else    // 5.2.3 colBeforeNum > 1
                {
                    // should not be same as tileStr
                    for (int i = 1; i <= colBeforeNum; i++)
                    {
                        if (tileStr == obj[row][col - i])
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    if (colAfterNum == 0)   // 5.2.3.1 colAfterNum == 0
                    {
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1) // 5.2.3.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            if(obj[row][col+1] == obj[row][col-i]){
                                flag = 0;
                                return flag;
                            }
                        }


                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                    else    // 5.2.3.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            for(int j=1;j<colAfterNum;j++){
                                if(obj[row][col+j] == obj[row][col-i]){
                                    flag = 0;
                                    return flag;
                                }
                            }
                        }

                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }


            }else{  // 5.3 rowDownNUm > 1

                // should not be same as tileStr
                for(int i=1;i<=rowDownNUm;i++){
                    if(tileStr == obj[row+i][col]){
                        flag = 0;
                        return flag;
                    }
                }

                // row or col should not be same as tileStr
                for(int i=1;i<=rowUpNUm;i++){
                    for(int j=1;j<=rowDownNUm;j++){
                        if(obj[row-i][col] == obj[row+j][col]){
                            flag = 0;
                            return flag;
                        }
                    }
                }


                if(!((tileStr[0] == obj[row-1][col][0] && tileStr[0] == obj[row-2][col][0] && tileStr[0] == obj[row+1][col][0] && tileStr[0] == obj[row+2][col][0]) || (tileStr[1] == obj[row-1][col][1] && tileStr[1] == obj[row-2][col][1] && tileStr[1] == obj[row+1][col][1] && tileStr[1] == obj[row+2][col][1]))){
                    flag = 0;
                    return flag;
                }

                if (colBeforeNum == 0)  // 5.3.1 colBeforeNum == 0
                {
                    if (colAfterNum == 0)   // 5.3.1.1 colAfterNum == 0
                    {
                        flag = 1;
                        
                    }
                    else if (colAfterNum == 1)  // 5.3.1.2 colAfterNum == 1
                    {
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col + 1][0] || tileStr[1] == obj[row][col + 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else // 5.3.1.3 colAfterNum > 1
                    {

                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    
                }
                else if (colBeforeNum == 1) // 5.3.2 colBeforeNum == 1
                {
                    // should not be same as tileStr
                    if (tileStr == obj[row][col - 1])
                    {
                        flag = 0;
                        return flag;
                    }
                    if (colAfterNum == 0) // 5.3.2.1 colAfterNum == 0
                    {
                        // colour or shape is the same
                        if (tileStr[0] == obj[row][col - 1][0] || tileStr[1] == obj[row][col - 1][1])
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1)  // 5.3.2.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        if (obj[row][col - 1] == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }
                        else
                        {
                            if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                            {
                                flag = 1;
                            }
                            else
                            {
                                flag = 0;
                                return 0;
                            }
                        }
                    }
                    else    // 5.3.2.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (obj[row][col - 1] == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }
                else    // 5.3.3 colBeforeNum > 1
                {
                    // should not be same as tileStr
                    for (int i = 1; i <= colBeforeNum; i++)
                    {
                        if (tileStr == obj[row][col - i])
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    if (colAfterNum == 0)   // 5.3.3.1 colAfterNum == 0
                    {
                        if ((tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col + 1][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return flag;
                        }
                    }
                    else if (colAfterNum == 1) // 5.3.3.2 colAfterNum == 1
                    {
                        // should not be same as tileStr
                        if (tileStr == obj[row][col + 1])
                        {
                            flag = 0;
                            return flag;
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            if(obj[row][col+1] == obj[row][col-i]){
                                flag = 0;
                                return flag;
                            }
                        }


                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                    else    // 5.3.3.3 colAfterNum > 1
                    {
                        // should not be same as tileStr
                        for (int i = 1; i <= colAfterNum; i++)
                        {
                            if (tileStr == obj[row][col + i])
                            {
                                flag = 0;
                                return flag;
                            }
                        }

                        // row or col should not has same tile
                        for(int i=1;i<=colBeforeNum;i++){
                            for(int j=1;j<colAfterNum;j++){
                                if(obj[row][col+j] == obj[row][col-i]){
                                    flag = 0;
                                    return flag;
                                }
                            }
                        }

                        if ((tileStr[0] == obj[row][col + 1][0] && tileStr[0] == obj[row][col + 2][0] && tileStr[0] == obj[row][col - 1][0] && tileStr[0] == obj[row][col - 2][0]) || (tileStr[1] == obj[row][col + 1][1] && tileStr[1] == obj[row][col + 2][1] && tileStr[1] == obj[row][col - 1][1] && tileStr[1] == obj[row][col - 2][1]))
                        {
                            flag = 1;
                        }
                        else
                        {
                            flag = 0;
                            return 0;
                        }
                    }
                }

            }
        }
    }

    return flag;
}

// 14. make score after palyer place tile
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
    for (int colAfter = col + 1; colAfter < (int)obj[0].size(); colAfter++)
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
    for (int rowDown = row + 1; rowDown < (int)obj.size(); rowDown++)
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

// 15. add a tile from bag to hand after player place a tile to board
int addHand(LinkedList& linkedList, LinkedList& hand)
{

    int flag;
    Node* head;

    head = linkedList.getHead();

    if (linkedList.size() < 1)
    {
        flag = 0;
    }
    else
    {
        Tile* tileNew = new Tile();
        tileNew->colour = head->next->tile->colour;
        tileNew->shape = head->next->tile->shape;

        Node* nodeNew = new Node(tileNew, NULL);

        hand.insert(nodeNew);
        linkedList.cut(head->next->tile->colour, head->next->tile->shape);
        head = linkedList.getHead();
        flag = 1;
    }
    return flag;
}

// 16. print the board when a player's turn is over
void printVector(vector<vector<string> > obj)
{
    for (int j = 0; j < (int)obj[0].size(); j++)
    {
        if (j == 0)
        {
            cout << "   " << j;
        }
        else if (j < 10)
        {
            cout << "  " << j;
        }
        else
        {
            cout << " " << j;
        }
    }
    cout << endl;

    for (int j = 0; j < (int)obj[0].size(); j++)
    {
        if (j == 0)
        {
            cout << "  ----";
        }
        else
        {
            cout << "----";
        }
    }
    cout << endl;

    for (int i = 0; i < (int)obj.size(); i++)
    {
        switch (i)
        {
        case 0:
            cout << 'A' << " |";
            break;
        case 1:
            cout << 'B' << " |";
            break;
        case 2:
            cout << 'C' << " |";
            break;
        case 3:
            cout << 'D' << " |";
            break;
        case 4:
            cout << 'E' << " |";
            break;
        case 5:
            cout << 'F' << " |";
            break;
        case 6:
            cout << 'G' << " |";
            break;
        case 7:
            cout << 'H' << " |";
            break;
        case 8:
            cout << 'I' << " |";
            break;
        case 9:
            cout << 'J' << " |";
            break;
        case 10:
            cout << 'K' << " |";
            break;
        case 11:
            cout << 'L' << " |";
            break;
        case 12:
            cout << 'M' << " |";
            break;
        case 13:
            cout << 'N' << " |";
            break;
        case 14:
            cout << 'O' << " |";
            break;
        case 15:
            cout << 'P' << " |";
            break;
        case 16:
            cout << 'Q' << " |";
            break;
        case 17:
            cout << 'R' << " |";
            break;
        case 18:
            cout << 'S' << " |";
            break;
        case 19:
            cout << 'T' << " |";
            break;
        case 20:
            cout << 'U' << " |";
            break;
        case 21:
            cout << 'V' << " |";
            break;
        case 22:
            cout << 'W' << " |";
            break;
        case 23:
            cout << 'X' << " |";
            break;
        case 24:
            cout << 'Y' << " |";
            break;
        case 25:
            cout << 'Z' << " |";
            break;
        default:
            break;
        }
        for (int j = 0; j < (int)obj[i].size(); j++)
        {
            cout << obj[i][j] << "|";
        }
        cout << "\n";
    }
}

// 17. welcome
void welcome()
{
    cout << "Welcome to Qwirkle!" << endl;
    cout << "-------------------" << endl;
}

// 18. main menu
void mainMenu()
{

    cout << "Menu" << endl;
    cout << "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Credits (Show student information)" << endl;
    cout << "4. Quit" << endl;
    cout << endl;
}

// 19. credit(show info. of teamer)
void showStudentInfo()
{
    Student student[3];
    student[0].name = "HenghaoLi";
    student[0].sudentId = "s3798993";
    student[0].email = "s3798993@student.rmit.edu.au";

    student[1].name = "JunyuLi";
    student[1].sudentId = "s3706335";
    student[1].email = "s3706335@student.rmit.edu.au";

    student[2].name = "Zongzhouwang";
    student[2].sudentId = "s3688540";
    student[2].email = "s3688540@student.rmit.edu.au";

    cout << endl;
    cout << "----------------------------------" << endl;
    cout << "Name:" << student[0].name << endl;
    cout << "Student ID:" << student[0].sudentId << endl;
    cout << "Email:" << student[0].email << endl;
    cout << endl;

    cout << "Name:" << student[1].name << endl;
    cout << "Student ID:" << student[1].sudentId << endl;
    cout << "Email:" << student[1].email << endl;
    cout << endl;

    cout << "Name:" << student[2].name << endl;
    cout << "Student ID:" << student[2].sudentId << endl;
    cout << "Email:" << student[2].email << endl;
    cout << "----------------------------------" << endl;

    cout << endl;
}

// 20. check palyer name legal or not,if legal return 1,else return 0
int checkPlayName(string name)
{
    int flag = 1;
    for (int i = 0; i < (int)name.length(); i++)
    {
        if (isupper(name[i]) == 0)
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

int checkSaveLegal(vector<string> result)
{
    if (result.size() == 2 && result[0] == "save")
        return 0;
    return 1;
}

void winner(Player player1, Player player2)
{
    cout << "Game over" << endl;
    cout << "Score for " << player1.name << ": " << player1.score << endl;
    cout << "Score for " << player2.name << ": " << player2.score << endl;
    if (player1.score > player2.score) {
        cout << "Player " << player1.name << " won!" << endl;
    }
    else if (player1.score == player2.score) {
        cout << "There is no winner,as the two palyers' have the same score!!!" << endl;
    }
    else {
        cout << "Player " << player2.name << " won!" << endl;
    }
    cout << "Goodbye" << endl;
}

void newGame(LinkedList& bagLinkedList, Player& player1, Player& player2, vector<vector<string> >& boardVector)
{
    boardVector = initBoard();
    vector<int> randArr = initRandObj();
    bagLinkedList = initBag(randArr);
    player1.hand = initHand(bagLinkedList);
    player2.hand = initHand(bagLinkedList);

    //bool turnFlag = true;

    string stdinstr;

    cout << endl;
    cout << "Enter a name for player 1 (uppercase characters only)" << endl;
    do
    {
        cout << ">";
        stdinstr = readStdin();
        if (checkPlayName(stdinstr) == 0)
        {
            cout << "inlegal name" << endl;
        }
        else
        {
            player1.name = stdinstr;
        }

    } while (checkPlayName(stdinstr) == 0);

    cout << endl;
    cout << "Enter a name for player 2 (uppercase characters only)" << endl;
    do
    {
        cout << ">";
        stdinstr = readStdin();
        if (checkPlayName(stdinstr) == 0)
        {
            cout << "inlegal name" << endl;
        }
        else
        {
            player2.name = stdinstr;
        }

    } while (checkPlayName(stdinstr) == 0);

    cout << endl;
    cout << "Let's Play!" << endl;
    play(bagLinkedList, player1, player2, boardVector, true);
}

void loadGame(LinkedList& bagLinkedList, Player& player1, Player& player2, vector<vector<string> >& boardVector)
{
    GameFile gamefile;
    int status;
    cout << "Enter the filename from which load a game" << endl;

    do
    {
        cout << ">";
        string stdstr = readStdin();
        status = gamefile.loadGame(stdstr);
        if (status == -1)
            cout << "File doesn't exist,please enter filename again.." << endl; 
        else if (status == 1)
            cout << "data format error,please enter filename again.." << endl; 
    } while (status != 0);
    bool turnFlag = gamefile.player[0].turn ? true : false;
    bagLinkedList = gamefile.bagLinkedList;
    player1 = gamefile.player[0];
    player2 = gamefile.player[1];
    boardVector = gamefile.boardState;
    cout << "Qwirkle game successfully loaded" << endl;
    play(bagLinkedList, player1, player2, boardVector, turnFlag);
}

void saveGame(LinkedList& bagLinkedList, Player& player1, Player& player2, vector<vector<string>>& boardVector,string filename)
{
    GameFile gamefile;
    gamefile.player[0] = player1;
    gamefile.player[1] = player2;
    gamefile.boardHeight = boardVector.size();
    gamefile.boardWidth = boardVector[0].size();
    gamefile.boardState = boardVector;
    gamefile.currentPlayer = player1.turn ? player1.name : player2.name;
    gamefile.bagLinkedList = bagLinkedList;
    gamefile.saveGame(filename);
    cout << "Game successfully saved" << endl;

}

void play(LinkedList& bagLinkedList, Player& player1, Player& player2, vector<vector<string> >& boardVector, bool turnFlag)
{
    bool checkFlag;
    bool overFlag = false;
    vector<string> result;

    string stdinstr;
    do
    {
        if (turnFlag)
        {
            cout << endl;
            cout << player1.name << ",it's your turn" << endl;
            cout << "Score for " << player1.name << ":" << player1.score << endl;
            cout << "Score for " << player2.name << ":" << player2.score << endl;
            cout << "The number of Tiles in Bag is:" << bagLinkedList.size() << endl;
            printVector(boardVector);
            player1.hand.printHand();
            do
            {
                cout << ">";
                stdinstr = readStdin();
                if (stdinstr == "EOF")
                {
                    overFlag = true;
                    break;
                }
                result = charSplit(stdinstr);

                if (checkSaveLegal(result) == 0)
                {
                    player1.turn = turnFlag;
                    player2.turn = !turnFlag;
                    saveGame(bagLinkedList, player1, player2, boardVector, result[1]);
                    continue;
                }

                checkFlag = checkReplaceLegal(result, player1.hand) == 5 || checkPlaceLegal(result, player1.hand) == 8;
                if (checkReplaceLegal(result, player1.hand) == 5)
                {
                    //
                    if (replaceHand(result, bagLinkedList, player1.hand) == 0)
                    {
                        cout << "repalce Tile fail! Bag is empty." << endl;
                        checkFlag = false;
                    }
                }
                else if (checkPlaceLegal(result, player1.hand) == 8)
                {
                    if (palceAtVector(result.at(1), result.at(3), boardVector, player1) == 1)
                    {
                        player1.hand.cut(result.at(1)[0], atoi(&result.at(1)[1]));
                        if (addHand(bagLinkedList, player1.hand) == 0)
                        {
                            if (player1.hand.size() < 1) {
                                //
                                winner(player1, player2);
                                overFlag = true;
                                break;
                            }
                            else {
                                cout << "Attention!!! Bag is empty." << endl;
                            }
                        }
                    }
                    else
                    {
                        cout << "inlegal place position input!" << endl;
                        checkFlag = false;
                    }
                }
                else
                {
                    cout << "inlegal input!" << endl;
                }
            } while (!checkFlag);
            turnFlag = false;
        }
        else
        {
            cout << endl;
            cout << player2.name << ",it's your turn" << endl;
            cout << "Score for " << player1.name << ":" << player1.score << endl;
            cout << "Score for " << player2.name << ":" << player2.score << endl;
            cout << "The number of Tiles in Bag is:" << bagLinkedList.size() << endl;
            printVector(boardVector);
            player2.hand.printHand();
            do
            {
                cout << ">";
                stdinstr = readStdin();
                if (stdinstr == "EOF")
                {
                    overFlag = true;
                    break;

                }
                result = charSplit(stdinstr);

                if (checkSaveLegal(result) == 0)
                {
                    player1.turn = turnFlag;
                    player2.turn = !turnFlag;
                    saveGame(bagLinkedList, player1, player2, boardVector, result[1]);
                    continue;
                }

                checkFlag = checkReplaceLegal(result, player2.hand) == 5 || checkPlaceLegal(result, player2.hand) == 8;
                if (checkReplaceLegal(result, player2.hand) == 5)
                {
                    //
                    if (replaceHand(result, bagLinkedList, player2.hand) == 0)
                    {
                        cout << "repalce Tile fail! Bag is empty." << endl;
                        checkFlag = false;
                    }
                }
                else if (checkPlaceLegal(result, player2.hand) == 8)
                {
                    if (palceAtVector(result.at(1), result.at(3), boardVector, player2) == 1)
                    {
                        player2.hand.cut(result.at(1)[0], atoi(&result.at(1)[1]));
                        if (addHand(bagLinkedList, player2.hand) == 0)
                        {
                            if (player2.hand.size() < 1) {
                                //
                                winner(player1, player2);
                                overFlag = true;
                                break;
                            }
                            else {
                                cout << "Attention!!! Bag is empty." << endl;
                            }
                        }
                    }
                    else
                    {
                        cout << "inlegal place position input!" << endl;
                        checkFlag = false;
                    }
                }
                else
                {
                    cout << "inlegal input!" << endl;
                }
            } while (!checkFlag);
            turnFlag = true;
        }
    } while (!overFlag);
}
