
#include "LinkedList.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

LinkedList::LinkedList()
{
   // TODO
}

LinkedList::~LinkedList()
{
}

void LinkedList::create()
{
   //
   head = new Node();
   head->next = NULL;
}

void LinkedList::insert(Node *node)
{
   //
   /*
      1.the inserted node at the head of the linkedlist

   node->next = head->next;
   head->next = node;
   */

   //  2.the inserted node at the the tail of the linkedlist

   Node *tmp = head;
   while (tmp->next)
   {
      tmp = tmp->next;
   }
   node->next = NULL;
   tmp->next = node;
}

void LinkedList::print()
{
   for (Node *node = head->next; node; node = node->next)
   {
      std::cout << node->tile->colour << node->tile->shape << std::endl;
   }
}

void LinkedList::printHand()
{
   cout << "Your hand is" << endl;
   for (Node *node = head->next; node; node = node->next)
   {
       if (node == head->next)
           std::cout << node->tile->colour << node->tile->shape;
       else
           std::cout << "," << node->tile->colour << node->tile->shape;
   }
   cout << endl;
}
Node *LinkedList::find(Colour colour, Shape shape)
{
   Node *node = head;
   for (; node; node = node->next)
   {
      if (node->next && node->next->tile->colour == colour && node->next->tile->shape == shape)
      {
         break;
      }
   }
   return node;
}

void LinkedList::replace(Node *oldNode, Node *newNode)
{
   Node *preNode = find(oldNode->tile->colour, oldNode->tile->shape);
   Node *tmp = preNode->next;
   newNode->next = tmp->next;
   preNode->next = newNode;
   delete tmp;
}

void LinkedList::replace(Colour colourOld, Shape shapeOld, Node *newNode)
{
   cut(colourOld, shapeOld);
   insert(newNode);
}

void LinkedList::replace(Colour colourOld, Shape shapeOld, Colour colourNew, Shape shapeNew)
{
   Tile *tile = new Tile();
   Node *node = NULL;
   tile->colour = colourNew;
   tile->shape = shapeNew;
   Node *newNode = new Node(tile, node);
   cut(colourOld, shapeOld);
   insert(newNode);
}

void LinkedList::cut(Colour colour, Shape shape)
{
   Node *preNode = find(colour, shape); //use preNode to contain the node before node
   Node *tmp = preNode->next;
   preNode->next = tmp->next;
   delete tmp;
}

Node* LinkedList::getHead()
{
   return head;
}

int LinkedList::size()
{
   int length = 0;
   Node* node = head;
   while(node->next){
      node = node->next;
      length += 1;
   }
   return length;
} 