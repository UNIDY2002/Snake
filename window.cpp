#include <QtWidgets/QMessageBox>
#include <QCloseEvent>
#include <utils.h>
#include "window.h"

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window) {
    ui->setupUi(this);
}

Window::~Window() {
    delete ui;
}

void Window::refresh(Status original, Status status) {
    UPDATE_ENABLED(status)
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

void Window::closeEvent(QCloseEvent *event) {
    if (QMessageBox::information(this, "Confirm", "Are you sure to exit?", "Yes", "No")) {
        event->ignore();
    } else {
        event->accept();
    }
}