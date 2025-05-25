// ObjectManager.h
// --------------------------------------------------
// Handles the spawning and position of game objects
// like fruit, and will support more in the future.
// --------------------------------------------------

#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <SFML/Graphics.hpp>
#include "Snake.h"

// Fruit object class
class Fruit {
public:
    Fruit() : x(-1), y(-1) {}
    Fruit(int x, int y) : x(x), y(y) {}
    void setPosition(int nx, int ny) { x = nx; y = ny; }
    int getX() const { return x; }
    int getY() const { return y; }
private:
    int x, y;
};

// Bomb object class
class Bomb {
public:
    Bomb() : x(-1), y(-1), active(false) {}
    Bomb(int x, int y, bool active) : x(x), y(y), active(active) {}
    void setPosition(int nx, int ny) { x = nx; y = ny; }
    void setActive(bool a) { active = a; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool isActive() const { return active; }
private:
    int x, y;
    bool active;
};

// SuperFruit object class
class SuperFruit {
public:
    SuperFruit() : x(-1), y(-1), active(false), timer(0.f) {}
    SuperFruit(int x, int y, bool active, float timer) : x(x), y(y), active(active), timer(timer) {}
    void setPosition(int nx, int ny) { x = nx; y = ny; }
    void setActive(bool a) { active = a; }
    void setTimer(float t) { timer = t; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool isActive() const { return active; }
    float getTimer() const { return timer; }
private:
    int x, y;
    bool active;
    float timer;
};

// Manages all game objects (fruit, bomb, super fruit)
class ObjectManager {
public:
    ObjectManager(int gridWidth, int gridHeight);

    void setGridSize(int gridWidth, int gridHeight);
    void reset();

    // Fruit management
    void spawnFruit(const Snake& snake);
    bool checkFruitCollision(const Snake& snake);
    const Fruit& getFruit() const;
    void load(const Fruit& f);

    // Bomb management
    void updateBomb(int score, const Snake& snake);
    bool checkBombCollision(const Snake& snake);
    const Bomb& getBomb() const;
    void loadBomb(const Bomb& b);
    int getNextBombToggleScore() const;
    void setNextBombToggleScore(int score);

    // Super fruit management
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

