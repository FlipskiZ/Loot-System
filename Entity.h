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

        std::vector<double> getPosition();
        std::vector<double> getCenterPosition();
        std::vector<double> getDimension();
        double getMovementSpeed();
        std::vector<double> getDelta();
        int getEntityId();
        int getTypeId();
        double getAngle(); //Degrees
        int getAnimationValue();
        bool getActive();

        void setPos(double x, double y);
        void setDimensions(double width, double height);
        void setMovementSpeed(double movementSpeed);
        void setDeltaX(double deltaX);
        void setDeltaY(double deltaY);
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

        int entityId, typeId, random;

        int sheetColums, sheetRows;
        double frameX, frameY, frameWidth, frameHeight, animationValue, animationValueHelper, animationCount, animationCountHelper;
        ALLEGRO_BITMAP *sheetImage, *frameImage;
        bool untilFinished, nextFrame;
        int rememberAnimationValue;
        double multiplier;

        bool active;
    private:

};

#endif // ENTITY_H
