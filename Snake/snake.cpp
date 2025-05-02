// Snake.cpp
// --------------------------------------------------
// Implements the behavior of the Snake class
// --------------------------------------------------

#include "Snake.h"
#include "Config.h"

Snake::Snake(int gridWidth, int gridHeight)
    : gridW(gridWidth), gridH(gridHeight) {
    reset();
}

void Snake::reset() {
    body.clear();
    int startX = gridW / 2;
    int startY = gridH / 2 + 2;
    body.push_back({ startX, startY });
    body.push_back({ startX, startY + 1 });
    direction = 0;
    prevDirection = 0;
}

void Snake::move() {
    // Shift body segments
    for (int i = body.size() - 1; i > 0; --i) {
        body[i] = body[i - 1];
    }

    // Apply movement
    switch (direction) {
    case 0: body[0].y += 1; break;
    case 1: body[0].x -= 1; break;
    case 2: body[0].x += 1; break;
    case 3: body[0].y -= 1; break;
    }

    // Wrap around edges of play area
    if (body[0].x < 0) body[0].x = gridW - 1;
    if (body[0].x >= gridW) body[0].x = 0;
    if (body[0].y < 0) body[0].y = gridH - 1;
    if (body[0].y >= gridH) body[0].y = 0;

    prevDirection = direction;
}

void Snake::grow() {
    body.push_back(body.back());
}

bool Snake::checkSelfCollision() const {
    for (size_t i = 1; i < body.size(); ++i) {
        if (body[0].x == body[i].x && body[0].y == body[i].y) {
            return true;
        }
    }
    return false;
}

bool Snake::isAt(int x, int y) const {
    for (const auto& seg : body) {
        if (seg.x == x && seg.y == y) return true;
    }
    return false;
}

void Snake::setDirection(int dir) {
    if ((dir == 0 && prevDirection != 3) ||
        (dir == 1 && prevDirection != 2) ||
        (dir == 2 && prevDirection != 1) ||
        (dir == 3 && prevDirection != 0)) {
        direction = dir;
    }
}

int Snake::getDirection() const {
    return direction;
}

const std::vector<SnakeSegment>& Snake::getBody() const {
    return body;
}

void Snake::load(const std::vector<SnakeSegment>& segments) {
    body = segments;
}

void Snake::load(const std::vector<SnakeSegment>& segments, int restoredDirection) {
    body = segments;
    direction = restoredDirection;
    prevDirection = restoredDirection;
}

void Snake::setGridSize(int gridWidth, int gridHeight) {
    gridW = gridWidth;
    gridH = gridHeight;
}
