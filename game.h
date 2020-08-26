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

    static GameState defaultState;

    GameState state = defaultState;

    void init();

    void move();

    void restart();

    bool available(const Point &p);

    void changeStatus(Status status);

    Point randomPoint(int left = 0, int right = COL, int top = 0, int bottom = ROW);

};

#endif //SNAKE_GAME_H
