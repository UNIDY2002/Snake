#include "window.h"

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window) {
    ui->setupUi(this);
    setCentralWidget(new Game(this));
}

Window::~Window() {
    delete ui;
}
