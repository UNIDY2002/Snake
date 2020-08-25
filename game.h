#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <QtWidgets/QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <iostream>
#include "constants.h"
#include "ui_game.h"
#include "types.h"

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
            /* growth: */ -1,
            /* speed: */ SPEED,
    };

    void keyPressEvent(QKeyEvent *event) override;

    void move();

    void changeStatus(Status status);

};

#endif //SNAKE_GAME_H
