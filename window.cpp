#include "window.h"

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window) {
    ui->setupUi(this);
    auto game = new Game(this);
    setCentralWidget(game);
    connect(ui->load, &QAction::triggered, game, &Game::load);
    connect(ui->save, &QAction::triggered, game, &Game::save);
    connect(ui->exit, &QAction::triggered, game, &Game::exit);
    connect(ui->start, &QAction::triggered, game, &Game::start);
    connect(ui->pause, &QAction::triggered, game, &Game::pause);
    connect(ui->resume, &QAction::triggered, game, &Game::resume);
    connect(ui->restart, &QAction::triggered, game, &Game::restart);
}

Window::~Window() {
    delete ui;
}

void Window::refreshActions(Status original, Status status) {
    ui->load->setEnabled(status == NONE);
    ui->save->setEnabled(status == PAUSE);
    ui->start->setEnabled(status == NONE);
    ui->pause->setEnabled(status == START);
    ui->resume->setEnabled(status == PAUSE);
    ui->restart->setEnabled(status == PAUSE || status == STOP);
    switch (status) {
        case START:
            ui->statusbar->showMessage(original == NONE ? "Game starts." : "Game resumes.", 2400);
            break;
        case PAUSE:
            ui->statusbar->showMessage("Pausing.");
            break;
        case STOP:
            ui->statusbar->showMessage("Game stopped.", 2400);
            break;
        default:
            ui->statusbar->clearMessage();
    }
}
