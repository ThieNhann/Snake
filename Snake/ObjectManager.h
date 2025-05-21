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

struct Bomb {
    int x, y;
    bool active;
};

struct SuperFruit {
    int x, y;
    bool active;
    float timer;  
};

class ObjectManager {
public:
    ObjectManager(int gridWidth, int gridHeight);

    // Grid setup
    void setGridSize(int gridWidth, int gridHeight);
    void reset();

    // --- Fruit ---
    void spawnFruit(const Snake& snake);
    bool checkFruitCollision(const Snake& snake);
    const Fruit& getFruit() const;
    void load(const Fruit& f);

    // --- Bomb ---
    void updateBomb(int score, const Snake& snake);
    bool checkBombCollision(const Snake& snake);
    const Bomb& getBomb() const;
    void loadBomb(const Bomb& b);
    int getNextBombToggleScore() const;
    void setNextBombToggleScore(int score);

    // --- Super Fruit ---
    void spawnSuperFruit(const Snake& snake);
    void updateSuperFruit(float dt, int score, const Snake& snake);
    bool checkSuperFruitCollision(const Snake& snake);
    const SuperFruit& getSuperFruit() const;
    void loadSuperFruit(const SuperFruit& s);
    SuperFruit getSuperFruitState() const;
    int getNextSuperFruitScore() const;
    void setNextSuperFruitScore(int score);

private:
    int gridW, gridH;

    Fruit fruit;
    Bomb bomb;
    SuperFruit superFruit;

    int nextBombToggleScore = 0;
    int nextSuperFruitScore = 0;

    bool bombVisible = false;

    void spawnBomb(const Snake& snake);
};

#endif 

