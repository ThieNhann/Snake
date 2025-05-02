// ObjectManager.h
// --------------------------------------------------
// Handles the spawning and position of game objects
// like fruit, and will support more in the future.
// --------------------------------------------------

#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <SFML/Graphics.hpp>
#include "Snake.h"

struct Fruit {
    int x, y;
};

class ObjectManager {
public:
    ObjectManager(int gridWidth, int gridHeight);

    void spawnFruit(const Snake& snake);
    bool checkFruitCollision(const Snake& snake);
    const Fruit& getFruit() const;
    void load(const Fruit& f);
    void setGridSize(int gridWidth, int gridHeight);

private:
    Fruit fruit;
    int gridW, gridH;
};

#endif // OBJECTMANAGER_H
