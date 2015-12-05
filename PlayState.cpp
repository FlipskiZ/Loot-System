#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"
#include "InventoryState.h"

PlayState PlayState::m_PlayState;

void PlayState::init(){
    LootSystem lootSystem;

    //Create Player
    double width = 24, height = 24, movementSpeed = 128, sheetColums = 4, sheetRows = 3, animationSpeed = 0.25, inventorySpace = 20;
    playerList.resize(1);
    playerList[0] = unique_ptr<LivingPlayer>(new LivingPlayer());
    playerList[0]->setPos(mapDisplayWidth/2-width/2, mapDisplayHeight/2-height/2);
    playerList[0]->setDimensions(width, height);
    playerList[0]->setMovementSpeed(movementSpeed);
    playerList[0]->setSheetDimensions(sheetColums, sheetRows, width, height);
    playerList[0]->setAnimationSpeed(animationSpeed);
    playerList[0]->setBitmap(playerImage);
    playerList[0]->setMaxInventorySpace(inventorySpace);

    loadMapArray();

    for(int i = 0; i < buttonList.size(); i++){
        buttonList[i] = NULL;
    }
    for(int i = 0; i < itemList.size(); i++){
        itemList[i] = NULL;
    }
    for(int i = 0; i < missileList.size(); i++){
        missileList[i] = NULL;
    }
    lootSystem.createWeapon(1);
    playerList[0]->equipWeapon(0);

    al_stop_samples();
}

void PlayState::cleanup(){

}

void PlayState::pause(){
    inGame = true;
}

void PlayState::resume(){
}

void PlayState::update(Engine* engine){

    //Player Input +
    double movementSpeedP = playerList[0]->getMovementSpeed();

    playerList[0]->setAnimationValue(0, false, 1);

    int horizontal = 0, vertical = 0;

    if(al_key_down(&keyState, ALLEGRO_KEY_W)){
        playerList[0]->setDeltaY(-movementSpeedP);
        playerList[0]->setAnimationValue(1, false, 1);
        vertical = 1;
    }else if(al_key_down(&keyState, ALLEGRO_KEY_S)){
        playerList[0]->setDeltaY(movementSpeedP);
        playerList[0]->setAnimationValue(1, false, 1);
        vertical = 2;
    }
    if(al_key_down(&keyState, ALLEGRO_KEY_A)){
        playerList[0]->setDeltaX(-movementSpeedP);
        playerList[0]->setAnimationValue(1, false, 1);
        horizontal = 1;
    }else if(al_key_down(&keyState, ALLEGRO_KEY_D)){
        playerList[0]->setDeltaX(movementSpeedP);
        playerList[0]->setAnimationValue(1, false, 1);
        horizontal = 2;
    }

    if(vertical != 0 && horizontal != 0){
        playerList[0]->setDeltaX(playerList[0]->getDelta()[0]/deltaTime / twoSqrt);
        playerList[0]->setDeltaY(playerList[0]->getDelta()[1]/deltaTime / twoSqrt);
    }

    if(!mouseButtonLeft && !mouseButtonLeftClick){ //Sets the angle for the player depending on the keys presset
        if(vertical > 0){
            if(horizontal == 0){
                playerList[0]->setAngle((vertical-1)*180*toRadians);
            }else{
                if(vertical == 1){
                    playerList[0]->setAngle((horizontal == 1) ? 315*toRadians : 45*toRadians);
                }else if(vertical == 2){
                    playerList[0]->setAngle((horizontal == 1) ? 225*toRadians : 135*toRadians);
                }
            }
        }else if(horizontal > 0){
            playerList[0]->setAngle(270*toRadians+(horizontal-1)*180*toRadians);
        }
    }else if(mouseY <= mapDisplayHeight){ //If the mouse is within the map bounds
        playerList[0]->setAngle(-atan2(playerList[0]->getCenterPosition()[0] + cameraOffsetX - mouseX, playerList[0]->getCenterPosition()[1] + cameraOffsetY - mouseY));
        playerList[0]->setAnimationValue(2, false, 1);
        if(mouseButtonLeft){
            playerList[0]->fireWeapon();
        }
    }

    if(al_key_down(&keyState, ALLEGRO_KEY_F5)){
        if(lastKeyPress != ALLEGRO_KEY_F5){
            loadMapArray();
            lastKeyPress = ALLEGRO_KEY_F5;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_G)){
        if(lastKeyPress != ALLEGRO_KEY_G){
            double width = 24, height = 24, movementSpeed = 64, sheetColums = 4, sheetRows = 3, animationSpeed = 0.25;
            unique_ptr<LivingZombie> newZombie(new LivingZombie());
            newZombie->setPos(mouseX-width/2, mouseY-height/2);
            newZombie->setDimensions(width, height);
            newZombie->setMovementSpeed(movementSpeed);
            newZombie->setSheetDimensions(sheetColums, sheetRows, width, height);
            newZombie->setAnimationSpeed(animationSpeed);
            newZombie->setBitmap(zombieImage);
            addZombieToList(move(newZombie));

            lastKeyPress = ALLEGRO_KEY_G;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_I)){
        if(lastKeyPress != ALLEGRO_KEY_I){
            engine->pushState(InventoryState::instance());

            lastKeyPress = ALLEGRO_KEY_I;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
        if(lastKeyPress != ALLEGRO_KEY_ESCAPE){
            engine->pushState(MenuState::instance());
            //engine->changeState(PlayState::instance());
            lastKeyPress = ALLEGRO_KEY_ESCAPE;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_COMMA)){
        if(lastKeyPress != ALLEGRO_KEY_COMMA){
            if(volumeLevel > 0)
                volumeLevel -= 0.1;

            lastKeyPress = ALLEGRO_KEY_COMMA;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_FULLSTOP)){
        if(lastKeyPress != ALLEGRO_KEY_FULLSTOP){
            if(volumeLevel < 1)
                volumeLevel += 0.1;

            lastKeyPress = ALLEGRO_KEY_FULLSTOP;
        }
    }
    //Player Input -

    //Update Entities +
    for(int i = 0; i < playerList.size(); i++){
        if(playerList[i] != NULL && playerList[i]->getActive()){
            playerList[i]->update();
        }
    }
    for(int i = 0; i < missileList.size(); i++){
        if(missileList[i] != NULL && missileList[i]->getActive()){
            missileList[i]->update();
        }
    }
    for(int i = 0; i < livingList.size(); i++){
        if(livingList[i] != NULL && livingList[i]->getActive()){
            livingList[i]->update();
        }
    }
    for(int i = 0; i < particleList.size(); i++){
        if(particleList[i] != NULL && particleList[i]->getActive()){
            particleList[i]->update();
        }
    }
    /*for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonList[i] != NULL && buttonList[i]->getActive()){
            buttonList[i]->update();
            if(buttonList[i]->clicked){
                switch(buttonList[i]->typeId){
                    case 3:
                        playerList[0]->buyWeapon(0,0);
                        break;
                }
            }
        }
    }*/
    //Update Entities -

    //Rest +

    //Rest -

}

void PlayState::draw(Engine* engine){
    //Camera +
    updateCamera();
    //Camera -

    //Draw Map+
    drawMap();
    //Draw Map-

    //Draw Entities +
    for(int i = 0; i < missileList.size(); i++){
        if(missileList[i] != NULL && missileList[i]->getActive()){
            missileList[i]->draw();
        }
    }
    for(int i = 0; i < playerList.size(); i++){
        if(playerList[i] != NULL && playerList[i]->getActive()){
            playerList[i]->draw();
        }
    }
    for(int i = 0; i < livingList.size(); i++){
        if(livingList[i] != NULL && livingList[i]->getActive()){
            livingList[i]->draw();
        }
    }
    for(int i = 0; i < particleList.size(); i++){
        if(particleList[i] != NULL && particleList[i]->getActive()){
            particleList[i]->draw();
        }
    }
    //Draw Entities -

    //Draw GUI +
    al_draw_filled_rectangle(0, mapDisplayHeight, screenWidth, screenHeight, al_map_rgb(0,0,0));

    fpsTimeNew = al_get_time();
    fpsCounter = 1/(fpsTimeNew - fpsTimeOld);
    fpsTimeOld = fpsTimeNew;
    al_draw_textf(defaultFont, (fpsCounter > 55) ? al_map_rgb(50, 150, 50) : (fpsCounter <= 55 && fpsCounter > 30) ? al_map_rgb(150, 150, 50) : al_map_rgb(150, 50, 50), screenWidth-95, mapDisplayHeight, NULL, "FPS: %d", (int)round(fpsCounter));
}
