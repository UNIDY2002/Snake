#include <QtGui/QPainter>
#include <QTimer>
#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) : QWidget(parent), ui(new Ui::Game), timer(new QTimer(this)) {
    ui->setupUi(this);

    state.snake.push_back({14, 23});
    state.snake.push_back({15, 23});

    timer->start(state.speed);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
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

void Game::onTimeout() {
    state.snake.erase(state.snake.begin());
    auto beg = state.snake.begin();
    state.snake.push_back({beg->x, beg->y - 1});
    update();
}
