#ifndef LIVINGZOMBIE_H
#define LIVINGZOMBIE_H

#include "LivingEntity.h"

class LivingZombie : public LivingEntity{
    public:
        LivingZombie();
        void update();
        void draw();
        void takeDamage(double damage, bool crit);

    protected:
    private:
};

#endif // LIVINGZOMBIE_H
