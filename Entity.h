#ifndef ENTITY_H
#define ENTITY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector>

class Entity{
    public:
        Entity();
        virtual void update() = 0;
        virtual void draw() = 0;
        void updateCenter();
        void updateAnimation();

        double getPosition(int xory);
        double getCenterPosition(int xory);
        double getDimension(int worh);
        double getMovementSpeed();
        double getDelta(int xory);
        int getWorldPosition(int xory);
        std::vector<int> getWorldPosition();
        bool getUsesWorldPosition();
        int getEntityId();
        int getTypeId();
        double getAngle(); //Degrees
        int getAnimationValue();
        bool getActive();

        void setPos(double x, double y);
        void setDimensions(double width, double height);
        void setMovementSpeed(double movementSpeed);
        void setDeltaX(double deltaX);
        void setDeltaX(double deltaX, double angle);
        void setDeltaY(double deltaY);
        void setDeltaY(double deltaY, double angle);
        void setWorldPosition(int x, int y);
        void setUsesWorldPosition(bool usesPosition);
        void setEntityId(int entityId);
        void setTypeId(int entityId);
        void setAngle(double angle);
        void setActive(bool active);
        void setSheetDimensions(int sheetColums, int sheetRows, int frameWidth, int frameHeight);
        void setAnimationSpeed(double animationSpeed);
        void setBitmap(ALLEGRO_BITMAP *sheetImage);
        void setAnimationValue(int animationValue, bool untilFinished, double multiplier);

    protected:
        double posX, posY, centerX, centerY;
        double deltaX, deltaY;
        double width, height;
        double movementSpeed, maxMovementSpeed;
        double angle;

        std::vector<int> entityWorldPosition;
        bool usesWorldPosition;

        int entityId, typeId, random;

        int sheetColums, sheetRows;
        double frameX, frameY, frameWidth, frameHeight, animationValue, animationValueHelper, animationCount, animationCountHelper;
        ALLEGRO_BITMAP *sheetImage, *frameImage;
        bool untilFinished, nextFrame;
        int rememberAnimationValue;
        double rememberMultiplier;
        double multiplier;

        bool active;
    private:

};

#endif // ENTITY_H
