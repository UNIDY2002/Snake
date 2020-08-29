#include "window.h"

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window) {
    ui->setupUi(this);
    auto game = new Game(this);
    setCentralWidget(game);
    connect(ui->open, &QAction::triggered, game, &Game::load);
    connect(ui->save, &QAction::triggered, game, &Game::save);
    refreshButtons(NONE);
}

Window::~Window() {
    delete ui;
}

void Window::refreshButtons(Status status) {
    ui->open->setEnabled(status == NONE);
    ui->save->setEnabled(status == PAUSE);
}
