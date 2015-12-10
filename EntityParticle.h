#ifndef ENTITYPARTICLE_H
#define ENTITYPARTICLE_H

#include "Entity.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <string>

class EntityParticle : public Entity{
    public:
        EntityParticle();
        void update();
        void draw();
        void updateMovePatternDelta();

        int getMovePattern();
        double getTimeAlive();
        double getDeathTime();
        double getFriction();
        bool getCollidesWithMap();
        std::string getShowText();
        ALLEGRO_FONT *getFont();
        ALLEGRO_COLOR getColor();

        void setMovePattern(int movePattern);
        void setDeathTime(double deathTime);
        void setFriction(double frictionValue);
        void setGravity(double gravity);
        void setCollidesWithMap(bool collides);
        void setTextValue(std::string textValue, int fontValue);
        void setColor(ALLEGRO_COLOR colorValue);
    protected:
    private:
        int particleMovePattern;
        double particleDeathTime, particleTimeAlive;
        double particleFriction;
        double particleGravity;
        bool particleCollidesWithMap;
        bool particleIsText;
        std::string particleText;
        int particleFont;
        ALLEGRO_COLOR particleColor;
        double particleFrictionRatio;
};

#endif // ENTITYPARTICLE_H
