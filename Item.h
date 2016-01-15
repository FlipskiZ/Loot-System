#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"

#include <string>
#include <vector>

class Item : public Entity{
    public:
        Item();
        virtual void drawInformationBox() = 0;

        virtual bool getHoveringOver() = 0;
        virtual std::string getItemName() = 0;
        virtual double getItemStat(int stat) = 0;
        virtual std::vector<double> getItemStats() = 0;
        virtual double getItemSpecial(int special) = 0;
        virtual std::vector<double> getItemSpecials() = 0;
        virtual int getItemRarity() = 0;
        virtual int getItemPiece() = 0;

        void setItemLevel(double level);

        double getItemLevel();

    protected:

        double itemLevel;
    private:
};

#endif // ITEM_H
