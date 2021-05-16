#include "GameFile.h"
#include <string>
#include <string.h>
#include <vector>
#include <iostream>
using namespace std;

GameFile::GameFile()
{
   // TODO
}

GameFile::~GameFile()
{
}

vector<string> GameFile::charSplit(const string& str,const char* sep)
{
    vector<string> result;
    char* s = new char[str.length() + 1];
    strcpy(s, str.c_str());
    char* p;
    p = strtok(s, sep);
    while (p)
    {
        result.push_back(p);
        p = strtok(NULL, sep);
    }
    return result;
}

bool GameFile::checkIsNum(string str)
{
    int len = str.size();
    for(int i = 0;i<len;i++)
    {
        if(!isdigit(str[i]))
            return false;
    }
    return true;
}

bool GameFile::colourIsValid(char c)
{
    if(c == RED || c == ORANGE || c == YELLOW || c == GREEN || c == BLUE || c == PURPLE)
        return true;
    return false;
}

bool GameFile::ShapeIsValid(char s)
{
    if(!isdigit(s) || s<'1' || s >'6')
        return false;
    return true;
}

bool GameFile::rowIsValid(char r)
{
    if(r>='A'&&r<='Z')
        return true;
    return false;
}

bool GameFile::boardIsValid(string colstr)
{
    if(!checkIsNum(colstr))
        return false;
    int col = stoi(colstr);
    if(col>25 || col<0)
        return false;
    return true;
}

int GameFile::saveGame(string fileName)
{
     
    int result = 0;
    ofstream out(fileName);
    bool flag = true;

    //save the data for player 1
    out << player[0].name << endl;
    out << player[0].score << endl;
    Node* player_head = player[0].hand.getHead();
    for (Node* node = player_head->next; node; node = node->next)
    {
        if (flag)
        {
            flag = false;
            out << node->tile->colour << node->tile->shape;
        }
        else
        {
            out << "," << node->tile->colour << node->tile->shape;
        }
    }
    out << endl;

    //save the data for player 2
    out << player[1].name << endl;
    out << player[1].score << endl;
    player_head = player[1].hand.getHead();
    flag = true;
    for (Node* node = player_head->next; node; node = node->next)
    {
        if (flag)
        {
            flag = false;
            out << node->tile->colour << node->tile->shape;
        }
        else
        {
            out << "," << node->tile->colour << node->tile->shape;
        }
    }
    out << endl;

    //save the borad size
    out << boardHeight << "," << boardWidth << endl;

    //save the board state
    flag = true;
    for (size_t r = 0; r < boardState.size(); r++)
    {
        for (size_t c = 0; c < boardState.size(); c++)
        {
            if (boardState[r][c] != "  ")
            {   
                char row_char = r + 65;
                string item = boardState[r][c] + '@' + row_char + std::to_string(c);
                if (flag)
                {
                    flag = false;
                    out << item;
                }
                else
                {
                    out << "," << item;
                }
            }
        }
    }
    out << endl;

    //save BagList tile,tile,tile,tile
    Node* bagHand = bagLinkedList.getHead();
    flag = true;
    for (Node* node = player_head->next; node; node = node->next)
    {
        if (flag)
        {
            flag = false;
            out << node->tile->colour << node->tile->shape;
        }
        else
        {
            out << "," << node->tile->colour << node->tile->shape;
        }
    }
    out << endl;

    //save players' name
    if (player[0].turn)
        out << player[0].name;
    else
        out << player[1].name;
    
    result = 1;
    return result;
}

int GameFile::loadGame(string fileName)
{

    ifstream input;
    input.open(fileName);

    //
    if (!input.is_open())
        return -1;
    vector<string> buffers;
    string buffer;

    //load the data from the file
    while(!input.eof())
    {
        getline(input,buffer);
        buffers.push_back(buffer);
    }

    if(buffers.size()!=10)
        return 1;
    
    //load the data for player 1
    buffer = buffers[0];
    player[0].name = buffer;

    buffer = buffers[1];
    if(!checkIsNum(buffer) || stoi(buffer) < 0)
        return 1;
    player[0].score = stoi(buffer);

    buffer = buffers[2];
    vector<string> tiles = charSplit(buffer,",");
    player[0].hand.create();
    for (size_t i = 0; i < tiles.size(); i++)
    {
        Tile* tile = new Tile();
        if(tiles[i].size() != 2 || !colourIsValid(tiles[i][0]) || !ShapeIsValid(tiles[i][1]))
            return 1;
        tile->colour = tiles[i][0];
        tile->shape = tiles[i][1]-48;
        Node* node = new Node(tile, NULL);
        player[0].hand.insert(node);
    }

    //load the data for player 2
    buffer = buffers[3];
    player[1].name = buffer;

    buffer = buffers[4];
    if(!checkIsNum(buffer) || stoi(buffer) < 0)
        return 1;
    player[1].score = stoi(buffer);
    
    buffer = buffers[5];
    tiles = charSplit(buffer,",");
    player[1].hand.create();
    for (size_t i = 0; i < tiles.size(); i++)
    {
        Tile* tile = new Tile();
        if(tiles[i].size() != 2 || !colourIsValid(tiles[i][0]) || !ShapeIsValid(tiles[i][1]))
            return 1;
        tile->colour = tiles[i][0];
        tile->shape = tiles[i][1]-48;
        Node* node = new Node(tile, NULL);
        player[1].hand.insert(node);
    }

    //load board size
    buffer = buffers[6];
    vector<string> shape = charSplit(buffer,",");
    if(shape.size() != 2 || !checkIsNum(shape[0]) || !checkIsNum(shape[1]))
        return 1;
    
    boardHeight = stoi(shape[0]);
    boardWidth = stoi(shape[1]);
    if (boardHeight > 26 || boardHeight < 1 || boardWidth>26 || boardWidth < 1)
        return 1;
    //load board state
    buffer = buffers[7];
    vector<string> items = charSplit(buffer, ",");
    if(items.size()>26*26)
        return 1;
    boardState = vector<vector<string>>(26, vector<string>(26,"  "));
    for (size_t i = 0; i < items.size(); i++)
    {
        vector<string> TileAndPos = charSplit(items[i], "@");

        string colstr = TileAndPos[1].substr(1);

        if(TileAndPos.size() !=2 || !rowIsValid(TileAndPos[1][0]) || !boardIsValid(colstr) || !colourIsValid(TileAndPos[0][0])|| !ShapeIsValid(TileAndPos[0][1]))
            return 1;
        
        int row = TileAndPos[1][0] - 65;
        int col = stoi(colstr);

        boardState[row][col] = TileAndPos[0];
    }

    //load Bag tile,tile,tile,tile,...
    buffer = buffers[8];
    tiles = charSplit(buffer, ",");
    bagLinkedList.create();
    for (size_t i = 0; i < tiles.size(); i++)
    {
        if(tiles[i].size() != 2 || !colourIsValid(tiles[i][0]) || !ShapeIsValid(tiles[i][1]))
            return 1;
        Tile* tile = new Tile();
        tile->colour = tiles[i][0];
        tile->shape = tiles[i][1]-48;
        Node* node = new Node(tile, NULL);
        bagLinkedList.insert(node);
    }

    //load the current player
    buffer = buffers[9];
    if(buffer != player[0].name && buffer != player[1].name)
        return 1;
    currentPlayer = buffer;
    if (buffer == player[0].name)
    {
        player[0].turn = true;
        player[1].turn = false;
    }
    else
    {
        player[0].turn = false;
        player[1].turn = true;
    }
    return 0;
}