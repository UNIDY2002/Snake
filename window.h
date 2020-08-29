#ifndef SNAKE_WINDOW_H
#define SNAKE_WINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_window.h"
#include "game.h"

#include <iostream>

using std::cout;
using std::endl;

namespace Ui {
    class Window;
}

class Window : public QMainWindow {
Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

    ~Window() override;

    void refreshButtons(Status status);

private:
    Ui::Window *ui;
};

#endif //SNAKE_WINDOW_H
