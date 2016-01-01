#ifndef SPECIALTILE_H
#define SPECIALTILE_H

#include "Entity.h"

class SpecialTile : public Entity{
    public:
        SpecialTile();
        void createImpassableSpace();

        void setPassable(bool passable);
        void setTileType(int tileType);

        bool getPassable();
        int getTileType();

        virtual std::vector<int> getContainedWeapons() = 0;
    protected:
        bool tilePassable;

        int tileType;
    private:
};

#endif // SPECIALTILE_H
