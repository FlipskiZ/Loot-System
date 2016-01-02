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
        int getPlayerLevel(){return this->playerLevel;}
        int getPlayerEquippedWeapon(){return this->equippedWeapon;}
        bool getPlayerHasWeaponEquipped(){return this->weaponEquipped;}

        void setPlayerLevel(int level){this->playerLevel = level;}
        void setMaxInventorySpace(int maxInventorySpace);
        void addItemToInventory(int itemID);
        void removeItemFromInventory(int itemID);
        void clearInventory();
        void equipWeapon(int weaponId);

    protected:
    private:
        int maxInventorySpace;
        std::vector<int> inventoryItemIDs;

        double playerLevel;
        int equippedWeapon;

        double fireRateHelper;

        std::vector<int> livingCollisions;

        bool changedWorldSegment;

        bool borderingChest;

        bool weaponEquipped;
};

#endif // PLAYER_H
