#ifndef SNAKE_UTILS_H
#define SNAKE_UTILS_H

#define UPDATE_ENABLED(status)\
ui->load->setEnabled(status == NONE);\
ui->save->setEnabled(status == PAUSE);\
ui->start->setEnabled(status == NONE);\
ui->pause->setEnabled(status == START);\
ui->resume->setEnabled(status == PAUSE);\
ui->restart->setEnabled(status == PAUSE || status == STOP);

#endif //SNAKE_UTILS_H
