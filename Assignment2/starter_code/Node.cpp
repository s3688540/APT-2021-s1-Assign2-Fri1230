
#include "Node.h"
#include <iostream>

Node::Node(){

}

Node::Node(Tile* tile, Node* next)
{
   // TODO
   this->tile = tile;
   this->next = next;
}

Node::Node(Node& other)
{
   // TODO
}
