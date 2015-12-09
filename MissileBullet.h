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
        /*double damage; //check
        double minDamage; //check
        double maxDamage; //check
        double fireRate; //check
        double shotSpeed; //check
        double accuracy; //check
        double critChance;
        double critStrength;
        double armorPenetration;*/

        std::vector<double> bulletSpecials;
        /*double fireStrength; //Burning
        double poisionStrength; //Poison
        double electricStrength; //Lightning. Bounces to closest target, losing a percentage of power depending on the distance
        double slowStrenght; //Slowness amount. Decreases by a constant amount over time
        double additionalBullets; //Additional bullets in a 45 degree spread                    //check
        double explosionRadius; //Decreases in damage the further out from center the target is //check
        double bulletSplit; //Splits after a short while                                        //check
        double ricochetAmount; //Bounces off of walls                                           //check
        double homingForce; //Depends on ease of implementation. Homes on closest enemy
        double vampirism; //Steals a percentage of the base damage dealt every bullet hit
        double executionTreshold; //Instant kill at health percantage
        double penetrationAmount; //Amount of times the bullet can penetrate enemies*/          //check

        double rangeBeforeSplit, rangeBeforeSplitHelper;
        double explosionLinger, explosionLingerCount;
        bool explode;

        bool tileColX, tileColY;
        bool livingColX, livingColY;
        bool richochetX, richochetY;
        std::vector<int> penetratedEnemies; //Keeps trach of currently penetrating enemies
        std::vector<int> loopPenetratedEnemies; //This is in case the bullet hits multiple enemies in a loop

        std::vector<int> livingCollisions;
};

#endif // MISSILEBULLET_H
