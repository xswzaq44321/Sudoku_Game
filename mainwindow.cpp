#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(0, 0, 1201, 871))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);


    number *n = new number();
}

MainWindow::~MainWindow()
{
    delete ui;
}
