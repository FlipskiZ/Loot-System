#ifndef MISSILEBULLET_H
#define MISSILEBULLET_H

#include "MissileEntity.h"

#include <vector>

class MissileBullet : public MissileEntity{
    public:
        MissileBullet();
        ~MissileBullet(){}
        void update();
        void draw();

        double getMissileStat(int stat){return this->bulletStats[stat];}
        double getMissileSpecial(int special){return this->bulletSpecials[special];}

        void setMissileStats(std::vector<double> missileStats);
        void setMissileSpecials(std::vector<double> missileSpecials);
        void setMissilePlayerOwned(bool playerOwned){this->playerOwned = playerOwned;}
    protected:
    private:
        bool playerOwned;

        std::vector<double> bulletStats;
        /*double damage;
        double minDamage;
        double maxDamage;
        double fireRate;
        double shotSpeed;
        double accuracy;
        double critChance;
        double critStrength;
        double armorPenetration;*/

        std::vector<double> bulletSpecials;
        /*double fireStrength; //Burning
        double poisionStrength; //Poison
        double electricStrength; //Lightning. Bounces to closest target, losing a percentage of power depending on the distance
        double slowStrenght; //Slowness amount. Decreases by a constant amount over time
        double additionalBullets; //Additional bullets in a 45 degree spread
        double explosionRadius; //Decreases in damage the further out from center the target is
        double bulletSplit; //Splits after a short while
        double ricochetAmount; //Bounces off of walls
        double homingForce; //Depends on ease of implementation. Homes on closest enemy
        double vampirism; //Steals a percentage of the base damage dealt every bullet hit
        double executionTreshold; //Instant kill at health percantage
        double penetrationAmount; //Amount of times the bullet can penetrate enemies*/

        double rangeBeforeSplit, rangeBeforeSplitHelper;
        double explosionLinger, explosionLingerCount;
        bool explode;
};

#endif // MISSILEBULLET_H
