#ifndef SNAKE_TYPES_H
#define SNAKE_TYPES_H

#include <list>

enum Direction {
    W, A, S, D, undefined
};

struct Point {
    int x, y;
};

struct GameState {
    std::list<Point> snake;
    Direction direction;
    int growth;
    int speed;
};

#endif //SNAKE_TYPES_H
