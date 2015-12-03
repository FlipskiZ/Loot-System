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
    double deltaX_l = this->getDelta()[0], deltaY_l = this->getDelta()[1];

    double loopI = ceil(this->movementSpeed*deltaTime/this->width);

    for(double i = 0; i < loopI; i++){
        /*for(int lI = 0; lI < livingList.size() && (!colX || !colY); lI++){
            if(livingList[lI] != NULL && livingList[lI]->getActive()){
                if(checkCollision(this->posX + this->deltaX_l/loopI, this->posY, livingList[lI]->posX, livingList[lI]->posY,
                    this->width, this->height, livingList[lI]->width, livingList[lI]->height)){
                    colX = true;
                }
                if(checkCollision(this->posX, this->posY + this->deltaY_l/loopI, livingList[lI]->posX, livingList[lI]->posY,
                    this->width, this->height, livingList[lI]->width, livingList[lI]->height)){
                    colY = true;
                }
            }
        }*/

        if(isPassable(this->posX + deltaX_l/loopI, this->posY, this->width, this->height)){
            this->posX += deltaX_l/loopI;
        }else{
            if(this->bulletSpecials[weaponRicochetAmount] <= 0){
                if(this->bulletSpecials[weaponExplosionRadius] > 0){
                    this->explode = true;
                }else{
                    this->setActive(false);
                }
            }else if(this->bulletSpecials[weaponRicochetAmount] < 1){ //If it is in between 0 and 1 decide by rng if it should bounce
                if(randDouble() > this->bulletSpecials[weaponRicochetAmount]){
                    this->angle = -atan2(deltaX_l, -deltaY_l);
                    this->bulletSpecials[weaponRicochetAmount] = 0;
                    this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
                    deltaX_l = this->getDelta()[0], deltaY_l = this->getDelta()[1];
                }else{
                    if(this->bulletSpecials[weaponExplosionRadius] > 0){
                        this->explode = true;
                    }else{
                        this->setActive(false);
                    }
                }
            }else{ //If ricochetAmount is >1 bounce the bullet
                this->angle = -atan2(deltaX_l, -deltaY_l);
                this->bulletSpecials[weaponRicochetAmount]--;
                this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
                deltaX_l = this->getDelta()[0], deltaY_l = this->getDelta()[1];
            }
        }

        if(isPassable(this->posX, this->posY + deltaY_l/loopI, this->width, this->height)){
            this->posY += deltaY_l/loopI;
        }else{
            if(this->bulletSpecials[weaponRicochetAmount] <= 0){
                if(this->bulletSpecials[weaponExplosionRadius] > 0){
                    this->explode = true;
                }else{
                    this->setActive(false);
                }
            }else if(this->bulletSpecials[weaponRicochetAmount] < 1){ //If it is in between 0 and 1 decide by rng if it should bounce
                if(randDouble() > this->bulletSpecials[weaponRicochetAmount]){
                    this->angle = -atan2(-deltaX_l, deltaY_l);
                    this->bulletSpecials[weaponRicochetAmount] = 0;
                    this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
                    deltaX_l = this->getDelta()[0], deltaY_l = this->getDelta()[1];
                }else{
                    if(this->bulletSpecials[weaponExplosionRadius] > 0){
                        this->explode = true;
                    }else{
                        this->setActive(false);
                    }
                }
            }else{ //If ricochetAmount is >1 bounce the bullet
                this->angle = -atan2(-deltaX_l, deltaY_l);
                this->bulletSpecials[weaponRicochetAmount]--;
                this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
                deltaX_l = this->getDelta()[0], deltaY_l = this->getDelta()[1];
            }
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
        al_draw_scaled_bitmap(explosionImage, 0, 0, 32, 32, this->posX-this->bulletSpecials[weaponExplosionRadius], this->posY-this->bulletSpecials[weaponExplosionRadius], this->bulletSpecials[weaponExplosionRadius]*2, this->bulletSpecials[weaponExplosionRadius]*2, NULL);
    }
}
