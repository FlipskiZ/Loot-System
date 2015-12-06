#include "LivingZombie.h"
#include "Engine.h"

LivingZombie::LivingZombie(){

}

void LivingZombie::takeDamage(double damage, bool crit){
    double movementSpeed = 128, deathTime = 0.5, frictionValue = 0.99;
    int movePattern = 0, fontValue = 0;
    string textValue = dtos(damage);
    ALLEGRO_COLOR colorValue = al_map_rgb(255, 50, 50);

    unique_ptr<EntityParticle> newParticle(new EntityParticle);
    newParticle->setPos(this->centerX, this->posY);
    newParticle->setMovementSpeed(movementSpeed);
    newParticle->setMovePattern(movePattern);
    newParticle->setDeathTime(deathTime);
    newParticle->setFriction(frictionValue);
    newParticle->setTextValue(textValue, fontValue);
    newParticle->setColor(colorValue);
    addParticleToList(move(newParticle));
}

void LivingZombie::update(){
    setAngle(-atan2(this->getCenterPosition()[0] - playerList[0]->getCenterPosition()[0], this->getCenterPosition()[1] - playerList[0]->getCenterPosition()[1]));
    this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
    double deltaX_l = this->getDelta()[0], deltaY_l = this->getDelta()[1];

    double loopI = ceil(this->movementSpeed*deltaTime/this->width);

    bool colX = false, colY = false;

    for(double i = 0; i < loopI; i++){
        colX = false, colY = false;
        for(int j = 0; j < livingList.size() && (!colX || !colY); j++){
            if(livingList[j] != NULL && livingList[j]->getActive() && j != this->entityId){
                if(checkCollision(this->posX + deltaX_l/loopI, this->posY, livingList[j]->getPosition()[0], livingList[j]->getPosition()[1],
                    this->width, this->height, livingList[j]->getDimension()[0], livingList[j]->getDimension()[1])){
                    colX = true;
                }
                if(checkCollision(this->posX, this->posY + deltaY_l/loopI, livingList[j]->getPosition()[0], livingList[j]->getPosition()[1],
                    this->width, this->height, livingList[j]->getDimension()[0], livingList[j]->getDimension()[1])){
                    colY = true;
                }
            }
        }

        if(checkCollision(this->posX + deltaX_l/loopI, this->posY, playerList[0]->getPosition()[0], playerList[0]->getPosition()[1],
            this->width, this->height, playerList[0]->getDimension()[0], playerList[0]->getDimension()[1])){
            colX = true;
        }
        if(checkCollision(this->posX, this->posY + deltaY_l/loopI, playerList[0]->getPosition()[0], playerList[0]->getPosition()[1],
            this->width, this->height, playerList[0]->getDimension()[0], playerList[0]->getDimension()[1])){
            colY = true;
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
}

void LivingZombie::draw(){
    al_draw_rotated_bitmap(this->frameImage, this->width/2, this->height/2, this->posX+this->width/2+cameraOffsetX, this->posY+this->height/2+cameraOffsetY, this->angle, NULL);
}
