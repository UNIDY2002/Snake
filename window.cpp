#include "window.h"

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window) {
    ui->setupUi(this);
    auto game = new Game(this);
    setCentralWidget(game);
    connect(ui->open, &QAction::triggered, game, &Game::load);
    connect(ui->save, &QAction::triggered, game, &Game::save);
}

Window::~Window() {
    delete ui;
}

void Window::refreshActions(Status original, Status status) {
    ui->open->setEnabled(status == NONE);
    ui->save->setEnabled(status == PAUSE);
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
