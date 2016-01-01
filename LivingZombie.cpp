#include "LivingZombie.h"
#include "Engine.h"

LivingZombie::LivingZombie(){
    this->livingCollisions.clear();
    this->setAnimationValue(1, false, 1);
}

void LivingZombie::takeDamage(double damage, bool crit, double armorBypass){
    double armor_l = this->livingArmor;

    armor_l = armor_l*(1 - armorBypass); //Reduce the armor by the armorbypass percentage
    if(armor_l < 0){
        armor_l = 0;
    }
    if(damage > 1){
        if(damage-armor_l < 1){ //If the damage will be less than 1 after armor set the damage to 1; Effectively making 1 the minimum damage with armor.
            damage = 1;
        }else{
            damage -= armor_l;
        }
    }

    double movementSpeed = 160, deathTime = 0.6, frictionValue = 2, gravityValue = 5, angle = randDouble(-45, 45)*toRadians;
    int movePattern = patternGravity, fontValue = 0;
    bool collidesWithMap = false;
    string textValue = dtos(round(damage*100)/100);
    textValue.insert(textValue.begin(), '-');
    ALLEGRO_COLOR colorValue = al_map_rgb(255, 50, 50);

    if(crit){
        deathTime *= 2;
        gravityValue /= 2;
        fontValue = 1;
    }

    unique_ptr<EntityParticle> newParticle(new EntityParticle);
    newParticle->setPos(this->centerX, this->posY);
    newParticle->setMovementSpeed(movementSpeed);
    newParticle->setMovePattern(movePattern);
    newParticle->setDeltaX(movementSpeed, angle), newParticle->setDeltaY(movementSpeed, angle);
    newParticle->setDeathTime(deathTime);
    newParticle->setFriction(frictionValue);
    newParticle->setGravity(gravityValue);
    newParticle->setCollidesWithMap(collidesWithMap);
    newParticle->setTextValue(textValue, fontValue);
    newParticle->setColor(colorValue);
    addParticleToList(move(newParticle));

    this->livingHP -= damage;

    if(this->livingHP <= 0){
        this->setActive(false);
    }

    this->livingDamaged = true;
    this->livingDamagedDuration = 0.25;
    this->livingDamagedHelper = 0;
}

void LivingZombie::takeDebuffDamage(double damage, int debuffID){
    double movementSpeed = 160, deathTime = 0.6, frictionValue = 2, gravityValue = 5, angle = randDouble(-45, 45)*toRadians;
    int movePattern = patternGravity, fontValue = 0;
    bool collidesWithMap = false;
    string textValue = dtos(round(damage*100)/100);
    textValue.insert(textValue.begin(), '-');
    ALLEGRO_COLOR colorValue = al_map_rgb(255, 255, 255);
    if(debuffID == debuffBurning){
        colorValue = al_map_rgb(255, 125, 50);
    }else if(debuffID == debuffPoison){
        colorValue = al_map_rgb(75, 150, 50);
    }else if(debuffID == debuffElectrified){
        colorValue = al_map_rgb(50, 225, 255);
    }

    unique_ptr<EntityParticle> newParticle(new EntityParticle);
    newParticle->setPos(this->centerX, this->posY);
    newParticle->setMovementSpeed(movementSpeed);
    newParticle->setMovePattern(movePattern);
    newParticle->setDeltaX(movementSpeed, angle), newParticle->setDeltaY(movementSpeed, angle);
    newParticle->setDeathTime(deathTime);
    newParticle->setFriction(frictionValue);
    newParticle->setGravity(gravityValue);
    newParticle->setCollidesWithMap(collidesWithMap);
    newParticle->setTextValue(textValue, fontValue);
    newParticle->setColor(colorValue);
    addParticleToList(move(newParticle));

    this->livingHP -= damage;

    if(this->livingHP <= 0){
        this->setActive(false);
    }

    this->livingDamaged = true;
    this->livingDamagedDuration = 0.125;
    this->livingDamagedHelper = 0;
}

void LivingZombie::healHP(double health){
    double movementSpeed = 160, deathTime = 0.6, frictionValue = 2, gravityValue = 5, angle = randDouble(-45, 45)*toRadians;
    int movePattern = patternGravity, fontValue = 0;
    bool collidesWithMap = false;
    string textValue = dtos(round(health*100)/100);
    textValue.insert(textValue.begin(), '+');
    ALLEGRO_COLOR colorValue = al_map_rgb(50, 255, 50);

    unique_ptr<EntityParticle> newParticle(new EntityParticle);
    newParticle->setPos(this->centerX, this->posY);
    newParticle->setMovementSpeed(movementSpeed);
    newParticle->setMovePattern(movePattern);
    newParticle->setDeltaX(movementSpeed, angle), newParticle->setDeltaY(movementSpeed, angle);
    newParticle->setDeathTime(deathTime);
    newParticle->setFriction(frictionValue);
    newParticle->setGravity(gravityValue);
    newParticle->setCollidesWithMap(collidesWithMap);
    newParticle->setTextValue(textValue, fontValue);
    newParticle->setColor(colorValue);
    addParticleToList(move(newParticle));

    this->livingHP += health;
    if(this->livingHP > this->livingMaxHP){
        this->livingHP = this->livingMaxHP;
    }

    this->livingHealed = true;
    this->livingHealedDuration = 0.25;
    this->livingHealedHelper = 0;
}

void LivingZombie::update(){
    if(this->entityWorldPosition != worldPosition && this->usesWorldPosition){
        return;
    }
    this->updateDebuffs();

    this->angleSin = sin(this->angle), this->angleCos = -cos(this->angle);

    setAngle(-atan2(this->getCenterPosition(0) - playerList[0]->getCenterPosition(0), this->getCenterPosition(1) - playerList[0]->getCenterPosition(1)));
    this->setDeltaX(angleSin*this->movementSpeed), this->setDeltaY(angleCos*this->movementSpeed);
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

        if(this->animationValue != 2){
            this->setAnimationValue(3, true, 1);
            if(this->nextFrame && this->frameX == 2){
                playerList[0]->takeDamage(2);
            }
        }
    }
    if(checkCollision(this->posX, this->posY + deltaY_l, playerList[0]->getPosition(0), playerList[0]->getPosition(1),
        this->width, this->height, playerList[0]->getDimension(0), playerList[0]->getDimension(1))){
        colY = true;

        if(this->animationValue != 2){
            this->setAnimationValue(3, true, 1);
            if(this->nextFrame && this->frameX == 2){
                playerList[0]->takeDamage(2);
            }
        }
    }

    if(this->animationValue != 3){
        if(checkCollision(this->posX + deltaX_l + this->angleSin*12, this->posY + deltaY_l + this->angleCos*12, playerList[0]->getPosition(0), playerList[0]->getPosition(1),
            this->width, this->height, playerList[0]->getDimension(0), playerList[0]->getDimension(1))){
            this->setAnimationValue(2, true, 1);
            if(this->nextFrame && this->frameX == 2){
                playerList[0]->takeDamage(1);
            }
        }
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

    this->timeAlive += deltaTime;

    if(this->livingDamaged){
        this->livingDamagedHelper += deltaTime;
        if(this->livingDamagedHelper >= this->livingDamagedDuration){
            this->livingDamaged = false;
        }
    }

    if(this->livingHealed){
        this->livingHealedHelper += deltaTime;
        if(this->livingHealedHelper >= this->livingHealedDuration){
            this->livingHealed = false;
        }
    }
}

void LivingZombie::draw(){
    if(this->entityWorldPosition != worldPosition && this->usesWorldPosition){
        return;
    }
    if(this->livingDamaged){
        al_draw_tinted_rotated_bitmap(this->frameImage, al_map_rgba_f(1, 0, 0, 1), this->width/2, this->height/2, this->posX+this->width/2+cameraOffsetX, this->posY+this->height/2+cameraOffsetY, this->angle, NULL);
    }else if(this->livingHealed){
        al_draw_tinted_rotated_bitmap(this->frameImage, al_map_rgba_f(0, 1, 0, 1), this->width/2, this->height/2, this->posX+this->width/2+cameraOffsetX, this->posY+this->height/2+cameraOffsetY, this->angle, NULL);
    }else{
        al_draw_rotated_bitmap(this->frameImage, this->width/2, this->height/2, this->posX+this->width/2+cameraOffsetX, this->posY+this->height/2+cameraOffsetY, this->angle, NULL);
    }
    //HP bar
    if(this->livingHP < this->livingMaxHP){
        al_draw_rectangle(this->posX-1+cameraOffsetX, this->posY-11+cameraOffsetY, this->posX+this->width+1+cameraOffsetX, this->posY-4+cameraOffsetY, al_map_rgb(200,200,200), 2);
        al_draw_filled_rectangle(this->posX+cameraOffsetX, this->posY-10+cameraOffsetY, 2+(this->posX-2)+this->width/this->livingMaxHP*this->livingHP+cameraOffsetX, this->posY-5+cameraOffsetY, al_map_rgb(200,0,0));
    }
}
