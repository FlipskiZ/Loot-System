#ifndef TILECONTAINER_H
#define TILECONTAINER_H

#include "SpecialTile.h"
#include "LootSystem.h"
#include <vector>

class TileContainer : public SpecialTile{
    public:
        TileContainer();
        void draw();
        void update();

        std::vector<int> getContainedItems();
    protected:
    private:
        LootSystem lootSystem;
        std::vector<int> containedItems;
};

#endif // TILECONTAINER_H
