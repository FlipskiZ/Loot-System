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
    double maxHP = 10, armor = 0;
    playerList.resize(1);
    playerList[0] = unique_ptr<LivingPlayer>(new LivingPlayer());
    playerList[0]->setPos(mapArrayWidth*tileSize/2-width/2, mapArrayHeight*tileSize/2-height/2);
    playerList[0]->setDimensions(width, height);
    playerList[0]->setMovementSpeed(movementSpeed);
    playerList[0]->setMaxHP(maxHP);
    playerList[0]->setArmor(armor);
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
    for(int i = 0; i < livingList.size(); i++){
        livingList[i] = NULL;
    }
    for(int i = 0; i < specialTileList.size(); i++){
        specialTileList[i] = NULL;
    }
    collisionDetection = unique_ptr<CollisionDetection>(new CollisionDetection(0, 0, 0, mapArrayWidth*tileSize, mapArrayHeight*tileSize)); //Level, PosX, PosY, Width, Height

    worldPosition.resize(2);
    worldPosition[0] = 0, worldPosition[1] = 0;

    changeWorldSegment(moveUp);
    visitedWorldPositions.clear();
    changeWorldSegment(moveDown);

    currentLevel = 1;

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
    collisionDetection->clearNode();
    for(int i = 0; i < livingList.size(); i++){
        if(livingList[i] != NULL && livingList[i]->getActive() && livingList[i]->getWorldPosition() == worldPosition){
            collisionDetection->insertObjectInNode(i);
        }
    }

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
        playerList[0]->setDeltaX(playerList[0]->getDelta(0)/deltaTime / twoSqrt);
        playerList[0]->setDeltaY(playerList[0]->getDelta(1)/deltaTime / twoSqrt);
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
        playerList[0]->setAngle(-atan2(playerList[0]->getCenterPosition(0) + cameraOffsetX - mouseX, playerList[0]->getCenterPosition(1) + cameraOffsetY - mouseY));
        playerList[0]->setAnimationValue(2, false, 1);
        if(mouseButtonLeft){
            playerList[0]->fireWeapon();
        }
    }
    /*
    if(al_key_down(&keyState, ALLEGRO_KEY_G)){
        if(lastKeyPress != ALLEGRO_KEY_G){
        double width = 24, height = 24, movementSpeed = 64, sheetColums = 4, sheetRows = 3, animationSpeed = 0.25;
        double maxHP = randInt(enemyLevel*10, enemyLevel*50), armor = randInt(enemyLevel*2, enemyLevel*5);
        unique_ptr<LivingZombie> newZombie(new LivingZombie());
        newZombie->setPos(mouseX-cameraOffsetX-width/2, mouseY-cameraOffsetY-height/2);
        newZombie->setDimensions(width, height);
        newZombie->setMovementSpeed(movementSpeed);
        newZombie->setMaxHP(maxHP);
        newZombie->setArmor(armor);
        newZombie->setSheetDimensions(sheetColums, sheetRows, width, height);
        newZombie->setAnimationSpeed(animationSpeed);
        newZombie->setBitmap(zombieImage);
        addZombieToList(move(newZombie));

        lastKeyPress = ALLEGRO_KEY_G;
        }
    }*/

    if(al_key_down(&keyState, ALLEGRO_KEY_F5)){
        if(lastKeyPress != ALLEGRO_KEY_F5){
            loadMapArray();
            lastKeyPress = ALLEGRO_KEY_F5;
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
    }else if(al_key_down(&keyState, ALLEGRO_KEY_E)){
        if(lastKeyPress != ALLEGRO_KEY_E){
            playerList[0]->playerUseKey();
            lastKeyPress = ALLEGRO_KEY_E;
        }
    }
    /*
    if(al_key_down(&keyState, ALLEGRO_KEY_COMMA)){
        if(enemyLevel > 1){
            enemyLevel--;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_FULLSTOP)){
        if(enemyLevel < 100){
            enemyLevel++;
        }
    }*/
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
    for(int i = 0; i < specialTileList.size(); i++){
        if(specialTileList[i] != NULL && specialTileList[i]->getActive()){
            specialTileList[i]->update();
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
    for(int i = 0; i < specialTileList.size(); i++){
        if(specialTileList[i] != NULL && specialTileList[i]->getActive()){
            specialTileList[i]->draw();
        }
    }
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

    //Draw Health Bar
    al_draw_rectangle(2.5, mapDisplayHeight+2.5, 360-2.5, screenHeight-2.5, al_map_rgb(255,255,255), 5);
    al_draw_filled_rectangle(5, mapDisplayHeight+5, (360-10)*(playerList[0]->getCurrentHP()/playerList[0]->getMaxHP())+5, screenHeight-5, al_map_rgb(215, 0, 0));
    al_draw_textf(defaultFont, al_map_rgb(255, 255, 255), 360/2, mapDisplayHeight+(screenHeight-mapDisplayHeight)/2-al_get_font_ascent(defaultFont)/2, ALLEGRO_ALIGN_CENTER, "%.2f/%.2f", playerList[0]->getCurrentHP(), playerList[0]->getMaxHP());

    //This is used for making a black to white transformation of the text as the health bar is going down. So that the text is black on the red and white on the black.
    al_set_target_bitmap(healthText);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    al_draw_textf(defaultFont, al_map_rgb(0, 0, 0), 360/2, 0, ALLEGRO_ALIGN_CENTER, "%.2f/%.2f", playerList[0]->getCurrentHP(), playerList[0]->getMaxHP());
    al_set_target_bitmap(al_get_backbuffer(display));

    al_destroy_bitmap(healthTextHelper);
    healthTextHelper = al_create_sub_bitmap(healthText, 0, 0, (360-10)*(playerList[0]->getCurrentHP()/playerList[0]->getMaxHP())+5, al_get_bitmap_height(healthText));

    al_draw_bitmap(healthTextHelper, 0, mapDisplayHeight+(screenHeight-mapDisplayHeight)/2-al_get_font_ascent(defaultFont)/2, 0);
    //Finish Drawing Health Bar

    //This is used to draw a rotating arrow that points to the nearest chest.
    int closestChestId = 0;
    double distanceFromPlayer = INT_MAX;
    double oldClosestDistace = INT_MAX;
    bool chestExists = false;

    for(int i = 0; i < specialTileList.size(); i++){
        if(specialTileList[i] != NULL && specialTileList[i]->getActive() && specialTileList[i]->getWorldPosition() == worldPosition){
            chestExists = true;

            distanceFromPlayer = pow(abs(specialTileList[i]->getCenterPosition(0)-playerList[0]->getCenterPosition(0)), 2) + pow(abs(specialTileList[i]->getCenterPosition(1)-playerList[0]->getCenterPosition(1)), 2);

            if(distanceFromPlayer < oldClosestDistace){
                closestChestId = i;
                oldClosestDistace = distanceFromPlayer;
            }
        }
    }

    if(chestExists){
        double cX = playerList[0]->getDimension(0), cY = playerList[0]->getDimension(0);
        double oX = playerList[0]->getPosition(0)+cameraOffsetX+playerList[0]->getDimension(0)/2, oY = playerList[0]->getPosition(1)+cameraOffsetY+playerList[0]->getDimension(1)/2;

        double angle = -atan2(playerList[0]->getCenterPosition(0)-specialTileList[closestChestId]->getCenterPosition(0), playerList[0]->getCenterPosition(1)-specialTileList[closestChestId]->getCenterPosition(1));
        double angleSin = sin(angle), angleCos = -cos(angle);

        oX += angleSin*cX;
        oY += angleCos*cY;

        al_draw_rotated_bitmap(arrowImage, 8, 8, oX, oY, angle, 0);
    }
    //Finish drawing the arrow

    fpsTimeNew = al_get_time();
    fpsCounter = 1/(fpsTimeNew - fpsTimeOld);
    fpsTimeOld = fpsTimeNew;
    al_draw_textf(defaultFont, (fpsCounter > 55) ? al_map_rgb(50, 150, 50) : (fpsCounter <= 55 && fpsCounter > 30) ? al_map_rgb(150, 150, 50) : al_map_rgb(150, 50, 50), screenWidth-95, mapDisplayHeight, NULL, "FPS: %d", (int)round(fpsCounter));
    al_draw_textf(defaultFont, al_map_rgb(127, 127, 127), screenWidth, mapDisplayHeight+25, ALLEGRO_ALIGN_RIGHT, "Current Level: %.2f", currentLevel);
    al_draw_textf(defaultFont, al_map_rgb(127, 127, 127), screenWidth, mapDisplayHeight+50, ALLEGRO_ALIGN_RIGHT, "World Position X: %d Y: %d", worldPosition[0], worldPosition[1]);

    printf("Player X: %f Y: %f\n", playerList[0]->getPosition(0), playerList[0]->getPosition(1));

    //collisionDetection->draw();
}
