#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <QtWidgets/QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QtCore/QRandomGenerator>
#include "constants.h"
#include "ui_game.h"
#include "types.h"
#include "utils.h"

#include <iostream>
using std::cout;
using std::endl;

namespace Ui {
    class Game;
}

class Game : public QWidget {
Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);

    ~Game() override;

protected:
    void paintEvent(QPaintEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::Game *ui;
    QTimer *timer;

    GameState state = {
            /* status: */ NONE,
            /* snake: */ std::list<Point>(),
            /* direction: */ undefined,
            /* nextDirection: */ undefined,
            /* food */ {12, 4},
            /* barriers */ std::set<Point>(),
            /* growth: */ 0,
            /* speed: */ SPEED,
    };

    void move();

    bool available(const Point &p);

    void changeStatus(Status status);

    Point randomPoint();

};

#endif //SNAKE_GAME_H
