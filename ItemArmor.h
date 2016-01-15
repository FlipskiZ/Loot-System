#ifndef ITEMARMOR_H
#define ITEMARMOR_H

#include "Item.h"

#include <string>
#include <vector>

class ItemArmor : public Item{
    public:
        ItemArmor();
        ~ItemArmor(){}
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
        int getItemPiece();
        std::string getArmorPieceAsString(int armorPiece);
        void setArmorStats(std::vector<double> armorStats, std::string name, int armorPiece);
        void setArmorSpecials(std::vector<double> armorSpecials);
        void setArmorParts(int rarity, int type, int prefix, int suffix);
    protected:
    private:
        int armorRarity; //Normal - Uncommon - Rare - Epic - Legendary - Unique
        int armorType;
        int armorPrefix;
        int armorSuffix;

        //Standard Weapon Statistics
        std::string armorName;
        int armorPiece;
        std::vector<double> armorStats;/*
        double minDamage;
        double maxDamage;
        double fireRate;
        double shotSpeed;
        double accuracy;
        double critChance;
        double critStrength;
        double armorPenetration;*/

        //Special modification. Decided by rarity and level.
        std::vector<double> armorSpecials;/*
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

#endif // ITEMARMOR_H
