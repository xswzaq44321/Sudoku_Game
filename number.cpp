#include "number.h"

number::number()
{
    this->setPixmap(QPixmap(":/picture/number_empty_selected.png").scaled(40, 40));
}

number::number(int x, int y){
    this->setPixmap(QPixmap("/picture/number_empty_selected.png").scaled(40, 40));
    this->setPos(x - this->pixmap().width() / 2, y - this->pixmap().height());
}
