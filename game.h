#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <QtWidgets/QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <iostream>
#include <QtCore/QRandomGenerator>
#include "constants.h"
#include "ui_game.h"
#include "types.h"
#include "utils.h"

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

private:
    Ui::Game *ui;
    QTimer *timer;

    GameState state = {
            /* status: */ NONE,
            /* snake: */ std::list<Point>(),
            /* direction: */ undefined,
            /* nextDirection: */ undefined,
            /* food */ {12, 4},
            /* growth: */ 0,
            /* speed: */ SPEED,
    };

    void keyPressEvent(QKeyEvent *event) override;

    void move();

    void changeStatus(Status status);

    Point randomPoint();

};

#endif //SNAKE_GAME_H
