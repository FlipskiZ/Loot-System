#ifndef LIVINGENTITY_H
#define LIVINGENTITY_H

#include "Entity.h"
#include <vector>

class LivingEntity : public Entity{
    public:
        LivingEntity();
        void updateDebuffs();
        virtual void takeDamage(double damage, bool crit, double armorBypass = 0) = 0;
        virtual void takeDebuffDamage(double damage, int debuffID) = 0;
        virtual void healHP(double health) = 0;

        double getCurrentHP();
        double getMaxHP();
        double getArmor();
        std::vector<std::vector<double>> getDebuffs();

        void setMaxHP(double maxHP);
        void setCurrentHP(double currentHP);
        void setArmor(double armor);
        void addDebuff(int debuffID, double debuffStrength, double debuffDuration);
        void removeDebuff(int debuffID);

    protected:
        double livingHP, livingMaxHP, livingArmor;

        std::vector<std::vector<double>> livingDebuffs;

        double timeAlive;
    private:
};

#endif // LIVINGENTITY_H
