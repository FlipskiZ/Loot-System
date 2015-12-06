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
        void fireWeapon();
        void takeDamage(double damage, bool crit);

        int getMaxInventorySpace(){return this->maxInventorySpace;}
        int getInventoryItem(int pos){return this->inventoryItemIDs[pos];}
        int getInventoryUsed();
        int getPlayerLevel(){return this->playerLevel;}
        int getPlayerEquippedWeapon(){return this->equippedWeapon;}

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

        int playerLevel, equippedWeapon;

        double fireRateHelper;
};

#endif // PLAYER_H
