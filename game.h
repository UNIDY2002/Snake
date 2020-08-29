#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <QtWidgets/QWidget>
#include "constants.h"
#include "types.h"
#include "window.h"
#include "ui_game.h"

class Window;

namespace Ui {
    class Game;
}

class Game : public QWidget {
Q_OBJECT
public:
    explicit Game(Window *parent = nullptr);

    ~Game() override;

public slots:

    void load();

    void save();

    static void exit();

    void start();

    void pause();

    void resume();

    void restart();

protected:
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    Window *parent;

    QTimer *timer;

    Ui::Game *ui;

    static GameState defaultState;

    GameState state = defaultState;

    BoardOccupation boardOccupation{};

    void init();

    void move();

    bool available(const Point &p);

    void changeStatus(Status status);

    Point randomPoint(int left = 0, int right = N, int top = 0, int bottom = N);

    void paintRect(QPainter &painter, const Point &p, const Qt::GlobalColor &color) const;

};

#endif //SNAKE_GAME_H
