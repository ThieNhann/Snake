// ObjectManager.cpp
// --------------------------------------------------
// Implements objects spawning and collision detection.
// --------------------------------------------------

#include "ObjectManager.h"
#include <cstdlib>

// ----------------------
//  Miscs implementation
// ----------------------

ObjectManager::ObjectManager(int gridWidth, int gridHeight)
    : gridW(gridWidth), gridH(gridHeight) {
    reset();
}

void ObjectManager::reset() {
    fruit = { -1, -1 };

    bomb = { -1, -1, false };
    bombVisible = false;
    nextBombToggleScore = 3 + rand() % 3; 

    superFruit = { -1, -1, false, 0.f };
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

// ------------------------------
//  Bomb logic implementation
// ------------------------------

void ObjectManager::updateBomb(int score, const Snake& snake) {
    if (score >= nextBombToggleScore) {
        if (!bombVisible) {
            spawnBomb(snake);
            bomb.active = true;
            bombVisible = true;
        }
        else {
            bomb = { -1, -1, false };
            bombVisible = false;
        }

        nextBombToggleScore = score + 3 + rand() % 3;
    }
}

void ObjectManager::spawnBomb(const Snake& snake) {
    bool valid = false;
    do {
        bomb.x = rand() % gridW;
        bomb.y = rand() % gridH;
        valid = !snake.isAt(bomb.x, bomb.y) && (bomb.x != fruit.x || bomb.y != fruit.y);
    } while (!valid);
}

bool ObjectManager::checkBombCollision(const Snake& snake) {
    return bomb.active && snake.getBody()[0].x == bomb.x && snake.getBody()[0].y == bomb.y;
}

const Bomb& ObjectManager::getBomb() const {
    return bomb;
}

void ObjectManager::loadBomb(const Bomb& b) {
    bomb = b;
    bombVisible = b.active;
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
    do {
        superFruit.x = rand() % gridW;
        superFruit.y = rand() % gridH;
        valid = !snake.isAt(superFruit.x, superFruit.y) &&
            (superFruit.x != fruit.x || superFruit.y != fruit.y) &&
            (superFruit.x != bomb.x || superFruit.y != bomb.y);
    } while (!valid);
    superFruit.active = true;
    superFruit.timer = 5.f;   
}

void ObjectManager::updateSuperFruit(float dt, int score, const Snake& snake) {
    if (!superFruit.active && score >= nextSuperFruitScore) {
        spawnSuperFruit(snake);
        const int baseDelay = 7 + rand() % 4;  
        const int offsetForSuperValue = 5;   
        nextSuperFruitScore = score + baseDelay + offsetForSuperValue;
    }

    if (superFruit.active) {
        superFruit.timer -= dt;
        if (superFruit.timer <= 0.f) {
            superFruit = { -1, -1, false, 0.f };
        }
    }
}

bool ObjectManager::checkSuperFruitCollision(const Snake& snake) {
    if (superFruit.active &&
        snake.getBody()[0].x == superFruit.x &&
        snake.getBody()[0].y == superFruit.y)
    {
        superFruit = { -1, -1, false, 0.f };  
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

