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
    bool isAt(int x, int y) const;
    void setDirection(int dir);
    int getDirection() const;
    const std::vector<SnakeSegment>& getBody() const;
    void load(const std::vector<SnakeSegment>& segments);
    void load(const std::vector<SnakeSegment>& segments, int restoredDirection = 0);
    void setWrappingEnabled(bool enabled);
    bool isWithinBounds() const;
    void setGridSize(int gridWidth, int gridHeight);

private:
    std::vector<SnakeSegment> body;
    int direction = 0;     
    int prevDirection = 0;
    bool wrappingEnabled = true;
    int gridW, gridH;
};

#endif 
