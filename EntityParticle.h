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

        int getMovePattern();
        double getTimeAlive();
        double getDeathTime();
        double getFriction();
        std::string getShowText();
        ALLEGRO_FONT *getFont();
        ALLEGRO_COLOR getColor();

        void setMovePattern(int movePattern);
        void setDeathTime(double deathTime);
        void setFriction(double frictionValue);
        void setTextValue(std::string textValue, int fontValue);
        void setColor(ALLEGRO_COLOR colorValue);
    protected:
    private:
        int particleMovePattern;
        double particleDeathTime, particleTimeAlive;
        double particleFriction;
        bool particleIsText;
        std::string particleText;
        int particleFont;
        ALLEGRO_COLOR particleColor;
};

#endif // ENTITYPARTICLE_H
