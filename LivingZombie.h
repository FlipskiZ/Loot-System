#ifndef LIVINGZOMBIE_H
#define LIVINGZOMBIE_H

#include "LivingEntity.h"

class LivingZombie : public LivingEntity{
    public:
        LivingZombie();
        void update();
        void draw();
        void takeDamage(double damage, bool crit, double armorBypass = 0);
        void takeDebuffDamage(double damage, int debuffID);
        void healHP(double health);

        std::vector<int> livingCollisions;
    protected:
    private:
};

#endif // LIVINGZOMBIE_H
