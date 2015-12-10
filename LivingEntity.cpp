#include "Engine.h"
#include "LivingEntity.h"

LivingEntity::LivingEntity(){
    this->livingHP = 0;
    this->livingMaxHP = 0;
    this->livingDebuffs.resize(AMOUNT_DEBUFFS);
    this->timeAlive = 0;
}

double LivingEntity::getCurrentHP(){
    return this->livingHP;
}
double LivingEntity::getMaxHP(){
    return this->livingMaxHP;
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
void LivingEntity::addDebuff(int debuffID, double debuffStrength, double debuffDuration){
    this->livingDebuffs[debuffID].clear();
    this->livingDebuffs[debuffID].push_back(debuffStrength);
    this->livingDebuffs[debuffID].push_back(debuffDuration);
}
void LivingEntity::removeDebuff(int debuffID){
    this->livingDebuffs[debuffID].clear();
    switch(debuffID){
        case debuffBurning:
            break;
        case debuffPoison:
            break;
        case debuffElectrified:
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
                    this->takeDebuffDamage(this->livingDebuffs[i][0]*deltaTime, i);
                    break;
                case debuffPoison:
                    this->takeDebuffDamage(this->livingDebuffs[i][0]*deltaTime, i);
                    break;
                case debuffElectrified:
                    this->takeDebuffDamage(this->livingDebuffs[i][0], i);
                    this->removeDebuff(i);
                    break;
                case debuffSlowed:
                    this->movementSpeed = this->maxMovementSpeed*(1-this->livingDebuffs[i][0]);
                    break;
            }
            this->livingDebuffs[i][1] -= deltaTime;
            if(this->livingDebuffs[i][1] <= 0){
                this->removeDebuff(i);
            }
        }
    }
}
