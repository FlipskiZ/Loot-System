#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"
#include "InventoryState.h"

InventoryState InventoryState::m_InventoryState;

void InventoryState::init(){
    updateInventoryPlacement();
}
void InventoryState::cleanup(){

}

void InventoryState::pause(){

}
void InventoryState::resume(){

}

void InventoryState::update(Engine* engine){
    compareItems = false;

    if(al_key_down(&keyState, ALLEGRO_KEY_I)){
        if(lastKeyPress != ALLEGRO_KEY_I){
            engine->popState();

            lastKeyPress = ALLEGRO_KEY_I;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
        if(lastKeyPress != ALLEGRO_KEY_ESCAPE){
            engine->popState();

            lastKeyPress = ALLEGRO_KEY_ESCAPE;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_N)){
        if(lastKeyPress != ALLEGRO_KEY_N){
            playerList[0]->clearInventory();

            lastKeyPress = ALLEGRO_KEY_N;
        }
    }/*else if(al_key_down(&keyState, ALLEGRO_KEY_G)){
        if(lastKeyPress != ALLEGRO_KEY_G){
            lootSystem.createWeapon(playerList[0]->getPlayerLevel());

            int inventoryItem = 0;
            for(int i = 0; i < playerList[0]->getMaxInventorySpace(); i++){
                inventoryItem = playerList[0]->getInventoryItem(i);
                if(inventoryItem != -1 && itemList[inventoryItem] != NULL && itemList[inventoryItem]->getActive()){
                    itemList[inventoryItem]->setPos((i%10+1)*68, ((trunc((double)i/10))+3)*68); //Sets the weapon positions according to their location in the inventory vector
                }
            }

            lastKeyPress = ALLEGRO_KEY_G;
        }
    }*/

    if(al_key_down(&keyState, ALLEGRO_KEY_LSHIFT)){
        compareItems = true;
    }

    if(mouseButtonRightClick){
        int inventoryItem = 0;
        for(int i = 0; i < playerList[0]->getMaxInventorySpace(); i++){
            inventoryItem = playerList[0]->getInventoryItem(i);
            if(inventoryItem != -1 && itemList[inventoryItem] != NULL && itemList[inventoryItem]->getActive()){
                if(checkCollision(mouseX, mouseY, itemList[inventoryItem]->getPosition(0), itemList[inventoryItem]->getPosition(1), 0, 0, itemList[inventoryItem]->getDimension(0), itemList[inventoryItem]->getDimension(1))){
                    playerList[0]->removeItemFromInventory(inventoryItem);
                    itemList[inventoryItem] = NULL;
                }
            }
        }
    }

    /*if(al_key_down(&keyState, ALLEGRO_KEY_COMMA)){
        if(playerList[0]->getPlayerLevel() > 1){
            playerList[0]->setPlayerLevel(playerList[0]->getPlayerLevel()-1);
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_FULLSTOP)){
        if(playerList[0]->getPlayerLevel() < 100){
            playerList[0]->setPlayerLevel(playerList[0]->getPlayerLevel()+1);
        }
    }*/

    int maxInventorySpace = playerList[0]->getMaxInventorySpace();
    int inventoryItem = 0;
    for(int i = 0; i < maxInventorySpace; i++){
        inventoryItem = playerList[0]->getInventoryItem(i);
        if(inventoryItem != -1 && itemList[inventoryItem] != NULL && itemList[inventoryItem]->getActive()){
            itemList[inventoryItem]->update();
        }
    }

    int equipmentPiece = 0;
    for(int i = 0; i < AMOUNT_ARMOR_PIECES+1; i++){
        if(i == 0){
            equipmentPiece = playerList[0]->getPlayerEquippedWeapon();
        }else{
            equipmentPiece = playerList[0]->getPlayerEquippedArmor(i-1);
        }
        if(equipmentPiece != -1 && itemList[equipmentPiece] != NULL && itemList[equipmentPiece]->getActive() && ( (i == 0 && playerList[0]->getPlayerHasWeaponEquipped()) || (
                i > 0 && playerList[0]->getPlayerHasArmorEquipped(i-1)))){
            itemList[equipmentPiece]->update();
        }
    }
}
void InventoryState::draw(Engine* engine){

    //Draw Inventory Grid
    int maxInventorySpace = playerList[0]->getMaxInventorySpace();
    for(int y = 5; y < ceil((double)maxInventorySpace/10)+5; y++){ //The +1s are for offsetting the grid by 64 pixels down and to the right
        for(int x = 0; x < 10 && x < maxInventorySpace+1; x++){
            al_draw_filled_rectangle(x*68+16, y*68+16, x*68+64+16, y*68+64+16, al_map_rgb(127, 127, 127)); //Draw the grid with 4 pixels space in between them
        }
    }

    for(int y = 3; y < 4; y++){ //The +1s are for offsetting the grid by 64 pixels down and to the right
        for(int x = 0; x < AMOUNT_ARMOR_PIECES+1; x++){
            al_draw_filled_rectangle(x*68+16, y*68+16, x*68+64+16, y*68+64+16, al_map_rgb(127, 127, 127)); //Draw the grid with 4 pixels space in between them
        }
    }

    int inventoryItem = 0;
    for(int i = 0; i < maxInventorySpace; i++){
        inventoryItem = playerList[0]->getInventoryItem(i);
        if(inventoryItem != -1 && itemList[inventoryItem] != NULL && itemList[inventoryItem]->getActive()){
            itemList[inventoryItem]->draw();
        }
    }

    int equipmentPiece = 0;
    for(int i = 0; i < AMOUNT_ARMOR_PIECES+1; i++){
        if(i == 0){
            equipmentPiece = playerList[0]->getPlayerEquippedWeapon();
        }else{
            equipmentPiece = playerList[0]->getPlayerEquippedArmor(i-1);
        }
        if(equipmentPiece != -1 && itemList[equipmentPiece] != NULL && itemList[equipmentPiece]->getActive() && ( (i == 0 && playerList[0]->getPlayerHasWeaponEquipped()) || (
                i > 0 && playerList[0]->getPlayerHasArmorEquipped(i-1)))){
            itemList[equipmentPiece]->draw();
        }
    }

    al_draw_textf(defaultFont, al_map_rgb(127, 127, 127), screenWidth, 20, ALLEGRO_ALIGN_RIGHT, "Current Level: %.2f", currentLevel);

    fpsTimeNew = al_get_time();
    fpsCounter = 1/(fpsTimeNew - fpsTimeOld);
    fpsTimeOld = fpsTimeNew;
    al_draw_textf(defaultFont, (fpsCounter > 55) ? al_map_rgb(50, 150, 50) : (fpsCounter <= 55 && fpsCounter > 30) ? al_map_rgb(150, 150, 50) : al_map_rgb(150, 50, 50), screenWidth-95, mapDisplayHeight, NULL, "FPS: %d", (int)round(fpsCounter));

    if(drawItemIDInformationBox != -1 && itemList[drawItemIDInformationBox] != NULL && itemList[drawItemIDInformationBox]->getActive() && itemList[drawItemIDInformationBox]->getHoveringOver()){
        itemList[drawItemIDInformationBox]->drawInformationBox();
    }
}
