#ifndef LOOTSYSTEM_H
#define LOOTSYSTEM_H

#include <allegro5/allegro.h>

#include "string"
#include "vector"

class LootSystem{
    public:
        LootSystem();
        ~LootSystem(){}
        void init();
        int createWeapon(double level); //Return a weapon object in the future

        double getGunType(int type, int element);
        int getGunTypeAdditionalSpecial(int type);
        double getGunPrefix(int type, int element);
        double getGunSuffix(int type, int element);

        std::string getGunTypeName(int type);
        std::string getGunPrefixName(int prefix);
        std::string getGunSuffixName(int suffix);

        double getRandSpecialStrength(int special, double level);

        ALLEGRO_COLOR getRarityColor(int rarity);
    protected:
    private:
        int weaponRarity; //Normal - Uncommon - Rare - Epic - Legendary - Unique
        int weaponType;
        int weaponPrefix;
        int weaponSuffix;

        //Standard Weapon Statistics
        std::string name;
        std::vector<double> weaponStats;/*
        double minDamage;
        double maxDamage;
        double fireRate;
        double shotSpeed;
        double accuracy;
        double critChance;
        double critStrength;
        double armorPenetration;*/

        //Special modification. Decided by rarity
        std::vector<double> weaponSpecials;/*
        double fireStrength; //Burning
        double poisionStrength; //Poison
        double electricStrength; //Lightning. Bounces to closest target, losing a percentage of power depending on the distance
        double slowStrenght; //Slowness amount. Decreases by a constant amount over time
        double additionalBullets;
        double explosionRadius; //Decreases in damage the further out from center the target is
        double bulletSplit; //Splits after a short while
        double ricochetAmount; //Bounces off of walls
        double homingForce; //Depends on ease of implementation. Homes on closest enemy
        double vampirism; //Steals a percentage of the base damage dealt every bullet hit
        double executionTreshold; //Instant kill at health percantage
        double penetrationAmount; //Amount of times the bullet can penetrate enemies*/
};

#endif // LOOTSYSTEM_H
