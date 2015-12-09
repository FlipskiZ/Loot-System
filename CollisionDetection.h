#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <vector>
#include <memory>

class CollisionDetection{
    public:
        CollisionDetection(int pLevel, double posX, double posY, double width, double height);
        void draw();

        void clearNode();
        void splitNode();
        int getNodeIndex(int livingIndex);
        int getNodeIndex(double posX, double posY, double width, double height);
        void insertObjectInNode(int livingIndex);
        void removeObjectInNode(int livingIndex);
        void getPotentialCollisions(std::vector<int> &returnObjects, int livingIndex);
        void getPotentialCollisions(std::vector<int> &returnObjects, double posX, double posY, double width, double height);
    protected:
    private:

    int nodeLevel;
    double posX, posY;
    double width, height;
    std::vector<int> livingObjects;
    std::vector<std::unique_ptr<CollisionDetection>> subNodes;
};

#endif // COLLISIONDETECTION_H
