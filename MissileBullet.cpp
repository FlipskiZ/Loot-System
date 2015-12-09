#include "MissileBullet.h"
#include "Engine.h"

MissileBullet::MissileBullet(){
    this->playerOwned = false;
    this->bulletStats.clear();
    this->bulletSpecials.clear();
    this->rangeBeforeSplit = randDouble(45,360); //How far in pixels it will go before splitting
    this->rangeBeforeSplitHelper = 0;
    this->explosionLinger = 0.25;
    this->explosionLingerCount = 0;
    this->explode = false;
    this->tileColX = false, this->tileColY = false;
    this->livingColX = false, this->livingColY = false;
    this->richochetX = false, this->richochetY = false;
    this->penetratedEnemies.clear();
    this->loopPenetratedEnemies.clear();
}

void MissileBullet::setMissileStats(std::vector<double> missileStats){
    this->bulletStats = missileStats;
}

void MissileBullet::setMissileSpecials(std::vector<double> missileSpecials){
    this->bulletSpecials = missileSpecials;
}

void MissileBullet::update(){
    if(this->explode){
        if(this->explosionLingerCount >= this->explosionLinger){
            this->setActive(false);
        }
        this->explosionLingerCount += deltaTime;
        return;
    }
    this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
    double deltaX_l = this->getDelta(0), deltaY_l = this->getDelta(1);

    double loopI = ceil(this->movementSpeed*deltaTime/this->width);

    int lastHitLiving = -1;

    for(double i = 0; i < loopI; i++){
        this->tileColX = false, this->tileColY = false;
        this->livingColX = false, this->livingColY = false;
        this->richochetX = false, this->richochetY = false;
        this->loopPenetratedEnemies.clear();

        bool loopCol = false;

        this->livingCollisions.clear();

        collisionDetection->getPotentialCollisions(this->livingCollisions, this->posX, this->posY, this->width, this->height);

        for(int j = 0; j < livingCollisions.size(); j++){
            if(livingList[livingCollisions[j]] != NULL && livingList[livingCollisions[j]]->getActive()){
                loopCol = false;

                if(checkCollision(this->posX + deltaX_l/loopI, this->posY, livingList[livingCollisions[j]]->getPosition(0), livingList[livingCollisions[j]]->getPosition(1),
                    this->width, this->height, livingList[livingCollisions[j]]->getDimension(0), livingList[livingCollisions[j]]->getDimension(1))){ //If it collides with a livingEntity in posX

                    this->livingColX = true;
                    loopCol = true;
                }
                if(checkCollision(this->posX, this->posY + deltaY_l/loopI, livingList[livingCollisions[j]]->getPosition(0), livingList[livingCollisions[j]]->getPosition(1),
                    this->width, this->height, livingList[livingCollisions[j]]->getDimension(0), livingList[livingCollisions[j]]->getDimension(1))){ //If it collides with a livingEntity in posY

                    this->livingColY = true;
                    loopCol = true;
                }

                if(loopCol){
                    if(find(penetratedEnemies.begin(), penetratedEnemies.end(), livingCollisions[j]) == penetratedEnemies.end()){//If bullet penetrates and if it hasn't already penetrated this entity
                        if(this->bulletSpecials[weaponPenetrationAmount] > 0){
                            if(this->bulletSpecials[weaponPenetrationAmount] >= 1 ||
                            (this->bulletSpecials[weaponPenetrationAmount] < 1 && randDouble() < this->bulletSpecials[weaponPenetrationAmount])){
                                this->penetratedEnemies.push_back(livingCollisions[j]);
                                this->bulletSpecials[weaponPenetrationAmount]--;
                                livingColX = false, livingColY = false;
                            }else if(bulletSpecials[weaponPenetrationAmount] < 1){ //If it didn't succeed the randDouble check
                                this->bulletSpecials[weaponPenetrationAmount] = 0;
                            }
                        }
                        if(this->loopPenetratedEnemies.size() <= this->bulletSpecials[weaponPenetrationAmount]){
                            this->loopPenetratedEnemies.push_back(livingCollisions[j]);
                            lastHitLiving = livingCollisions[j];
                        }
                    }else{
                        livingColX = false, livingColY = false;
                    }
                }
            }
        }

        vector<int>::iterator iteratorPenetrated;
        for(int i = 0; i < this->loopPenetratedEnemies.size(); i++){
            for(int j = 0; j < this->penetratedEnemies.size(); j++){
                iteratorPenetrated = find(this->penetratedEnemies.begin(), this->penetratedEnemies.end(), this->loopPenetratedEnemies[i]);
                if(iteratorPenetrated == this->penetratedEnemies.end()){
                    this->penetratedEnemies.erase(this->penetratedEnemies.begin()+j);
                }
            }
            //Damage Enemy
            livingList[loopPenetratedEnemies[i]]->takeDamage(randDouble(this->bulletStats[weaponMinDamage], this->bulletStats[weaponMaxDamage]), false);
        }

        if(!isPassable(this->posX + deltaX_l/loopI, this->posY, this->width, this->height)){
            this->tileColX = true;
        }

        if(!isPassable(this->posX, this->posY + deltaY_l/loopI, this->width, this->height)){
            this->tileColY = true;
        }

        if(this->tileColX || this->tileColY){
            if(this->bulletSpecials[weaponRicochetAmount] >= 1 ||
                (this->bulletSpecials[weaponRicochetAmount] < 1 && randDouble() < this->bulletSpecials[weaponRicochetAmount])){ //If it is in between 0 and 1 decide by rng if it should bounce
                if(this->tileColX){
                    this->richochetX = true;
                }
                if(this->tileColY){
                    this->richochetY = true;
                }
            }else{
                if(this->bulletSpecials[weaponExplosionRadius] > 0){
                    this->explode = true;
                }else{
                    this->setActive(false);
                }
            }
        }

        if(this->richochetX || this->richochetY){
            this->angle = -atan2((richochetY) ? -deltaX_l : deltaX_l, (richochetX) ? -deltaY_l : deltaY_l);
            this->bulletSpecials[weaponRicochetAmount]--;
            this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
            deltaX_l = this->getDelta(0), deltaY_l = this->getDelta(1);
        }

        if(this->livingColX){
            if(this->bulletSpecials[weaponExplosionRadius] > 0){
                this->explode = true;
            }else{
                this->setActive(false);
            }
        }

        if(this->livingColY){
            if(this->bulletSpecials[weaponExplosionRadius] > 0){
                this->explode = true;
            }else{
                this->setActive(false);
            }
        }

        if(!this->tileColX && !this->livingColX){
            this->posX += deltaX_l/loopI;
        }

        if(!this->tileColY && !this->livingColY){
            this->posY += deltaY_l/loopI;
        }
    }

    if(this->rangeBeforeSplitHelper >= this->rangeBeforeSplit && this->bulletSpecials[weaponBulletSplit] > 0){ //Split bullet
        double bulletSplit = this->bulletSpecials[weaponBulletSplit];
        this->bulletSpecials[weaponBulletSplit] = 0;
        for(double i = bulletSplit; i > 0; i--){
            if(i > 0 && i < 1){
                if(randDouble() < i){
                    bulletSplit = ceil(bulletSplit); //Round up if another bullet is to be shot
                }else{
                    bulletSplit = trunc(bulletSplit); //Round down if another bullet is not to be shot
                }
            }
        }

        if(bulletSplit > 1){
            double startAngle = this->angle, angleVariation = 0.3333333, maxAngle = 45*toRadians, angleVariationStart = -maxAngle/2;
            double angleConeStart = startAngle-maxAngle/2;
            double angleSlice = maxAngle/(bulletSplit-1);

            for(int i = 0; i < bulletSplit; i++){
                unique_ptr<MissileBullet> newBullet(new MissileBullet());
                newBullet->setPos(this->centerX, this->centerY);
                newBullet->setDimensions(this->width, this->height);
                newBullet->setMovementSpeed(this->movementSpeed);
                newBullet->setAngle(angleConeStart+angleSlice*i+randDouble(angleVariationStart, angleVariationStart+maxAngle)*(1-angleVariation));
                newBullet->setMissileStats(this->bulletStats);
                newBullet->setMissileSpecials(this->bulletSpecials);
                newBullet->setSheetDimensions(this->sheetColums, this->sheetRows, this->width, this->height);
                newBullet->setAnimationSpeed(this->animationCount);
                newBullet->setBitmap(bulletImage);
                addBulletToList(move(newBullet));
            }
            this->setActive(false);
        }
    }

    if(this->explode){ //The bullet died, it will now explode
        //Hit all enemies in the explosion radius
        double radius = this->bulletSpecials[weaponExplosionRadius];
        double pRadius = sqrt(pow(this->bulletSpecials[weaponExplosionRadius], 2)*2);

        double distance = 0;

        double damage = 0;

        this->livingCollisions.clear();

        collisionDetection->getPotentialCollisions(this->livingCollisions, this->posX-radius, this->posY-radius, radius*2, radius*2);

        for(int j = 0; j < livingCollisions.size(); j++){
            if(livingList[livingCollisions[j]] != NULL && livingList[livingCollisions[j]]->getActive() && this->livingCollisions[j] != lastHitLiving){ //The explosion is not supposed to hit the last hit enemy. No double damage, sorry.
                if(checkCollision(this->centerX - radius, this->centerY - radius, livingList[livingCollisions[j]]->getPosition(0), livingList[livingCollisions[j]]->getPosition(1),
                    radius*2, radius*2, livingList[livingCollisions[j]]->getDimension(0), livingList[livingCollisions[j]]->getDimension(1))){ //If it collides with a livingEntity

                    //Distance difference between the center of explosion and center of the object
                    //This->center is the center of the bullet, the explosion is already centered to the bullet
                    distance = sqrt(pow(abs(livingList[livingCollisions[j]]->getCenterPosition(0)-this->centerX), 2)+pow(abs(livingList[livingCollisions[j]]->getCenterPosition(1)-this->centerY), 2));

                    damage = randDouble(this->bulletStats[weaponMinDamage]-(distance*this->bulletStats[weaponMinDamage]/pRadius),
                            this->bulletStats[weaponMaxDamage]-(distance*this->bulletStats[weaponMaxDamage]/pRadius));

                    livingList[livingCollisions[j]]->takeDamage(damage, false);
                }
            }
        }
    }

    this->updateCenter();

    this->updateAnimation();

    this->setDeltaX(0);
    this->setDeltaY(0);

    this->rangeBeforeSplitHelper += this->movementSpeed * deltaTime; //Adds on the amount of pixels traveled in one tick.
}

void MissileBullet::draw(){
    if(!this->explode){
        al_draw_rotated_bitmap(this->frameImage, this->width/2, this->height/2, this->posX+this->width/2+cameraOffsetX, this->posY+this->height/2+cameraOffsetY, this->angle, NULL);
    }else{
        al_draw_scaled_bitmap(explosionImage, 0, 0, 32, 32, this->centerX-this->bulletSpecials[weaponExplosionRadius], this->centerY-this->bulletSpecials[weaponExplosionRadius], this->bulletSpecials[weaponExplosionRadius]*2, this->bulletSpecials[weaponExplosionRadius]*2, NULL);
    }
}
