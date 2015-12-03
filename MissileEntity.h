#ifndef MISSILEENTITY_H
#define MISSILEENTITY_H

#include "Entity.h"

#include <vector>

class MissileEntity : public Entity{
    public:
        MissileEntity();

        virtual double getMissileStat(int stat) = 0;
        virtual double getMissileSpecial(int special) = 0;

        virtual void setMissileStats(std::vector<double> missileStats) = 0;
        virtual void setMissileSpecials(std::vector<double> missileStats) = 0;
        virtual void setMissilePlayerOwned(bool playerOwned) = 0;

    protected:
    private:
};

#endif // MISSILEENTITY_H
