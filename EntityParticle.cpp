#include "EntityParticle.h"
#include "Engine.h"

EntityParticle::EntityParticle(){
    this->particleMovePattern = 0;
    this->particleDeathTime = 0, this->particleTimeAlive = 0;
    this->particleText = "";
    this->particleIsText = false;
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

void EntityParticle::update(){
    if(this->particleTimeAlive >= this->particleDeathTime){
        this->setActive(false);
    }

    setAngle(-atan2(this->getCenterPosition()[0] - playerList[0]->getCenterPosition()[0], this->getCenterPosition()[1] - playerList[0]->getCenterPosition()[1]));
    this->setDeltaX(sin(this->angle)*this->movementSpeed), this->setDeltaY(-cos(this->angle)*this->movementSpeed);
    double deltaX_l = this->getDelta()[0], deltaY_l = this->getDelta()[1];

    double loopI = ceil(this->movementSpeed*deltaTime/this->width);

    for(double i = 0; i < loopI; i++){
        if(isPassable(this->posX + deltaX_l/loopI, this->posY, this->width, this->height)){
            this->posX += deltaX_l/loopI;
        }

        if(isPassable(this->posX, this->posY + deltaY_l/loopI , this->width, this->height)){
            this->posY += deltaY_l/loopI;
        }
    }

    this->updateCenter();

    this->setDeltaX(0);
    this->setDeltaY(0);

    this->particleTimeAlive += deltaTime;
}

void EntityParticle::draw(){
    if(!this->particleIsText){
        al_draw_filled_rectangle(this->posX, this->posY, this->posX+this->width, this->posY+this->height, this->particleColor);
    }else{
        al_draw_text(this->getFont(), this->particleColor, this->posX, this->posY, ALLEGRO_ALIGN_CENTER, this->particleText.c_str());
    }
}
