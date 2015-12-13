#include "EntityParticle.h"
#include "Engine.h"

EntityParticle::EntityParticle(){
    this->particleMovePattern = 0;
    this->particleDeathTime = 0, this->particleTimeAlive = 0;
    this->particleText = "";
    this->particleIsText = false;
    this->particleFrictionRatio = 0;
}

int EntityParticle::getMovePattern(){
    return this->particleMovePattern;
}
double EntityParticle::getTimeAlive(){
    return this->particleTimeAlive;
}
double EntityParticle::getDeathTime(){
    return this->particleDeathTime;
}
double EntityParticle::getFriction(){
    return this->particleFriction;
}
bool EntityParticle::getCollidesWithMap(){
    return this->particleCollidesWithMap;
}
std::string EntityParticle::getShowText(){
    return this->particleText;
}
ALLEGRO_FONT *EntityParticle::getFont(){
    switch(this->particleFont){
        case 0:
            return smallFont; break;

        case 1:
            return defaultFont; break;

        case 2:
            return bigFont; break;

    }
}
ALLEGRO_COLOR EntityParticle::getColor(){
    return this->particleColor;
}

void EntityParticle::setMovePattern(int movePattern){
    this->particleMovePattern = movePattern;
}
void EntityParticle::setDeathTime(double deathTime){
    this->particleDeathTime = deathTime;
}
void EntityParticle::setFriction(double frictionValue){
    this->particleFriction = frictionValue;
}
void EntityParticle::setGravity(double gravity){
    this->particleGravity = gravity;
}
void EntityParticle::setCollidesWithMap(bool collides){
    this->particleCollidesWithMap = collides;
}
void EntityParticle::setTextValue(std::string textValue, int fontValue){
    this->particleText = textValue;
    this->particleIsText = true;
    this->particleFont = fontValue;
    this->height = al_get_font_line_height(this->getFont());
    this->width = al_get_text_width(this->getFont(), textValue.c_str());
}
void EntityParticle::setColor(ALLEGRO_COLOR colorValue){
    this->particleColor = colorValue;
}

void EntityParticle::updateMovePatternDelta(){
    /*patternHomeToPlayer = 0,
    patternHomeToMouse,
    patternKeepMomentum,
    patternJumpUp,
    patternJumpUpGravity,*/
    double deltaX_l = this->getDelta(0)/deltaTime, deltaY_l = this->getDelta(1)/deltaTime;
    switch(this->particleMovePattern){
        case patternHomeToPlayer:
            setAngle(-atan2(this->getCenterPosition(0) - playerList[0]->getCenterPosition(0), this->getCenterPosition(1) - playerList[0]->getCenterPosition(1)));
            this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
            break;
        case patternHomeToMouse:
            setAngle(-atan2(this->getCenterPosition(0) - mouseX, this->getCenterPosition(1) - mouseY));
            this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
            break;
        case patternKeepMomentum:
            this->setDeltaX(deltaX_l*this->particleFrictionRatio), this->setDeltaY(deltaY_l*this->particleFrictionRatio);
            break;
        case patternGravity:
            this->setDeltaX(deltaX_l*this->particleFrictionRatio), this->setDeltaY(deltaY_l*this->particleFrictionRatio+this->particleGravity);
            break;
    }
}
void EntityParticle::update(){
    if(this->particleTimeAlive >= this->particleDeathTime){
        this->setActive(false);
    }

    particleFrictionRatio = 1 / (1 + this->particleFriction * deltaTime);

    this->updateMovePatternDelta();

    double deltaX_l = this->getDelta(0), deltaY_l = this->getDelta(1);

    double loopI = ceil(this->movementSpeed*deltaTime/this->width);
    bool colX, colY;

    for(double i = 0; i < loopI; i++){
        colX = false, colY = false;
        if(this->particleCollidesWithMap){
            if(!isPassable(this->posX + deltaX_l/loopI, this->posY, this->width, this->height)){
                colX = true;
            }

            if(!isPassable(this->posX, this->posY + deltaY_l/loopI , this->width, this->height)){
                colY = true;
            }
        }
        if(!colX){
            this->posX += deltaX_l/loopI;
        }
        if(!colY){
            this->posY += deltaY_l/loopI;
        }
    }

    this->updateCenter();

    this->particleTimeAlive += deltaTime;
}

void EntityParticle::draw(){
    if(!this->particleIsText){
        al_draw_filled_rectangle(this->posX+cameraOffsetX, this->posY+cameraOffsetY, this->posX+this->width+cameraOffsetX, this->posY+this->height+cameraOffsetY, this->particleColor);
    }else{
        al_draw_text(this->getFont(), this->particleColor, this->posX+cameraOffsetX, this->posY+cameraOffsetY, ALLEGRO_ALIGN_CENTER, this->particleText.c_str());
    }
}
