#include "CollisionDetection.h"
#include "Engine.h"

CollisionDetection::CollisionDetection(int pLevel, double posX, double posY, double width, double height){
    this->nodeLevel = pLevel;
    this->posX = posX, this->posY = posY;
    this->width = width, this->height = height;
    this->nodes.resize(4);
}

void CollisionDetection::clearNode(){

}
void CollisionDetection::splitNode(){

}
void CollisionDetection::getNodeIndex(){

}
void CollisionDetection::insertObjectInNode(){

}
void CollisionDetection::getPotentialCollisions(){

}
