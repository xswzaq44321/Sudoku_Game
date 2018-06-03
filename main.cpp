#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase fontAndy;
    fontAndy.addApplicationFont(":/font/Andy");
    MainWindow w;
    w.setWindowTitle("Sudoku!");
    w.show();

    return a.exec();
}
