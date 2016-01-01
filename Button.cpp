#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

Button::Button(){
    this->clicked = false;
}

void Button::update(){
    if(this->entityWorldPosition != worldPosition && this->usesWorldPosition){
        return;
    }
    this->clicked = false;
    if(checkCollision(this->posX, this->posY, mouseX, mouseY, this->width, this->height, 0, 0)){
        if(mouseButtonLeftClick){
            switch(this->typeId){
                case 0:
                    break; //Reserved for resume

                case 1:
                    break; //Reserved for screenState change

                case 2:
                    break; //Reserved for quit

            }
            this->clicked = true;
        }
        this->selected = true;
    }else{
        this->selected = false;
    }
}

void Button::draw(){
    if(this->entityWorldPosition != worldPosition && this->usesWorldPosition){
        return;
    }
    if(this->typeId < 3){
        al_draw_rectangle(this->posX+5, this->posY+5, this->posX+this->width-5, this->posY+this->height-5, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), 10);
    }

    switch(this->typeId){
        case 0:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Resume"); break;
        case 1:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "New Game"); break;
        case 2:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Quit"); break;
    }
}
