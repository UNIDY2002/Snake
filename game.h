#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QtCore/QRandomGenerator>
#include "constants.h"
#include "types.h"
#include "window.h"

#include <iostream>

using std::cout;
using std::endl;

class Window;

class Game : public QWidget {
Q_OBJECT
public:
    explicit Game(Window *parent = nullptr);

    ~Game() override;

    void load();

    void save();

protected:
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    Window *parent;

    QTimer *timer;

    static GameState defaultState;

    GameState state = defaultState;

    BoardOccupation boardOccupation{};

    void init();

    void move();

    void restart();

    bool available(const Point &p);

    void changeStatus(Status status);

    Point randomPoint(int left = 0, int right = N, int top = 0, int bottom = N);

    void paintRect(QPainter &painter, const Point &p, const Qt::GlobalColor &color) const;

};

#endif //SNAKE_GAME_H
