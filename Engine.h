#ifndef ENGINE_H
#define ENGINE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include <algorithm>
#include <memory>

#include "Button.h"
#include "LootSystem.h"
#include "CollisionDetection.h"

#include "LivingPlayer.h"
#include "LivingZombie.h"

#include "MissileBullet.h"

#include "EntityParticle.h"

#include "ItemWeapon.h"
#include "TileContainer.h"

#define PI 3.14159265359
#define toRadians 0.0174532925
#define twoSqrt 1.414213562373

using namespace std;

enum itemRarity{
    itemCommon = 0,
    itemUncommon,
    itemRare,
    itemEpic,
    itemLegendary,
    itemUnique
};

enum weaponStats{
    weaponMinDamage = 0,
    weaponMaxDamage,
    weaponFireRate,
    weaponShotSpeed,
    weaponAccuracy,
    weaponCritChance,
    weaponCritStrength,
    weaponArmorPenetration,
    AMOUNT_WEAPON_STATS //Amount of entries
};

enum weaponSpecials{
    weaponFireStrength = 0, //Burning
    weaponPoisionStrength, //Poison
    weaponElectricStrength, //Lightning. Bounces to closest target, losing a percentage of power depending on the distance
    weaponSlowStrenght, //Slowness amount. Decreases by a constant amount over time
    weaponAdditionalBullets,
    weaponExplosionRadius, //Decreases in damage the further out from center the target is
    weaponBulletSplit, //Splits after a short while
    weaponRicochetAmount, //Bounces off of walls
    weaponHomingForce, //Depends on ease of implementation. Homes on closest enemy
    weaponVampirism, //Steals a percentage of the base damage dealt every bullet hit
    weaponExecutionTreshold, //Instant kill at health percantage
    weaponPenetrationAmount, //Amount of times the bullet can penetrate enemies
    AMOUNT_WEAPON_SPECIALS //Amount of entries
};

enum particlePatterns{
    patternHomeToPlayer = 0,
    patternHomeToMouse,
    patternKeepMomentum,
    patternGravity,
};

string itos(int arg); //converts an integer to a std::string
string dtos(double arg); //converts an float to a std::string
bool isPassable(double x, double y, double width, double height);
bool checkCollision(double x, double y, double ex, double ey, double width, double height, double ewidth, double eheight);
bool insideMap(double x, double y, double width, double height);
int addButtonToList(unique_ptr<Button> &&newButton);
int addWeaponToList(unique_ptr<ItemWeapon> &&newWeapon);
int addBulletToList(unique_ptr<MissileBullet> &&newBullet);
int addZombieToList(unique_ptr<LivingZombie> &&newZombie);
int addParticleToList(unique_ptr<EntityParticle> &&newParticle);
void loadMapArray();
void saveMapArray();
void drawMap();
void drawTile(double x, double y, int tileId);
void updateCamera();

inline int randInt(int x,int y){return rand()%(y-x+1)+x;} //returns a random integer between x and y

inline double randDouble(double x = 0,double y = 1){return (rand())/(RAND_MAX+1.0)*(y-x)+x;} //returns a random double between x and y. Defaults to 0 and 1

inline bool randBool(){return randInt(0,1);}

extern ALLEGRO_DISPLAY *display;

extern ALLEGRO_FONT *defaultFont;
extern ALLEGRO_FONT *smallFont;
extern ALLEGRO_FONT *bigFont;

extern ALLEGRO_BITMAP *cursorImage;
extern ALLEGRO_BITMAP *playerImage;
extern ALLEGRO_BITMAP *bulletImage;
extern ALLEGRO_BITMAP *explosionImage;
extern ALLEGRO_BITMAP *zombieImage;

extern ALLEGRO_KEYBOARD_STATE keyState;
extern ALLEGRO_MOUSE_STATE mouseState;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *timer;

extern double screenWidth, screenHeight, mapDisplayWidth, mapDisplayHeight, mapArrayWidth, mapArrayHeight;
extern int tileSize;
extern int MAX_BUTTONS, MAX_PLAYERS, MAX_ITEMS, MAX_MISSILES, MAX_LIVING, MAX_PARTICLES, MAX_TREE_OBJECTS, MAX_TREE_LEVELS;
extern double FPS, ticksPerSecond;

extern vector<unique_ptr<Button>> buttonList;
extern vector<unique_ptr<LivingPlayer>> playerList;
extern vector<unique_ptr<LivingEntity>> livingList;
extern vector<unique_ptr<EntityParticle>> particleList;
extern vector<unique_ptr<Item>> itemList;
extern vector<unique_ptr<MissileEntity>> missileList;

extern unique_ptr<CollisionDetection> collisionDetection;

extern int collisionsC;

extern const char* versionNumber;
extern double fpsTimeNew, fpsCounter, fpsTimeOld;
extern double deltaTime;

extern double cameraPosX, cameraPosY, cameraOffsetX, cameraOffsetY, mapDisplayOffsetX, mapDisplayOffsetY;
extern bool drawScreen, timerEvent, done, mouseButtonLeft, mouseButtonLeftClick, mouseButtonRight, mouseButtonRightClick, inGame;
extern double mouseX, mouseY, volumeLevel;
extern int lastKeyPress, mouseWheel;

extern int enemyLevel;

extern vector<vector<int> > mapArray;

class GameState;

class Engine{
    public:
        void init(const char* title, int width, int height, bool fullscreen);
        void cleanup();

        void changeState(GameState* state);
        void pushState(GameState* state);
        void popState();

        void update();
        void draw();

        bool running(){ return m_running; }
        void quit(){m_running = false;}

    private:
        // the stack of states
        vector<GameState*> states;

        bool m_running;
        bool m_fullscreen;
};

#endif // ENGINE_H
