#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

//Initalization +
struct TILE_TYPE{
	bool isPassable;
};

void loadConfig();
bool isPassable(double x, double y, double width, double height, double deltaX, double deltaY);
bool checkCollision(double x, double y, double ex, double ey, double width, double height, double ewidth, double eheight);
bool insideMap(double x, double y, double width, double height);
int addButtonToList(unique_ptr<Button> &&newButton);
int addWeaponToList(unique_ptr<ItemWeapon> &&newWeapon);
int addBulletToList(unique_ptr<MissileBullet> &&newBullet);
void loadMapArray();
void saveMapArray();
void drawMap();
void drawTile(double x, double y, int tileId);
void updateCamera();

ALLEGRO_DISPLAY *display;

ALLEGRO_FONT *defaultFont;
ALLEGRO_FONT *smallFont;
ALLEGRO_FONT *bigFont;

ALLEGRO_BITMAP *cursorImage;
ALLEGRO_BITMAP *playerImage;
ALLEGRO_BITMAP *bulletImage;
ALLEGRO_BITMAP *explosionImage;

ALLEGRO_KEYBOARD_STATE keyState;
ALLEGRO_MOUSE_STATE mouseState;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;

vector<unique_ptr<Button>> buttonList;
vector<unique_ptr<LivingPlayer>> playerList;
vector<unique_ptr<Item>> itemList;
vector<unique_ptr<MissileEntity>> missileList;

Engine engine;

double screenWidth, screenHeight, mapDisplayWidth, mapDisplayHeight, mapArrayWidth, mapArrayHeight;
int tileSize;
int MAX_BUTTONS, MAX_PLAYERS, MAX_ITEMS, MAX_MISSILES;
double FPS, ticksPerSecond;

vector<vector<int> > mapArray;

const char* versionNumber;
double fpsTimeNew, fpsCounter, fpsTimeOld;
double deltaTime;

double cameraPosX, cameraPosY, cameraOffsetX, cameraOffsetY, mapDisplayOffsetX, mapDisplayOffsetY;
bool drawScreen, timerEvent, done, mouseButtonLeft, mouseButtonLeftClick, mouseButtonRight, mouseButtonRightClick, inGame;
double mouseX, mouseY, volumeLevel;
int lastKeyPress, mouseWheel = 0;

TILE_TYPE tileIndex[] = {
	{true}, // (0) TILE_GROUND1
	{false}, // (1) TILE_WALL
};
//Initalization -

int main(){
    srand(time(0));

	Engine engine;

    loadConfig();

	//Initialize the Engine
	engine.init("Loot System", screenWidth, screenHeight, false);

	//Load the Menu
	engine.changeState(PlayState::instance());

    //Timestep Variables
    double t = 0.0;
    deltaTime = 1/ticksPerSecond;

    double currentTime = 0.0;
    double newTime = 0.0;
    double frameTime = 0.0;

    double accumulator = 0.0;

	//Main Loop
    while(engine.running()){
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        timerEvent = false;

        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            engine.quit();
        }

        if(events.type == ALLEGRO_EVENT_TIMER){
            timerEvent = true;
        }
        //Main Timer Event +
        if(timerEvent){
            if(events.timer.source == timer){
                newTime = al_get_time();
                frameTime = newTime - currentTime;
                if(frameTime > 0.25)
                    frameTime = 0.25;	  // note: max frame time to avoid spiral of death
                currentTime = newTime;

                accumulator += frameTime;

                while(accumulator >= deltaTime){
                    al_get_mouse_state(&mouseState);

                    al_get_keyboard_state(&keyState);

                    //Update Mouse Variables +
                    mouseX = al_get_mouse_state_axis(&mouseState,0);
                    mouseY = al_get_mouse_state_axis(&mouseState,1);
                    mouseWheel = al_get_mouse_state_axis(&mouseState, 2);

                    if(al_mouse_button_down(&mouseState, 1)){
                        mouseButtonLeft = true;
                        mouseButtonLeftClick = false;
                    }else if(mouseButtonLeft && events.timer.source == timer){
                        mouseButtonLeftClick = true;
                        mouseButtonLeft = false;
                    }else{
                        mouseButtonLeftClick = false;
                    }

                    if(al_mouse_button_down(&mouseState, 2)){
                        mouseButtonRight = true;
                        mouseButtonRightClick = false;
                    }else if(mouseButtonRight && events.timer.source == timer){
                        mouseButtonRightClick = true;
                        mouseButtonRight = false;
                    }else{
                        mouseButtonRightClick = false;
                    }
                    //Update Mouse Variables -

                    //Rest +
                    if(!al_key_down(&keyState, lastKeyPress)){
                        lastKeyPress = 0;
                    }
                    //Rest -
                    engine.update(); //Call the gameState specfic update
                    t += deltaTime;
                    accumulator -= deltaTime;
                }

                engine.draw();

                al_draw_bitmap(cursorImage, mouseX-4, mouseY-4, 0);

                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }
        }
        //Main Timer Event -
    }
	// cleanup the engine
	engine.cleanup();

}

void loadConfig(){
    screenWidth = 1280, screenHeight = 720, mapDisplayWidth = 1280, mapDisplayHeight = 640, mapArrayWidth = 40, mapArrayHeight = 20, tileSize = 32;
    MAX_BUTTONS = 25, MAX_PLAYERS = 1, MAX_ITEMS = 1000, MAX_MISSILES = 1000;
    FPS = 60, ticksPerSecond = 64;

    string desc;

    fstream configFile("config/config.ini");

    if(!configFile.is_open()){
        ofstream temp("config/config.ini");
        temp.close();

        configFile.open("config/config.ini");
        configFile << "ScreenWidth "        << screenWidth << endl;
        configFile << "screenHeight "       << screenHeight << endl;
        configFile << "mapDisplayWidth "    << mapDisplayWidth << endl;
        configFile << "mapDisplayHeight "   << mapDisplayHeight << endl;
        configFile << "mapArrayWidth "      << mapArrayWidth << endl;
        configFile << "mapArrayHeight "     << mapArrayHeight << endl;
        configFile << "tileSize "           << tileSize << endl;
        configFile << "FPS "                << FPS << endl;
        configFile << "ticksPerSecond "     << ticksPerSecond << endl;
        configFile << "MAX_BUTTONS "        << MAX_BUTTONS << endl;
        configFile << "MAX_PLAYERS "        << MAX_PLAYERS << endl;
    }else{
        configFile >> desc >> screenWidth;
        configFile >> desc >> screenHeight;
        configFile >> desc >> mapDisplayWidth;
        configFile >> desc >> mapDisplayHeight;
        configFile >> desc >> mapArrayWidth;
        configFile >> desc >> mapArrayHeight;
        configFile >> desc >> tileSize;
        configFile >> desc >> FPS;
        configFile >> desc >> ticksPerSecond;
        configFile >> desc >> MAX_BUTTONS;
        configFile >> desc >> MAX_PLAYERS;
    }

    mapArray.resize(mapArrayWidth);
    for(int i = 0; i < mapArrayWidth; i++){
        mapArray[i].resize(mapArrayHeight);
    }
}

bool isPassable(double x, double y, double width, double height){
    if(!insideMap(x, y, width, height)){
        return false;
    }else{
        int tX = trunc(x/tileSize), tY = trunc(y/tileSize), tWX = trunc((x+width-1)/tileSize), tHY = trunc((y+height-1)/tileSize);

        if(!tileIndex[mapArray[tX][tY]].isPassable || !tileIndex[mapArray[tWX][tY]].isPassable || !tileIndex[mapArray[tX][tHY]].isPassable || !tileIndex[mapArray[tWX][tHY]].isPassable){
            return false;
        }
    }
    return true;
}

bool checkCollision(double x, double y, double ex, double ey, double width, double height, double ewidth, double eheight){
    if(x + width - 1 < ex || x > ewidth + ex - 1 || y + height - 1 < ey || y > eheight + ey - 1){
        return false;
    }else{
        return true;
    }
}

bool insideMap(double x, double y, double width, double height){
    if(x < 0 || x + width-1 >= mapArrayWidth*tileSize || y < 0 || y + height-1 >= mapArrayHeight*tileSize){
        return false;
    }
    return true;
}

int addButtonToList(unique_ptr<Button> &&newButton){
    for(int i = 0; i < buttonList.size()+1; i++){
        if(i < MAX_BUTTONS && i >= buttonList.size()){
            newButton->setEntityId(i);
            buttonList.push_back(move(newButton));
            return i;
        }else if(buttonList[i] == NULL || (i < MAX_BUTTONS && !buttonList[i]->getActive())){
            newButton->setEntityId(i);
            buttonList[i] = move(newButton);
            return i;
        }
    }
}

int addWeaponToList(unique_ptr<ItemWeapon> &&newWeapon){
    for(int i = 0; i < itemList.size()+1; i++){
        if(i < MAX_ITEMS && i >= itemList.size()){
            newWeapon->setEntityId(i);
            itemList.push_back(move(newWeapon));
            return i;
        }else if(itemList[i] == NULL || (i < MAX_ITEMS && !itemList[i]->getActive())){
            newWeapon->setEntityId(i);
            itemList[i] = move(newWeapon);
            return i;
        }
    }
}

int addBulletToList(unique_ptr<MissileBullet> &&newBullet){
    for(int i = 0; i < missileList.size()+1; i++){
        if(i < MAX_MISSILES && i >= missileList.size()){
            newBullet->setEntityId(i);
            missileList.push_back(move(newBullet));
            return i;
        }else if(missileList[i] == NULL || (i < MAX_MISSILES && !missileList[i]->getActive())){
            newBullet->setEntityId(i);
            missileList[i] = move(newBullet);
            return i;
        }
    }
}

void loadMapArray(){
    ifstream mapArrayFile("config/MapArray.txt");

    for(int y = 0; y < mapArrayHeight; y++){
        for(int x = 0; x < mapArrayWidth; x++){
            mapArrayFile >> mapArray[x][y];
        }
    }

    mapArrayFile.close();
}

void saveMapArray(){
    ofstream mapArrayFile("config/MapArray.txt");

    for(int y = 0; y < mapArrayHeight; y++){
        for(int x = 0; x <= mapArrayWidth; x++){
            if(x < mapArrayWidth){
                mapArrayFile << mapArray[x][y] << " ";
            }else if(x == mapArrayWidth){
                mapArrayFile << endl;
            }
        }
    }

    mapArrayFile.close();
}

void drawMap(){
    for(int x = 0; x < mapDisplayWidth/tileSize+1; x++){
        for(int y = 0; y < mapDisplayHeight/tileSize+1; y++){
            int mapOffsetX = cameraPosX/tileSize, mapOffsetY = cameraPosY/tileSize;
            if(x+mapOffsetX >= mapArrayWidth){
                mapOffsetX--;
            }if(y+mapOffsetY >= mapArrayHeight){
                mapOffsetY--;
            }
            drawTile(x+mapDisplayOffsetX/tileSize, y+mapDisplayOffsetY/tileSize, mapArray[x+mapOffsetX][y+mapOffsetY]);
        }
    }
}

void drawTile(double x, double y, int tileId){
    switch(tileId){
        case 0:
            //al_draw_rotated_bitmap(groundImage1, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[truncx][truncy]*90*toRadians, NULL);
            al_draw_filled_rectangle(x*tileSize, y*tileSize, x*tileSize+tileSize, y*tileSize+tileSize, al_map_rgb(150, 150, 150));
            break;

        case 1:
            //al_draw_rotated_bitmap(groundImage2, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[truncx][truncy]*90*toRadians, NULL);
            al_draw_filled_rectangle(x*tileSize, y*tileSize, x*tileSize+tileSize, y*tileSize+tileSize, al_map_rgb(100, 100, 100));
            break;
    }
}

void updateCamera(){
    cameraPosX = playerList[0]->getPosition()[0]-mapDisplayWidth/2+playerList[0]->getDimension()[0]/2;
    cameraPosY = playerList[0]->getPosition()[1]-mapDisplayHeight/2+playerList[0]->getDimension()[1]/2;

    if(cameraPosX < 0){
        cameraPosX = 0;
    }
    if(cameraPosX + mapDisplayWidth >= mapArrayWidth*tileSize){
        cameraPosX = mapArrayWidth*tileSize - mapDisplayWidth;
    }
    if(cameraPosY < 0){
        cameraPosY = 0;
    }
    if(cameraPosY + mapDisplayHeight >= mapArrayHeight*tileSize){
        cameraPosY = mapArrayHeight*tileSize - mapDisplayHeight;
    }

    int cX = cameraPosX, cY = cameraPosY;

    mapDisplayOffsetX = -(cX%tileSize);
    mapDisplayOffsetY = -(cY%tileSize);

    cameraOffsetX = -cameraPosX;
    cameraOffsetY = -cameraPosY;
}