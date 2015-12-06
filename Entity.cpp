#include "Entity.h"
#include "Engine.h"

Entity::Entity(){
    this->posX = 0;
    this->posY = 0;
    this->centerX = 0;
    this->centerY = 0;
    this->width = 0;
    this->height = 0;
    this->angle = 0;
    this->entityId = 0;
    this->typeId = 0;
    this->sheetColums = 1;
    this->sheetRows = 1;
    this->frameWidth = 0;
    this->frameHeight = 0;
    this->animationValue = 0;
    this->animationValueHelper = 0;
    this->animationCount = 0;
    this->animationCountHelper = 0;
    this->sheetImage = 0;
    this->frameImage = 0;
    this->untilFinished = false;
    this->nextFrame = false;
    this->rememberAnimationValue = 0;
    this->multiplier = 0;
    this->random = 0;
    this->active = true;
}

double Entity::getPosition(int xory){
    return (xory == 0) ? this->posX : this->posY;

}
double Entity::getCenterPosition(int xory){
    return (xory == 0) ? this->centerX : this->centerY;
}
double Entity::getDimension(int worh){
    return (worh == 0) ? this->width : this->height;
}
double Entity::getMovementSpeed(){
    return this->movementSpeed;
}
double Entity::getDelta(int xory){
    return (xory == 0) ? this->deltaX : this->deltaY;
}
int Entity::getEntityId(){
    return this->entityId;
}
int Entity::getTypeId(){
    return this->typeId;
}
double Entity::getAngle(){
    return this->angle;
}
int Entity::getAnimationValue(){
    return this->animationValue;
}
bool Entity::getActive(){
    return this->active;
}

void Entity::setPos(double x, double y){
    this->posX = x;
    this->posY = y;
    this->updateCenter();
}
void Entity::setDimensions(double width, double height){
    this->width = width;
    this->height = height;
}
void Entity::setMovementSpeed(double movementSpeed){
    this->movementSpeed = movementSpeed;
    this->maxMovementSpeed = movementSpeed;
}
void Entity::setDeltaX(double deltaX){
    this->deltaX = deltaX*deltaTime;
}
void Entity::setDeltaY(double deltaY){
    this->deltaY = deltaY*deltaTime;
}
void Entity::setEntityId(int entityId){
    this->entityId = entityId;
}
void Entity::setTypeId(int typeId){
    this->typeId = typeId;
}
void Entity::setAngle(double angle){
    this->angle = angle;
}
void Entity::setActive(bool active){
    this->active = active;
}
void Entity::setSheetDimensions(int sheetColums, int sheetRows, int frameWidth, int frameHeight){
    this->sheetColums = sheetColums;
    this->sheetRows = sheetRows;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
}
void Entity::setBitmap(ALLEGRO_BITMAP *sheetImage){
    this->sheetImage = sheetImage;
    this->updateAnimation();
}
void Entity::setAnimationSpeed(double animationSpeed){
    this->animationCount = animationSpeed;
}

void Entity::setAnimationValue(int animationValue, bool untilFinished, double multiplier){
    if(untilFinished){
        this->rememberAnimationValue = animationValue;
        this->untilFinished = true;
    }
    this->animationValue = animationValue;
    this->multiplier = multiplier;
}

void Entity::updateCenter(){
    this->centerX = this->posX + this->width/2, this->centerY = this->posY + this->height/2;
}
void Entity::updateAnimation(){

    if(this->untilFinished){
        this->animationValue = this->rememberAnimationValue;
    }

    if(this->animationValue != this->animationValueHelper){
        this->animationCountHelper = this->animationCount;
        this->animationValueHelper = this->animationValue;
        this->frameX = 0;
    }

    if(this->animationValue <= 0){
        this->frameX = 0;
        this->frameY = 0;
        this->animationCountHelper = animationCount;
        al_destroy_bitmap(this->frameImage);
        this->frameImage = al_create_sub_bitmap(this->sheetImage, this->frameX, this->frameY, this->frameWidth, this->frameHeight);
        return;
    }

    if(this->animationCountHelper*this->multiplier >= this->animationCount){
        this->frameY = this->animationValue-1;
        if(this->frameX+1 < this->sheetColums){
            this->frameX++;
        }else{
            this->frameX = 0;
            if(this->untilFinished){
                this->untilFinished = false;
            }
        }

        al_destroy_bitmap(this->frameImage);
        this->frameImage = al_create_sub_bitmap(this->sheetImage, this->frameX*this->frameWidth, this->frameY*this->frameHeight, this->frameWidth, this->frameHeight);
        this->animationCountHelper = 0;

        this->nextFrame = true;
    }else{
        this->nextFrame = false;
    }

    this->animationCountHelper += deltaTime;
}
