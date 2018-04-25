#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(0, 0, 1201, 871))
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}
