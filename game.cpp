#include <QtGui/QPainter>
#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) : QWidget(parent), ui(new Ui::Game), timer(new QTimer(this)) {
    ui->setupUi(this);

    state.snake.push_back({14, 23});
    state.snake.push_back({15, 23});
    state.direction = D;

    timer->callOnTimeout([&]() { move(); });
}

Game::~Game() {
    delete ui;
    delete timer;
}

void Game::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    for (int x = 0; x < ROW; ++x) {
        for (int y = 0; y < COL; ++y) {
            painter.drawRect(QRect(x * WIDTH, y * HEIGHT, WIDTH, HEIGHT));
        }
    }
    for (const auto &point:state.snake) {
        painter.fillRect(QRect(point.x * WIDTH, point.y * HEIGHT, WIDTH, HEIGHT), Qt::green);
    }
}

void Game::move() {
    state.direction = state.nextDirection;
    state.snake.erase(state.snake.begin());
    auto beg = state.snake.begin();
    state.snake.push_back({beg->x + dx[state.direction], beg->y + dy[state.direction]});
    update();
}

void Game::keyPressEvent(QKeyEvent *event) {
    auto key = event->key();
    auto targetDirection = undefined;
    switch (key) {
        case Qt::Key_Up:
            targetDirection = W;
            break;
        case Qt::Key_Left:
            targetDirection = A;
            break;
        case Qt::Key_Down:
            targetDirection = S;
            break;
        case Qt::Key_Right:
            targetDirection = D;
            break;
        default:
            break;
    }

    if (targetDirection && (!state.direction || abs(state.direction - targetDirection) != 2)) {
        state.nextDirection = targetDirection;
        move();
        if (!timer->isActive()) timer->start(state.speed);
        return;
    }
}
