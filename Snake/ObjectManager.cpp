// ObjectManager.cpp
// --------------------------------------------------
// Implements fruit spawning and collision detection.
// --------------------------------------------------

#include "ObjectManager.h"
#include <cstdlib>

ObjectManager::ObjectManager(int gridWidth, int gridHeight)
    : gridW(gridWidth), gridH(gridHeight) {
    fruit = { -1, -1 };
}

void ObjectManager::spawnFruit(const Snake& snake) {
    bool valid = false;
    do {
        fruit.x = rand() % gridW;
        fruit.y = rand() % gridH;
        valid = !snake.isAt(fruit.x, fruit.y);
    } while (!valid);
}

bool ObjectManager::checkFruitCollision(const Snake& snake) {
    return snake.getBody()[0].x == fruit.x && snake.getBody()[0].y == fruit.y;
}

const Fruit& ObjectManager::getFruit() const {
    return fruit;
}

void ObjectManager::load(const Fruit& f) {
    fruit = f;
}

void ObjectManager::setGridSize(int gridWidth, int gridHeight) {
    gridW = gridWidth;
    gridH = gridHeight;
}