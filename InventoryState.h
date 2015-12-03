#ifndef INVENTORYSTATE_H
#define INVENTORYSTATE_H

#include "GameState.h"

class InventoryState : public GameState{
    public:
        void init();
        void cleanup();

        void pause();
        void resume();

        void update(Engine* engine);
        void draw(Engine* engine);

        static InventoryState* instance() {
            return &m_InventoryState;
        }

    protected:
        InventoryState(){}
        ~InventoryState(){}

    private:
        static InventoryState m_InventoryState;
        LootSystem lootSystem;
};

#endif // INVENTORYSTATE_H
