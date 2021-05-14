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
        out << player[0].name << endl;
    else
        out << player[1].name << endl;
    
    result = 1;
    return result;
}

GameFile GameFile::loadGame(string fileName)
{
    GameFile gameFile;
    string buffer;

    ifstream input;
    input.open(fileName);
    if (!input.is_open())
        return gameFile;

    //load the data for player 1
    getline(input, buffer);
    gameFile.player[0].name = buffer;
    getline(input, buffer);
    gameFile.player[0].score = stoi(buffer);
    getline(input, buffer);
    vector<string> tiles = charSplit(buffer,",");
    gameFile.player[0].hand.create();
    for (size_t i = 0; i < tiles.size(); i++)
    {
        Tile* tile = new Tile();
        tile->colour = tiles[i][0];
        tile->shape = tiles[i][1]-48;
        Node* node = new Node(tile, NULL);
        gameFile.player[0].hand.insert(node);
    }

    //load the data for player 2
    getline(input, buffer);
    gameFile.player[1].name = buffer;
    getline(input, buffer);
    gameFile.player[1].score = stoi(buffer);
    getline(input, buffer);
    tiles = charSplit(buffer,",");
    gameFile.player[1].hand.create();
    for (size_t i = 0; i < tiles.size(); i++)
    {
        Tile* tile = new Tile();
        tile->colour = tiles[i][0];
        tile->shape = tiles[i][1]-48;
        Node* node = new Node(tile, NULL);
        gameFile.player[1].hand.insert(node);
    }

    //load board size
    getline(input, buffer);
    vector<string> shape = charSplit(buffer,",");
    gameFile.boardHeight = stoi(shape[0]);
    gameFile.boardWidth = stoi(shape[1]);
    
    //load board state
    getline(input, buffer);
    vector<string> items = charSplit(buffer, ",");
    gameFile.boardState = vector<vector<string>>(26, vector<string>(26,"  "));
    for (size_t i = 0; i < items.size(); i++)
    {
        vector<string> TileAndPos = charSplit(items[i], "@");

        int row = TileAndPos[1][0] - 65;
        
        string colstr = TileAndPos[1].substr(1);
        int col = stoi(colstr);

        gameFile.boardState[row][col] = TileAndPos[0];
    }

    //load Bag tile,tile,tile,tile,...
    getline(input, buffer);
    tiles = charSplit(buffer, ",");
    gameFile.bagLinkedList.create();
    for (size_t i = 0; i < tiles.size(); i++)
    {
        Tile* tile = new Tile();
        tile->colour = tiles[i][0];
        tile->shape = tiles[i][1]-48;
        Node* node = new Node(tile, NULL);
        gameFile.bagLinkedList.insert(node);
    }

    //load the current player
    getline(input, buffer);
    gameFile.currentPlayer = buffer;
    if (buffer == gameFile.player[0].name)
    {
        gameFile.player[0].turn = true;
        gameFile.player[1].turn = false;
    }
    else
    {
        gameFile.player[0].turn = false;
        gameFile.player[1].turn = true;
    }
    return gameFile;

}