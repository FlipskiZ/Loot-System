#ifndef LIVINGPLAYER_H
#define LIVINGPLAYER_H

#include <vector>
#include "LivingEntity.h"

class LivingPlayer : public LivingEntity{
    public:
        LivingPlayer();
        ~LivingPlayer(){}
        void update();
        void draw();
        void playerUseKey();
        void fireWeapon();
        void takeDamage(double damage, bool crit = false, double armorBypass = 0);
        void takeDebuffDamage(double damage, int debuffID);
        void healHP(double health);

        int getMaxInventorySpace(){return this->maxInventorySpace;}
        int getInventoryItem(int pos){return this->inventoryItemIDs[pos];}
        int getInventoryUsed();
        int getInventorySpaceLeft(){return this->getMaxInventorySpace()-this->getInventoryUsed();}
        int getPlayerLevel(){return this->playerLevel;}
        int getPlayerEquippedWeapon(){return this->equippedWeapon;}
        bool getPlayerHasWeaponEquipped(){return this->hasWeaponEquipped;}
        int getPlayerEquippedArmor(int armorPiece){return this->equippedArmor[armorPiece];}
        bool getPlayerHasArmorEquipped(int armorPiece){return this->hasArmorEquipped[armorPiece];}
        void updateGearValues();

        void setMaxHP(double maxHP);
        void setArmor(double armor);
        void setMovementSpeed(double movementSpeed);
        void setPlayerLevel(int level){this->playerLevel = level;}
        void setMaxInventorySpace(int maxInventorySpace);
        void addItemToInventory(int itemID);
        void removeItemFromInventory(int itemID);
        void clearInventory();
        void equipWeapon(int weaponId);
        void unequipWeapon();
        void equipArmor(int armorId);
        void unequipArmor(int armorPiece);

    protected:
    private:
        double baseMaxHP, baseArmor, baseMaxMovementSpeed;
        double encumbranceRating;

        int maxInventorySpace;
        std::vector<int> inventoryItemIDs;

        double playerLevel;
        int equippedWeapon;
        std::vector<int> equippedArmor;

        double fireRateHelper;

        std::vector<int> livingCollisions;

        bool changedWorldSegment;

        bool borderingChest;

        bool hasWeaponEquipped;
        std::vector<bool> hasArmorEquipped;
};

#endif // PLAYER_H
