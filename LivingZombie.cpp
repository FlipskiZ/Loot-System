#include "LivingZombie.h"
#include "Engine.h"

LivingZombie::LivingZombie(){
    this->livingCollisions.clear();
}

void LivingZombie::takeDamage(double damage, bool crit){
    double movementSpeed = 160, deathTime = 0.6, frictionValue = 2, gravityValue = 5, angle = randDouble(-45, 45)*toRadians;
    int movePattern = patternGravity, fontValue = 0;
    string textValue = dtos(damage);
    ALLEGRO_COLOR colorValue = al_map_rgb(255, 50, 50);

    unique_ptr<EntityParticle> newParticle(new EntityParticle);
    newParticle->setPos(this->centerX, this->posY);
    newParticle->setMovementSpeed(movementSpeed);
    newParticle->setMovePattern(movePattern);
    newParticle->setDeltaX(movementSpeed, angle), newParticle->setDeltaY(movementSpeed, angle);
    newParticle->setDeathTime(deathTime);
    newParticle->setFriction(frictionValue);
    newParticle->setGravity(gravityValue);
    newParticle->setTextValue(textValue, fontValue);
    newParticle->setColor(colorValue);
    addParticleToList(move(newParticle));

    this->livingHP -= damage;

    if(this->livingHP <= 0){
        this->setActive(false);
    }
}

void LivingZombie::update(){
    setAngle(-atan2(this->getCenterPosition(0) - playerList[0]->getCenterPosition(0), this->getCenterPosition(1) - playerList[0]->getCenterPosition(1)));
    this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
    double deltaX_l = this->getDelta(0), deltaY_l = this->getDelta(1);

    bool colX = false, colY = false;

    this->livingCollisions.clear();

    collisionDetection->getPotentialCollisions(this->livingCollisions, this->entityId);

    for(int j = 0; j < this->livingCollisions.size(); j++){
        if(livingList[this->livingCollisions[j]] != NULL && livingList[this->livingCollisions[j]]->getActive() && this->livingCollisions[j] != this->entityId){
            if(checkCollision(this->posX + deltaX_l, this->posY, livingList[this->livingCollisions[j]]->getPosition(0), livingList[this->livingCollisions[j]]->getPosition(1),
                this->width, this->height, livingList[this->livingCollisions[j]]->getDimension(0), livingList[this->livingCollisions[j]]->getDimension(1))){
                colX = true;
            }
            if(checkCollision(this->posX, this->posY + deltaY_l, livingList[this->livingCollisions[j]]->getPosition(0), livingList[this->livingCollisions[j]]->getPosition(1),
                this->width, this->height, livingList[this->livingCollisions[j]]->getDimension(0), livingList[this->livingCollisions[j]]->getDimension(1))){
                colY = true;
            }
        }
    }

    /*for(int j = 0; j < livingList.size(); j++){
        if(livingList[j] != NULL && livingList[j]->getActive() && j != this->entityId){
            if(checkCollision(this->posX + deltaX_l, this->posY, livingList[j]->getPosition(0), livingList[j]->getPosition(1),
                this->width, this->height, livingList[j]->getDimension(0), livingList[j]->getDimension(1))){
                colX = true;
            }
            if(checkCollision(this->posX, this->posY + deltaY_l, livingList[j]->getPosition(0), livingList[j]->getPosition(1),
                this->width, this->height, livingList[j]->getDimension(0), livingList[j]->getDimension(1))){
                colY = true;
            }
        }
    }*/

    if(checkCollision(this->posX + deltaX_l, this->posY, playerList[0]->getPosition(0), playerList[0]->getPosition(1),
        this->width, this->height, playerList[0]->getDimension(0), playerList[0]->getDimension(1))){
        colX = true;
    }
    if(checkCollision(this->posX, this->posY + deltaY_l, playerList[0]->getPosition(0), playerList[0]->getPosition(1),
        this->width, this->height, playerList[0]->getDimension(0), playerList[0]->getDimension(1))){
        colY = true;
    }

    if(!isPassable(this->posX + deltaX_l, this->posY, this->width, this->height)){
        colX = true;
    }

    if(!isPassable(this->posX, this->posY + deltaY_l , this->width, this->height)){
        colY = true;
    }

    if(!colX){
        this->posX += deltaX_l;
    }

    if(!colY){
        this->posY += deltaY_l;
    }

    this->updateCenter();

    this->updateAnimation();

    this->setDeltaX(0);
    this->setDeltaY(0);
}

void LivingZombie::draw(){
    al_draw_rotated_bitmap(this->frameImage, this->width/2, this->height/2, this->posX+this->width/2+cameraOffsetX, this->posY+this->height/2+cameraOffsetY, this->angle, NULL);

    //HP bar
    if(this->livingHP < this->livingMaxHP){
        al_draw_rectangle(this->posX-1, this->posY-11, this->posX+this->width+1, this->posY-4, al_map_rgb(200,200,200), 2);
        al_draw_filled_rectangle(this->posX, this->posY-10, 2+(this->posX-2)+this->width/this->livingMaxHP*this->livingHP, this->posY-5, al_map_rgb(200,0,0));
    }
}
