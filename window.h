#ifndef SNAKE_WINDOW_H
#define SNAKE_WINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_window.h"
#include "game.h"

namespace Ui {
    class Window;
}

class Window : public QMainWindow {
Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

    ~Window() override;

public slots:

    void refresh(Status original, Status status);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::Window *ui;
};

#endif //SNAKE_WINDOW_H
