#include "CollisionDetection.h"
#include "Engine.h"

CollisionDetection::CollisionDetection(int pLevel, double posX, double posY, double width, double height){
    this->nodeLevel = pLevel;
    this->livingObjects.clear();
    this->posX = posX, this->posY = posY;
    this->width = width, this->height = height;
    this->subNodes.resize(4);
}
//Clear this node
void CollisionDetection::clearNode(){
    this->livingObjects.clear();
    if(this->subNodes[0] != NULL){
        for(int i = 0; i < 4; i++){
            this->subNodes[i]->clearNode();
            this->subNodes[i] = NULL;
        }
    }
}
//Split the node into 4 subnodes
void CollisionDetection::splitNode(){
    double subWidth = this->width / 2;
    double subHeight = this->height / 2;
    double x = this->posX;
    double y = this->posY;

    this->subNodes[0] = unique_ptr<CollisionDetection>(new CollisionDetection(this->nodeLevel+1, x, y, subWidth, subHeight));
    this->subNodes[1] = unique_ptr<CollisionDetection>(new CollisionDetection(this->nodeLevel+1, x + subWidth, y, subWidth, subHeight));
    this->subNodes[2] = unique_ptr<CollisionDetection>(new CollisionDetection(this->nodeLevel+1, x, y + subHeight, subWidth, subHeight));
    this->subNodes[3] = unique_ptr<CollisionDetection>(new CollisionDetection(this->nodeLevel+1, x + subWidth, y + subHeight, subWidth, subHeight));
}
/*Determine which node the object belongs to. -1 means
object cannot completely fit within a child node and is part
of the parent node*/
int CollisionDetection::getNodeIndex(int livingIndex){
    int index = -1;
    double verticalMidpoint = this->posX + this->width / 2;
    double horizontalMidpoint = this->posY + this->height / 2;

    // Object can completely fit within the top quadrants
    bool topQuadrant = (livingList[livingIndex]->getPosition(1) < horizontalMidpoint && livingList[livingIndex]->getPosition(1) + livingList[livingIndex]->getDimension(1) < horizontalMidpoint);
    // Object can completely fit within the bottom quadrants
    bool bottomQuadrant = (livingList[livingIndex]->getPosition(1) > horizontalMidpoint);

    // Object can completely fit within the left quadrants
    if(livingList[livingIndex]->getPosition(0) < verticalMidpoint && livingList[livingIndex]->getPosition(0) + livingList[livingIndex]->getDimension(0) < verticalMidpoint){
        if(topQuadrant){
            index = 0;
        }else if(bottomQuadrant){
            index = 2;
        }
    }
    // Object can completely fit within the right quadrants
    else if(livingList[livingIndex]->getPosition(0) > verticalMidpoint){
        if(topQuadrant){
            index = 1;
        }else if(bottomQuadrant){
            index = 3;
        }
    }
    return index;
}

int CollisionDetection::getNodeIndex(double posX, double posY, double width, double height){
    int index = -1;
    double verticalMidpoint = this->posX + this->width / 2;
    double horizontalMidpoint = this->posY + this->height / 2;

    // Object can completely fit within the top quadrants
    bool topQuadrant = (posY < horizontalMidpoint && posY + height < horizontalMidpoint);
    // Object can completely fit within the bottom quadrants
    bool bottomQuadrant = (posY > horizontalMidpoint);

    // Object can completely fit within the left quadrants
    if(posX < verticalMidpoint && posX + width < verticalMidpoint){
        if(topQuadrant){
            index = 0;
        }else if(bottomQuadrant){
            index = 2;
        }
    }
    // Object can completely fit within the right quadrants
    else if(posX > verticalMidpoint){
        if(topQuadrant){
            index = 1;
        }else if(bottomQuadrant){
            index = 3;
        }
    }
    return index;
}
/*Insert the object into the quadtree. If the node
exceeds the capacity, it will split and add all
objects to their corresponding nodes.*/
void CollisionDetection::insertObjectInNode(int livingIndex){
    if(this->subNodes[0] != NULL){
        int index = this->getNodeIndex(livingIndex);
        if(index != -1){
            this->subNodes[index]->insertObjectInNode(livingIndex);
            return;
        }
    }

    this->livingObjects.push_back(livingIndex);

    if(this->livingObjects.size() > MAX_TREE_OBJECTS && this->nodeLevel < MAX_TREE_LEVELS){
        if(this->subNodes[0] == NULL){
            this->splitNode();
        }
        for(int i = 0; i < this->livingObjects.size(); i++){
            int index = this->getNodeIndex(this->livingObjects[i]);
            if(index != -1){
                this->subNodes[index]->insertObjectInNode(this->livingObjects[i]);
                this->livingObjects.erase(this->livingObjects.begin()+i);
                i--;
            }
        }
    }
}
//Remove a object by value
void CollisionDetection::removeObjectInNode(int livingIndex){
    if(this->subNodes[0] != NULL){
        int index = this->getNodeIndex(livingIndex);
        if(index != -1){
            this->subNodes[index]->removeObjectInNode(livingIndex);
            return;
        }
    }
    this->livingObjects.erase(remove(this->livingObjects.begin(), this->livingObjects.end(), livingIndex), this->livingObjects.end());
}
//Return all objects that could collide with the given object
void CollisionDetection::getPotentialCollisions(vector<int> &returnObjects, int livingIndex){
    int index = this->getNodeIndex(livingIndex);

    if(index != -1 && this->subNodes[0] != NULL){
        this->subNodes[index]->getPotentialCollisions(returnObjects, livingIndex);
    }else if(index == -1 && this->subNodes[0] != NULL){
        this->subNodes[0]->getPotentialCollisions(returnObjects, livingIndex);
        this->subNodes[1]->getPotentialCollisions(returnObjects, livingIndex);
        this->subNodes[2]->getPotentialCollisions(returnObjects, livingIndex);
        this->subNodes[3]->getPotentialCollisions(returnObjects, livingIndex);
    }

    returnObjects.insert(returnObjects.end(), this->livingObjects.begin(), this->livingObjects.end());
}

void CollisionDetection::getPotentialCollisions(vector<int> &returnObjects, double posX, double posY, double width, double height){
    int index = this->getNodeIndex(posX, posY, width, height);

    if(index != -1 && this->subNodes[0] != NULL){
        this->subNodes[index]->getPotentialCollisions(returnObjects, posX, posY, width, height);
    }else if(index == -1 && this->subNodes[0] != NULL){
        this->subNodes[0]->getPotentialCollisions(returnObjects, posX, posY, width, height);
        this->subNodes[1]->getPotentialCollisions(returnObjects, posX, posY, width, height);
        this->subNodes[2]->getPotentialCollisions(returnObjects, posX, posY, width, height);
        this->subNodes[3]->getPotentialCollisions(returnObjects, posX, posY, width, height);
    }

    returnObjects.insert(returnObjects.end(), this->livingObjects.begin(), this->livingObjects.end());
}

void CollisionDetection::draw(){
    if(this->subNodes[0] != NULL){
        for(int i = 0; i < this->subNodes.size(); i++){
            this->subNodes[i]->draw();
        }
    }
    al_draw_rectangle(this->posX+cameraOffsetX, this->posY+cameraOffsetY, this->posX+this->width+cameraOffsetX, this->posY+height+cameraOffsetY, al_map_rgb(0, 0, 0), 3);
}
