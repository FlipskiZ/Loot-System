#include "SpecialTile.h"
#include "Engine.h"

SpecialTile::SpecialTile(){
    this->tilePassable = false;
}

void SpecialTile::createImpassableSpace(){
    mapArray[trunc(this->posX/tileSize)][trunc(this->posY/tileSize)] = this->tileType;
}

void SpecialTile::setPassable(bool passable){
    this->tilePassable = passable;
}

void SpecialTile::setTileType(int tileType){
    this->tileType = tileType;
}

bool SpecialTile::getPassable(){
    return this->tilePassable;
}

int SpecialTile::getTileType(){
    return this->tileType;
}
