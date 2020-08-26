#include <QtGui/QPainter>
#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) : QWidget(parent), ui(new Ui::Game), timer(new QTimer(this)) {
    ui->setupUi(this);
    timer->callOnTimeout([&]() { move(); });
    init();
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
        paintRect(painter, point, point == state.snake.front() ? Qt::darkGreen : Qt::green);
    }
    for (const auto &point:state.barriers) {
        paintRect(painter, point, Qt::blue);
    }
    paintRect(painter, state.food, Qt::red);
}

void Game::init() {
    auto point = randomPoint(COL / 3, COL * 2 / 3, ROW / 3, ROW * 2 / 3);
    state.direction = static_cast<Direction>(QRandomGenerator::global()->bounded(W, D + 1));
    state.snake.push_back({point.x + dx[state.direction], point.y + dy[state.direction]});
    state.snake.push_back(point);
}

void Game::move() {
    state.direction = state.nextDirection;
    auto head = state.snake.begin();
    Point dest{head->x + dx[state.direction], head->y + dy[state.direction]};
    if (dest.x >= 0 && dest.x < COL && dest.y >= 0 && dest.y < ROW && (dest == state.food || available(dest))) {
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
    } else {
        changeStatus(STOP);
    }
    update();
}

void Game::restart() {
    if (timer->isActive()) timer->stop();
    state = defaultState;
    init();
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
            break;
        case Qt::Key_R:
            restart();
            return;
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
                if (timer->isActive()) timer->stop();
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

Point Game::randomPoint(int left, int right, int top, int bottom) {
    while (true) {
        Point p{QRandomGenerator::global()->bounded(left, right), QRandomGenerator::global()->bounded(top, bottom)};
        if (available(p)) return p;
    }
}

void Game::mousePressEvent(QMouseEvent *event) {
    auto x = event->x() / WIDTH;
    auto y = event->y() / HEIGHT;
    if (x > 0 && x < COL && y > 0 && y < ROW && state.status == NONE) {
        if (state.barriers.find({x, y}) != state.barriers.end()) {
            state.barriers.erase({x, y});
        } else if (available({x, y})) {
            state.barriers.insert({x, y});
        }
        update();
    }
}

bool Game::available(const Point &p) {
    for (const auto &point:state.snake)
        if (point == p)
            return false;
    if (state.food == p)
        return false;
    return state.barriers.find(p) == state.barriers.end();
}

GameState Game::defaultState = {
        /* status: */ NONE,
        /* snake: */ std::list<Point>(),
        /* direction: */ undefined,
        /* nextDirection: */ undefined,
        /* food */ {12, 4},
        /* barriers */ std::set<Point>(),
        /* growth: */ 0,
        /* speed: */ SPEED,
};
