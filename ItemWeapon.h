#ifndef ITEMWEAPON_H
#define ITEMWEAPON_H

#include "Item.h"

#include <string>
#include <vector>


class ItemWeapon : public Item{
    public:
        ItemWeapon();
        ~ItemWeapon(){}
        void draw();
        void update();
        void drawInformationBox();

        bool getHoveringOver();
        std::string getItemName();
        double getItemStat(int stat);
        std::vector<double> getItemStats();
        double getItemSpecial(int special);
        std::vector<double> getItemSpecials();
        int getItemRarity();

        void setWeaponStats(std::vector<double> weaponStats, std::string name);
        void setWeaponSpecials(std::vector<double> weaponSpecials);
        void setWeaponParts(int rarity, int type, int prefix, int suffix);
    protected:
    private:
        int weaponRarity; //Normal - Uncommon - Rare - Epic - Legendary - Unique
        int weaponType;
        int weaponPrefix;
        int weaponSuffix;

        //Standard Weapon Statistics
        std::string weaponName;
        std::vector<double> weaponStats;/*
        double minDamage;
        double maxDamage;
        double fireRate;
        double shotSpeed;
        double accuracy;
        double critChance;
        double critStrength;
        double armorPenetration;*/

        //Special modification. Decided by rarity and level.
        std::vector<double> weaponSpecials;/*
        double fireStrength; //Burning
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

        bool hoveringOver;
};

#endif // WEAPON_H
