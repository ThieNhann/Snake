// ObjectManager.cpp
// --------------------------------------------------
// Implements objects spawning and collision detection.
// --------------------------------------------------

#include "ObjectManager.h"
#include <cstdlib>

// ----------------------
//  Misc implementation
// ----------------------

ObjectManager::ObjectManager(int gridWidth, int gridHeight)
    : gridW(gridWidth), gridH(gridHeight) {
    reset();
}

void ObjectManager::reset() {
    fruit = Fruit();

    bomb = Bomb();
    bombVisible = false;
    nextBombToggleScore = 3 + rand() % 3; 

    superFruit = SuperFruit();
    nextSuperFruitScore = 7 + rand() % 4; 
}

void ObjectManager::setGridSize(int gridWidth, int gridHeight) {
    gridW = gridWidth;
    gridH = gridHeight;
}

// ------------------------------
//  Fruit logic implementation
// ------------------------------

void ObjectManager::spawnFruit(const Snake& snake) {
    bool valid = false;
    int fx, fy;
    do {
        fx = rand() % gridW;
        fy = rand() % gridH;
        valid = !snake.isAt(fx, fy);
    } while (!valid);
    fruit.setPosition(fx, fy);
}

bool ObjectManager::checkFruitCollision(const Snake& snake) {
    return snake.getBody()[0].x == fruit.getX() && snake.getBody()[0].y == fruit.getY();
}

const Fruit& ObjectManager::getFruit() const {
    return fruit;
}

void ObjectManager::load(const Fruit& f) {
    fruit = f;
}

// ------------------------------
//  Bomb logic implementation
// ------------------------------

void ObjectManager::updateBomb(int score, const Snake& snake) {
    if (score >= nextBombToggleScore) {
        if (!bombVisible) {
            spawnBomb(snake);
            bomb.setActive(true);
            bombVisible = true;
        }
        else {
            bomb = Bomb();
            bombVisible = false;
        }

        nextBombToggleScore = score + 3 + rand() % 3;
    }
}

void ObjectManager::spawnBomb(const Snake& snake) {
    bool valid = false;
    int bx, by;
    do {
        bx = rand() % gridW;
        by = rand() % gridH;
        valid = !snake.isAt(bx, by) && (bx != fruit.getX() || by != fruit.getY());
    } while (!valid);
    bomb.setPosition(bx, by);
}

bool ObjectManager::checkBombCollision(const Snake& snake) {
    return bomb.isActive() && snake.getBody()[0].x == bomb.getX() && snake.getBody()[0].y == bomb.getY();
}

const Bomb& ObjectManager::getBomb() const {
    return bomb;
}

void ObjectManager::loadBomb(const Bomb& b) {
    bomb = b;
    bombVisible = b.isActive();
}

int ObjectManager::getNextBombToggleScore() const {
    return nextBombToggleScore;
}

void ObjectManager::setNextBombToggleScore(int score) {
    nextBombToggleScore = score;
}

// ----------------------------------
//  Super fruit logic implementation
// ----------------------------------

void ObjectManager::spawnSuperFruit(const Snake& snake) {
    bool valid = false;
    int sx, sy;
    do {
        sx = rand() % gridW;
        sy = rand() % gridH;
        valid = !snake.isAt(sx, sy) &&
            (sx != fruit.getX() || sy != fruit.getY()) &&
            (sx != bomb.getX() || sy != bomb.getY());
    } while (!valid);
    superFruit.setPosition(sx, sy);
    superFruit.setActive(true);
    superFruit.setTimer(5.f);
}

void ObjectManager::updateSuperFruit(float dt, int score, const Snake& snake) {
    if (!superFruit.isActive() && score >= nextSuperFruitScore) {
        spawnSuperFruit(snake);
        const int baseDelay = 7 + rand() % 4;  
        const int offsetForSuperValue = 5;   
        nextSuperFruitScore = score + baseDelay + offsetForSuperValue;
    }

    if (superFruit.isActive()) {
        superFruit.setTimer(superFruit.getTimer() - dt);
        if (superFruit.getTimer() <= 0.f) {
            superFruit = SuperFruit();
        }
    }
}

bool ObjectManager::checkSuperFruitCollision(const Snake& snake) {
    if (superFruit.isActive() &&
        snake.getBody()[0].x == superFruit.getX() &&
        snake.getBody()[0].y == superFruit.getY())
    {
        superFruit = SuperFruit();
        return true;
    }
    return false;
}

const SuperFruit& ObjectManager::getSuperFruit() const {
    return superFruit;
}

void ObjectManager::loadSuperFruit(const SuperFruit& s) {
    superFruit = s;
}

SuperFruit ObjectManager::getSuperFruitState() const {
    return superFruit;
}

int ObjectManager::getNextSuperFruitScore() const {
    return nextSuperFruitScore;
}

void ObjectManager::setNextSuperFruitScore(int score) {
    nextSuperFruitScore = score;
}

