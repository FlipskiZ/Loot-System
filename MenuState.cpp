#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

MenuState MenuState::m_MenuState;

void MenuState::init(){
    int i = (inGame) ? 0 : 1;
    int loopValue = 3;
    for(; i < loopValue; i++){
        double x = 0, y = 0, width = 0, height = 0;
        int buttonId = i;

        width = 256, height = 64;

        x = screenWidth/2-width/2, y = screenHeight/2-height/2-height*(((inGame) ? loopValue-1.0 : loopValue+0.0)/2)+height*i;

        unique_ptr<Button> newButton(new Button());
        newButton->setPos(x, y);
        newButton->setDimensions(width, height);
        newButton->setTypeId(buttonId);

        addButtonToList(move(newButton));
    }

    printf("MenuState Init\n");
}
void MenuState::cleanup(){
    for(int i = 0; i < buttonList.size(); i++){
        buttonList[i]->setActive(false);
    }
    inGame = false;
    printf("MenuState Cleanup\n");
}

void MenuState::pause(){
    printf("MenuState Pause\n");
}
void MenuState::resume(){
    printf("MenuState Resume\n");
}

void MenuState::update(Engine* engine){
    //Input +
    if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
        if(lastKeyPress != ALLEGRO_KEY_ESCAPE){
            if(inGame){
                engine->popState();
            }else{
                engine->quit();
            }
            lastKeyPress = ALLEGRO_KEY_ESCAPE;
        }
     }
    //Input -

    //Update Entities +
    for(int i = 0; i < buttonList.size(); i++){
        if(buttonList[i] != NULL && buttonList[i]->getActive()){
            buttonList[i]->update();
            if(buttonList[i]->clicked){
                switch(buttonList[i]->getTypeId()){
                    case 0:
                        engine->popState();
                        break;

                    case 1:
                        engine->changeState(PlayState::instance());
                        break;

                    case 2:
                        engine->quit();
                        break;
                }
            }
        }
    }
    //Update Entities -

    //Rest +

    //Rest -
}
void MenuState::draw(Engine* engine){
    for(int i = 0; i < buttonList.size(); i++){
        if(buttonList[i] != NULL && buttonList[i]->getActive()){
            buttonList[i]->draw();
        }
    }
}
