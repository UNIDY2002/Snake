#ifndef SNAKE_TYPES_H
#define SNAKE_TYPES_H

#include <list>

enum Direction {
    undefined, W, A, S, D
};

struct Point {
    int x, y;
};

enum Status {
    NONE, START, PAUSE, STOP
};

struct GameState {
    Status status;
    std::list<Point> snake;
    Direction direction;
    Direction nextDirection;
    int growth;
    int speed;
};

#endif //SNAKE_TYPES_H
