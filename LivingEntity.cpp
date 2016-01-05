#include "Engine.h"
#include "LivingEntity.h"

LivingEntity::LivingEntity(){
    this->livingHP = 0;
    this->livingMaxHP = 0;
    this->livingDebuffs.resize(AMOUNT_DEBUFFS);
    this->timeAlive = 0;
    this->livingDamaged = false;
    this->livingDamagedDuration = 0;
    this->livingDamagedHelper = 0;
    this->livingHealed = false;
    this->livingHealedDuration = 0;
    this->livingHealedHelper = 0;
}

double LivingEntity::getCurrentHP(){
    return this->livingHP;
}
double LivingEntity::getMaxHP(){
    return this->livingMaxHP;
}
double LivingEntity::getArmor(){
    return this->livingArmor;
}
vector<vector<double>> LivingEntity::getDebuffs(){
    return this->livingDebuffs;
}

void LivingEntity::setMaxHP(double maxHP){
    this->livingMaxHP = maxHP;
    this->livingHP = maxHP;
}
void LivingEntity::setCurrentHP(double currentHP){
    this->livingHP = currentHP;
}
void LivingEntity::setArmor(double armor){
    this->livingArmor = armor;
}
void LivingEntity::addDebuff(int debuffID, double debuffStrength, double debuffDuration){
    this->livingDebuffs[debuffID].clear();
    this->livingDebuffs[debuffID].push_back(debuffStrength);
    this->livingDebuffs[debuffID].push_back(debuffDuration);
    this->livingDebuffs[debuffID].push_back(al_get_time());

    if(debuffID == debuffElectrified){
        this->takeDebuffDamage(this->livingDebuffs[debuffID][0], debuffID);
    }
}
void LivingEntity::removeDebuff(int debuffID){
    this->livingDebuffs[debuffID].clear();
    switch(debuffID){
        case debuffBurning:
            break;
        case debuffPoison:
            break;
        case debuffElectrified:
            this->movementSpeed = this->maxMovementSpeed;
            break;
        case debuffSlowed:
            this->movementSpeed = this->maxMovementSpeed;
            break;
    }
}

void LivingEntity::updateDebuffs(){
    for(int i = 0; i < this->livingDebuffs.size(); i++){
        if(!this->livingDebuffs[i].empty()){
            switch(i){
                case debuffBurning:
                    if(fmod(abs(this->livingDebuffs[i][2]-this->timeAlive), 0.5) <= deltaTime){ //twice as fast as poison
                        this->takeDebuffDamage(this->livingDebuffs[i][0]*0.5, i);
                    }
                    break;
                case debuffPoison:
                    if(fmod(abs(this->livingDebuffs[i][2]-this->timeAlive), 1) <= deltaTime){ //Once a second
                        this->takeDebuffDamage(this->livingDebuffs[i][0], i);
                    }
                    break;
                case debuffElectrified:
                    this->movementSpeed = 0;
                    break;
                case debuffSlowed:
                    if(this->livingDebuffs[debuffElectrified].empty()){
                        this->movementSpeed = this->maxMovementSpeed*(1-this->livingDebuffs[i][0]);
                    }
                    break;
            }
            this->livingDebuffs[i][1] -= deltaTime;
            if(this->livingDebuffs[i][1] <= 0){
                this->removeDebuff(i);
            }
        }
    }
}
