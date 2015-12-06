#ifndef LIVINGENTITY_H
#define LIVINGENTITY_H

#include "Entity.h"

class LivingEntity : public Entity{
    public:
        LivingEntity();

        double getCurrentHP();
        double getMaxHP();

        void setMaxHP(double maxHP);
        void setCurrentHP(double currentHP);
        virtual void takeDamage(double damage, bool crit) = 0;

    protected:
        double livingHP, livingMaxHP;

    private:
};

#endif // LIVINGENTITY_H
