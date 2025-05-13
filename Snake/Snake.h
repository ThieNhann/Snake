// Snake.h
// --------------------------------------------------
// Defines the snake and handles its movement,
// growth, collision detection, and direction.
// --------------------------------------------------

#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>

struct SnakeSegment {
    int x, y;
};

class Snake {
public:
    Snake(int gridWidth, int gridHeight);

    void reset();
    void move();
    void grow();
    bool checkSelfCollision() const;
    bool checkWallCollision(bool wallCollisionMode) const;
    bool isAt(int x, int y) const;
    void setDirection(int dir);
    int getDirection() const;
    const std::vector<SnakeSegment>& getBody() const;
    void load(const std::vector<SnakeSegment>& segments);
    void load(const std::vector<SnakeSegment>& segments, int restoredDirection = 0);
    void setGridSize(int gridWidth, int gridHeight);

private:
    std::vector<SnakeSegment> body;
    int direction = 0;      // 0=Down, 1=Left, 2=Right, 3=Up
    int prevDirection = 0;
    int gridW, gridH;
};

#endif // SNAKE_H
