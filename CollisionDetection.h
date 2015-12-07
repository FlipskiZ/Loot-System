#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <vector>
#include <memory>

class CollisionDetection{
    public:
        CollisionDetection(int pLevel, double posX, double posY, double width, double height);

        void clearNode();
        void splitNode();
        void getNodeIndex();
        void insertObjectInNode();
        void getPotentialCollisions();
    protected:
    private:

    int nodeLevel;
    double posX, posY;
    double width, height;
    std::vector<std::unique_ptr<CollisionDetection>> nodes;
};

#endif // COLLISIONDETECTION_H
