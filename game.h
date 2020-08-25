#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <QtWidgets/QWidget>
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
            /* snake: */ std::list<Point>(),
            /* direction: */ undefined,
            /* growth: */ -1,
            /* speed: */ SPEED,
    };

private slots:

    void onTimeout();

};

#endif //SNAKE_GAME_H
