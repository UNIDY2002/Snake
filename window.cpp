#include <QtWidgets/QMessageBox>
#include <QCloseEvent>
#include "window.h"

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window) {
    ui->setupUi(this);
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

void Window::closeEvent(QCloseEvent *event) {
    if (QMessageBox::information(this, "Confirm", "Are you sure to exit?", "Yes", "No")) {
        event->ignore();
    } else {
        event->accept();
    }
}