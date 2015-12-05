#include "Engine.h"
#include "LivingEntity.h"

LivingEntity::LivingEntity(){
    this->livingHP = 0;
    this->livingMaxHP = 0;
}

double LivingEntity::getCurrentHP(){
    return this->livingHP;
}
double LivingEntity::getMaxHP(){
    return this->livingMaxHP;
}

void LivingEntity::setMaxHP(double maxHP){
    this->livingMaxHP = maxHP;
    this->livingHP = maxHP;
}
void LivingEntity::setCurrentHP(double currentHP){
    this->livingHP = currentHP;
}
