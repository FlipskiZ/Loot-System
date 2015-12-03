#ifndef TILECONTAINER_H
#define TILECONTAINER_H

#include "SpecialTile.h"

class TileContainer : public SpecialTile{
    public:
        TileContainer();
        void draw();
        void update();
    protected:
    private:
};

#endif // TILECONTAINER_H
