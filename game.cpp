#include <QtGui/QPainter>
#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) : QWidget(parent), ui(new Ui::Game), timer(new QTimer(this)) {
    ui->setupUi(this);

    state.snake.push_back({15, 23});
    state.snake.push_back({14, 23});
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
        paintRect(painter, point, Qt::green);
    }
    paintRect(painter, state.food, Qt::red);
}

void Game::move() {
    state.direction = state.nextDirection;
    auto head = state.snake.begin();
    Point dest{head->x + dx[state.direction], head->y + dy[state.direction]};
    auto alive = true;
    if (dest.x >= 0 && dest.x < COL && dest.y >= 0 && dest.y < ROW) {
        for (const auto &point:state.snake) {
            if (point == dest) {
                alive = false;
                break;
            }
        }
    } else {
        alive = false;
    }
    if (!alive) {
        changeStatus(STOP);
    } else {
        if (state.growth) {
            --state.growth;
        } else {
            state.snake.erase(--state.snake.end());
        }
        state.snake.push_front(dest);
        if (dest == state.food) {
            state.growth = 3;
            state.food = randomPoint();
        }
    }
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
        case Qt::Key_P:
        case Qt::Key_Space:
            if (state.status == START)
                changeStatus(PAUSE);
            else if (state.status == PAUSE)
                changeStatus(START);
        default:
            break;
    }

    if (targetDirection && state.status == START && (!state.direction || abs(state.direction - targetDirection) != 2)
        || state.status == NONE) {
        state.nextDirection = targetDirection;
        changeStatus(START);
        return;
    }
}

void Game::changeStatus(Status status) {
    if (status != state.status) {
        switch (status) {
            case NONE:
                break;
            case START:
                if (!timer->isActive()) timer->start(state.speed);
                break;
            case PAUSE:
                timer->stop();
                break;
            case STOP:
                timer->stop();
                break;
        }
        state.status = status;
    }
}

Point Game::randomPoint() {
    while (true) {
        Point p{QRandomGenerator::global()->bounded(WIDTH + 1), QRandomGenerator::global()->bounded(HEIGHT + 1)};
        auto safe = true;
        for (const auto &point:state.snake) {
            if (p == point) {
                safe = false;
                break;
            }
        }
        if (safe) return p;
    }
}
