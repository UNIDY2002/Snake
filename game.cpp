#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QRandomGenerator>
#include <QtGui/QPainter>
#include <QKeyEvent>
#include <QTimer>
#include "game.h"

Game::Game(Window *parent) : QWidget(parent), parent(parent), timer(new QTimer(this)), ui(new Ui::Game) {
    timer->callOnTimeout([&]() { move(); });
    ui->setupUi(this);
    setFocus();
    init();
}

Game::~Game() {
    delete timer;
    delete ui;
}

void Game::load() {
    QFile file(QFileDialog::getOpenFileName(this, "Open", "game.json", "JSON file (*.json)"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        try {
            GameState newState = defaultState;
            QJsonObject object = QJsonDocument::fromJson(file.readAll()).object();
            QJsonArray snakeX = object.value("snakeX").toArray();
            QJsonArray snakeY = object.value("snakeY").toArray();
            if (snakeX.size() != snakeY.size()) {
                throw std::runtime_error("Length of snakeX and snakeY does not match.");
            } else if (snakeX.size() < 2) {
                throw std::runtime_error("Length of snake should be at least 2.");
            }
            for (int i = 0; i < snakeX.size(); ++i) {
                newState.snake.push_back({snakeX[i].toInt(), snakeY[i].toInt()});
            }
            newState.direction = newState.nextDirection = static_cast<Direction>(object.value("direction").toInt());
            newState.food = {object.value("foodX").toInt(), object.value("foodY").toInt()};
            QJsonArray barriersX = object.value("barriersX").toArray();
            QJsonArray barriersY = object.value("barriersY").toArray();
            if (barriersX.size() != barriersY.size()) {
                throw std::runtime_error("Length of barriersX and barriersY does not match.");
            }
            for (int i = 0; i < barriersX.size(); ++i) {
                newState.barriers.insert({barriersX[i].toInt(), barriersY[i].toInt()});
            }
            newState.growth = object.value("growth").toInt();
            newState.speed = object.value("speed").toInt();
            newState.ticks = object.value("ticks").toInt();
            state = newState;
            changeStatus(PAUSE);
        } catch (std::exception &e) {
            QMessageBox::critical(this, "File open failure.", e.what());
        }
    }
}

void Game::save() {
    QFile file(QFileDialog::getSaveFileName(this, "Save", "game.json", "JSON file (*.json)"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonObject json;
        QJsonArray snakeX, snakeY;
        for (const auto &point:state.snake) {
            snakeX.push_back(point.x);
            snakeY.push_back(point.y);
        }
        json.insert("snakeX", snakeX);
        json.insert("snakeY", snakeY);
        json.insert("direction", state.direction);
        json.insert("foodX", state.food.x);
        json.insert("foodY", state.food.y);
        QJsonArray barriersX, barriersY;
        for (const auto &point:state.barriers) {
            barriersX.push_back(point.x);
            barriersY.push_back(point.y);
        }
        json.insert("barriersX", barriersX);
        json.insert("barriersY", barriersY);
        json.insert("growth", state.growth);
        json.insert("speed", state.speed);
        json.insert("ticks", state.ticks);
        file.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
        file.close();
    }
}

void Game::exit() {
    QApplication::closeAllWindows();
}

void Game::start() {
    state.food = randomPoint();
    changeStatus(START);
}

void Game::pause() {
    changeStatus(PAUSE);
    if (timer->isActive()) timer->stop();
}

void Game::resume() {
    changeStatus(START);
    if (!timer->isActive()) timer->start(state.speed);
}

void Game::restart() {
    if (timer->isActive()) timer->stop();
    state = defaultState;
    init();
    update();
}

void Game::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawRect(QRect(boardOccupation.x, boardOccupation.y, boardOccupation.boardSize, boardOccupation.boardSize));
    for (const auto &point:state.snake) {
        paintRect(painter, point, point == state.snake.front() ? Qt::darkGreen : Qt::green);
    }
    for (const auto &point:state.barriers) {
        paintRect(painter, point, Qt::blue);
    }
    if (state.food.x > -1) {
        paintRect(painter, state.food, Qt::red);
    }
    painter.setFont(QFont("Consolas", 28, QFont::Bold));
    painter.setPen(state.status == STOP ? Qt::red : QColor(128, 128, 128, 70));
    painter.drawText(boardOccupation.x + 10,
                     boardOccupation.y + boardOccupation.boardSize - 10,
                     QString::number(state.ticks));

    ui->load->setEnabled(state.status == NONE);
    ui->save->setEnabled(state.status == PAUSE);
    ui->start->setEnabled(state.status == NONE);
    ui->pause->setEnabled(state.status == START);
    ui->resume->setEnabled(state.status == PAUSE);
    ui->restart->setEnabled(state.status == PAUSE || state.status == STOP);
}

void Game::resizeEvent(QResizeEvent *event) {
    auto w = event->size().width() - ui->buttons->geometry().width() - 15;
    auto h = event->size().height();
    boardOccupation.gridSize = (std::min(w, h) / N);
    boardOccupation.boardSize = boardOccupation.gridSize * N;
    boardOccupation.x = (w - boardOccupation.boardSize) / 2;
    boardOccupation.y = (h - boardOccupation.boardSize) / 2;
}

void Game::keyPressEvent(QKeyEvent *event) {
    Direction targetDirection;
    switch (event->key()) {
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
            return;
    }

    if (state.status == START && abs(state.direction - targetDirection) != 2) {
        state.nextDirection = targetDirection;
        if (!timer->isActive()) timer->start(state.speed);
    }
}

void Game::mousePressEvent(QMouseEvent *event) {
    auto x = (event->x() - boardOccupation.x) / boardOccupation.gridSize;
    auto y = (event->y() - boardOccupation.y) / boardOccupation.gridSize;
    if (x > 0 && x < N && y > 0 && y < N && state.status == NONE) {
        if (state.barriers.find({x, y}) != state.barriers.end()) {
            state.barriers.erase({x, y});
        } else if (available({x, y})) {
            state.barriers.insert({x, y});
        }
        update();
    }
}

void Game::init() {
    auto point = randomPoint(N / 3, N * 2 / 3, N / 3, N * 2 / 3);
    state.direction = static_cast<Direction>(QRandomGenerator::global()->bounded(W, D + 1));
    state.snake.push_back({point.x + dx[state.direction], point.y + dy[state.direction]});
    state.snake.push_back(point);
    parent->refreshActions(NONE, NONE);
}

void Game::move() {
    if ((state.direction = state.nextDirection)) {
        auto head = state.snake.begin();
        Point dest{head->x + dx[state.direction], head->y + dy[state.direction]};
        if (dest.x >= 0 && dest.x < N && dest.y >= 0 && dest.y < N && (dest == state.food || available(dest))) {
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
            ++state.ticks;
            update();
        } else {
            changeStatus(STOP);
            timer->stop();
        }
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

void Game::changeStatus(Status status) {
    if (status != state.status) {
        parent->refreshActions(state.status, status);
        state.status = status;
        update();
    }
}

Point Game::randomPoint(int left, int right, int top, int bottom) {
    while (true) {
        Point p{QRandomGenerator::global()->bounded(left, right), QRandomGenerator::global()->bounded(top, bottom)};
        if (available(p)) return p;
    }
}

void Game::paintRect(QPainter &painter, const Point &p, const Qt::GlobalColor &color) const {
    painter.fillRect(QRect(boardOccupation.x + p.x * boardOccupation.gridSize,
                           boardOccupation.y + p.y * boardOccupation.gridSize,
                           boardOccupation.gridSize,
                           boardOccupation.gridSize), color);
}

GameState Game::defaultState = {
        /* status: */ NONE,
        /* snake: */ std::list<Point>(),
        /* direction: */ undefined,
        /* nextDirection: */ undefined,
        /* food */ {-1, -1},
        /* barriers */ std::set<Point>(),
        /* growth: */ 0,
        /* speed: */ SPEED,
        /* ticks: */ 0,
};
