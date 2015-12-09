#include "LivingPlayer.h"
#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

LivingPlayer::LivingPlayer(){
    maxInventorySpace = 0;
    playerLevel = 1;
    equippedWeapon = 0;
    fireRateHelper = 0;
}

int LivingPlayer::getInventoryUsed(){
    int itemCount = 0;
    for(int i = 0; i < this->maxInventorySpace; i++){
        if(this->inventoryItemIDs[i] != -1){
            itemCount++;
        }
    }
    return itemCount;
}

void LivingPlayer::setMaxInventorySpace(int maxInventorySpace){
    this->maxInventorySpace = maxInventorySpace;
    this->inventoryItemIDs.resize(maxInventorySpace, -1);
}

void LivingPlayer::addItemToInventory(int itemID){
    for(int i = 0; i < this->maxInventorySpace; i++){
        if(this->inventoryItemIDs[i] == -1){
            this->inventoryItemIDs[i] = itemID;
            return;
        }
    }
}

void LivingPlayer::removeItemFromInventory(int itemID){
    for(int i = 0; i < this->maxInventorySpace; i++){
        if(this->inventoryItemIDs[i] == itemID){
            this->inventoryItemIDs[i] = -1;
            return;
        }
    }
}
void LivingPlayer::equipWeapon(int weaponId){
    this->equippedWeapon = weaponId;
}
void LivingPlayer::clearInventory(){
    for(int i = 0; i < this->inventoryItemIDs.size(); i++){
        if(inventoryItemIDs[i] != -1 && itemList[inventoryItemIDs[i]] != NULL){
            itemList[inventoryItemIDs[i]]->setActive(false);
        }else{
            break;
        }
    }
    fill(this->inventoryItemIDs.begin(), this->inventoryItemIDs.end(), -1);
}

void LivingPlayer::takeDamage(double damage, bool crit){
    //Player damage
}

void LivingPlayer::fireWeapon(){
    if(this->fireRateHelper >= itemList[this->equippedWeapon]->getItemStat(weaponFireRate)){
        double width = 6, height = 6, movementSpeed = itemList[this->equippedWeapon]->getItemStat(weaponShotSpeed), sheetColums = 1, sheetRows = 1, animationSpeed = 0;
        double startAngle = this->angle, angleVariation = itemList[this->equippedWeapon]->getItemStat(weaponAccuracy), maxAngle = 25.5*toRadians, angleVariationStart = -maxAngle/2;
        double amountBullets = itemList[this->equippedWeapon]->getItemSpecial(weaponAdditionalBullets)+1;
        bool fireRandLast = false;

        if(amountBullets > 1){
            for(double i = amountBullets; i > 0; i--){
                if(i > 0 && i < 1){
                    if(randDouble() > i){
                        amountBullets = ceil(amountBullets); //Round up if another bullet is to be shot
                    }else{
                        amountBullets = trunc(amountBullets); //Round down if another bullet is not to be shot
                    }
                }
            }

            double angleConeStart = startAngle-maxAngle/2;
            double angleSlice = maxAngle/(amountBullets-1);

            for(int i = 0; i < amountBullets; i++){
                unique_ptr<MissileBullet> newBullet(new MissileBullet());
                newBullet->setPos(this->centerX, this->centerY);
                newBullet->setDimensions(width, height);
                newBullet->setMovementSpeed(movementSpeed);
                newBullet->setAngle(angleConeStart+angleSlice*i+randDouble(angleVariationStart, angleVariationStart+maxAngle)*(1-angleVariation));
                newBullet->setMissileStats(itemList[this->equippedWeapon]->getItemStats());
                newBullet->setMissileSpecials(itemList[this->equippedWeapon]->getItemSpecials());
                newBullet->setSheetDimensions(sheetColums, sheetRows, width, height);
                newBullet->setAnimationSpeed(animationSpeed);
                newBullet->setBitmap(bulletImage);
                addBulletToList(move(newBullet));
            }
        }else{
            unique_ptr<MissileBullet> newBullet(new MissileBullet());
            newBullet->setPos(this->centerX, this->centerY);
            newBullet->setDimensions(width, height);
            newBullet->setMovementSpeed(movementSpeed);
            newBullet->setAngle(startAngle+randDouble(angleVariationStart, angleVariationStart+maxAngle)*(1-angleVariation));
            newBullet->setMissileStats(itemList[this->equippedWeapon]->getItemStats());
            newBullet->setMissileSpecials(itemList[this->equippedWeapon]->getItemSpecials());
            newBullet->setSheetDimensions(sheetColums, sheetRows, width, height);
            newBullet->setAnimationSpeed(animationSpeed);
            newBullet->setBitmap(bulletImage);
            addBulletToList(move(newBullet));
        }
        this->fireRateHelper = 0;
    }
}

void LivingPlayer::update(){
    double deltaX_l = this->getDelta(0), deltaY_l = this->getDelta(1);

    double loopI = ceil(this->movementSpeed*deltaTime/this->width);

    bool colX = false, colY = false;

    this->livingCollisions.clear();

    collisionDetection->getPotentialCollisions(this->livingCollisions, this->posX, this->posY, this->width, this->height);

    for(double i = 0; i < loopI; i++){
        colX = false, colY = false;
        for(int j = 0; j < livingCollisions.size() && (!colX || !colY); j++){
            if(livingList[livingCollisions[j]] != NULL && livingList[livingCollisions[j]]->getActive()){
                if(checkCollision(this->posX + deltaX_l/loopI, this->posY, livingList[livingCollisions[j]]->getPosition(0), livingList[livingCollisions[j]]->getPosition(1),
                    this->width, this->height, livingList[livingCollisions[j]]->getDimension(0), livingList[livingCollisions[j]]->getDimension(1))){
                    colX = true;
                }
                if(checkCollision(this->posX, this->posY + deltaY_l/loopI, livingList[livingCollisions[j]]->getPosition(0), livingList[livingCollisions[j]]->getPosition(1),
                    this->width, this->height, livingList[livingCollisions[j]]->getDimension(0), livingList[livingCollisions[j]]->getDimension(1))){
                    colY = true;
                }
            }
        }

        if(!isPassable(this->posX + deltaX_l/loopI, this->posY, this->width, this->height)){
            colX = true;
        }

        if(!isPassable(this->posX, this->posY + deltaY_l/loopI , this->width, this->height)){
            colY = true;
        }

        if(!colX){
            this->posX += deltaX_l/loopI;
        }

        if(!colY){
            this->posY += deltaY_l/loopI;
        }
    }

    this->updateCenter();

    this->updateAnimation();

    this->setDeltaX(0);
    this->setDeltaY(0);

    this->fireRateHelper += deltaTime;
}

void LivingPlayer::draw(){
    al_draw_rotated_bitmap(this->frameImage, this->width/2, this->height/2, this->posX+this->width/2+cameraOffsetX, this->posY+this->height/2+cameraOffsetY, this->angle, NULL);
}
