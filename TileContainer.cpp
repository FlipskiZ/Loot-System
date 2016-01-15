#include "TileContainer.h"
#include "Engine.h"

TileContainer::TileContainer(){
    if(rand() % 2 == 0){
        containedItems.push_back(lootSystem.createWeapon(currentLevel));
    }else{
        containedItems.push_back(lootSystem.createArmor(currentLevel));
    }
}

vector<int> TileContainer::getContainedItems(){
    return this->containedItems;
}

void TileContainer::update(){
    if(this->entityWorldPosition != worldPosition && this->usesWorldPosition){
        return;
    }
    this->updateCenter();
}

void TileContainer::draw(){
    if(this->entityWorldPosition != worldPosition && this->usesWorldPosition){
        return;
    }
    al_draw_filled_rectangle(this->posX+cameraOffsetX, this->posY+cameraOffsetY, this->posX+this->width+cameraOffsetX, this->posY+this->height+cameraOffsetY, al_map_rgb(180, 110, 20));
}
