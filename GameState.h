#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Engine.h"

class GameState{
    public:
        virtual void init() = 0;
        virtual void cleanup() = 0;

        virtual void pause() = 0;
        virtual void resume() = 0;

        virtual void update(Engine* engine) = 0;
        virtual void draw(Engine* engine) = 0;

        void changeState(Engine* engine, GameState* gameState){
            engine->changeState(gameState);
        }

    protected:
        GameState(){}
};

#endif
