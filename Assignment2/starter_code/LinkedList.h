
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "TileCodes.h"
#include "Tile.h"
#include "Node.h"

class LinkedList {
public:

   LinkedList();
   ~LinkedList();

   void create();//creat a head pointer node
   void insert(Node *node);//insert a node
   void cut(Colour colour, Shape shape);//delete a appointed node
   void replace(Node* oldNode,Node* newNode);// replace oldNode by newNode
   void replace(Colour colourOld,Shape shapeOld,Node *newNode);
   void replace(Colour colourOld,Shape shapeOld,Colour colourNew,Shape shapeNew);
   void print();//show list
   void printHand();//show Hand
   //void clear();
   Node* find(Colour colour,Shape shape); 
   Node* getHead();
   int size(); 


private:
   Node* head;
};

#endif // ASSIGN2_LINKEDLIST_H
