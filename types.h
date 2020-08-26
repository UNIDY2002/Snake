#ifndef SNAKE_TYPES_H
#define SNAKE_TYPES_H

#include "constants.h"
#include <list>
#include <set>

enum Direction {
    undefined, W, A, S, D
};

struct Point {
    int x, y;

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point &other) const {
        return x * ROW + y < other.x * ROW + other.y;
    }
};

enum Status {
    NONE, START, PAUSE, STOP
};

struct GameState {
    Status status;
    std::list<Point> snake;
    Direction direction;
    Direction nextDirection;
    Point food;
    std::set<Point> barriers;
    int growth;
    int speed;
};

#endif //SNAKE_TYPES_H
