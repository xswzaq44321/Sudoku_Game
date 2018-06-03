#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadResource();
    QFont font("Andy");
    font.setPixelSize(18);
    ui->toolButton_note->setCheckable(true);
    ui->toolButton_note->setFont(font);
    ui->toolButton_note->setStyleSheet(blockIconWhite[0]);
    ui->toolButton_note->setIconSize(QSize(36,36));
    for(int i = 0; i < 9; ++i){ // set up buttons
        for(int j = 0; j < 9; ++j){
            button[i][j] = new QToolButton(this);
            button[i][j]->setGeometry(50 + j * 50, 50 + i * 50, 50, 50);
            if((i / 3 == 1 && j / 3 != 1) || (i / 3 != 1 && j / 3 == 1)){
                button[i][j]->setStyleSheet(blockIconBlue[0]);
            }else{
                button[i][j]->setStyleSheet(blockIconWhite[0]);
            }
            button[i][j]->setIconSize(QSize(36, 36));
            connect(button[i][j], &QToolButton::clicked, [this, i, j](){
                button_pressed(i, j);
            });
        }
    }
    for(int i = 0; i < 9; ++i){ // set up number buttons
        numberButton[i] = new QToolButton(this);
        numberButton[i]->setGeometry(90 + i * 50, 530, 41, 41);
        if(i / 3 == 1){
            numberButton[i]->setStyleSheet(blockIconBlue[0]);
        }else{
            numberButton[i]->setStyleSheet(blockIconWhite[0]);
        }
        numberButton[i]->setIcon(numberIcon[i + 1]);
        numberButton[i]->setIconSize(QSize(36, 36));
        connect(numberButton[i], &QToolButton::clicked, [this, i](void){
            enterHandel(i + 1);
        });
    }
    myTimer = new QTimer(this);
    myTimer->start(100);
    imageTimer = new QTimer(this);
    imageTimer->start(75);
    connect(ui->pushButton_solve, SIGNAL(clicked(bool)), this, SLOT(timeStop()));
    connect(ui->pushButton_clear, SIGNAL(clicked(bool)), this, SLOT(timeStop()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadResource(){
    moveSet.insert(Qt::Key_W);
    moveSet.insert(Qt::Key_A);
    moveSet.insert(Qt::Key_S);
    moveSet.insert(Qt::Key_D);
    numberSet.insert(Qt::Key_1);
    numberSet.insert(Qt::Key_2);
    numberSet.insert(Qt::Key_3);
    numberSet.insert(Qt::Key_4);
    numberSet.insert(Qt::Key_5);
    numberSet.insert(Qt::Key_6);
    numberSet.insert(Qt::Key_7);
    numberSet.insert(Qt::Key_8);
    numberSet.insert(Qt::Key_9);
    eraseSet.insert(Qt::Key_Delete);
    eraseSet.insert(Qt::Key_Backspace);
    eraseSet.insert(Qt::Key_0);
    numberIcon[0] = QIcon(":/number/res/number_0.png");
    numberIcon[1] = QIcon(":/number/res/number_1.png");
    numberIcon[2] = QIcon(":/number/res/number_2.png");
    numberIcon[3] = QIcon(":/number/res/number_3.png");
    numberIcon[4] = QIcon(":/number/res/number_4.png");
    numberIcon[5] = QIcon(":/number/res/number_5.png");
    numberIcon[6] = QIcon(":/number/res/number_6.png");
    numberIcon[7] = QIcon(":/number/res/number_7.png");
    numberIcon[8] = QIcon(":/number/res/number_8.png");
    numberIcon[9] = QIcon(":/number/res/number_9.png");
    numberIconRed[0] = QIcon(":/number/res/number_red_0.png");
    numberIconRed[1] = QIcon(":/number/res/number_red_1.png");
    numberIconRed[2] = QIcon(":/number/res/number_red_2.png");
    numberIconRed[3] = QIcon(":/number/res/number_red_3.png");
    numberIconRed[4] = QIcon(":/number/res/number_red_4.png");
    numberIconRed[5] = QIcon(":/number/res/number_red_5.png");
    numberIconRed[6] = QIcon(":/number/res/number_red_6.png");
    numberIconRed[7] = QIcon(":/number/res/number_red_7.png");
    numberIconRed[8] = QIcon(":/number/res/number_red_8.png");
    numberIconRed[9] = QIcon(":/number/res/number_red_9.png");
    numberIconGreen[0] = QIcon(":/number/res/number_green_0.png");
    numberIconGreen[1] = QIcon(":/number/res/number_green_1.png");
    numberIconGreen[2] = QIcon(":/number/res/number_green_2.png");
    numberIconGreen[3] = QIcon(":/number/res/number_green_3.png");
    numberIconGreen[4] = QIcon(":/number/res/number_green_4.png");
    numberIconGreen[5] = QIcon(":/number/res/number_green_5.png");
    numberIconGreen[6] = QIcon(":/number/res/number_green_6.png");
    numberIconGreen[7] = QIcon(":/number/res/number_green_7.png");
    numberIconGreen[8] = QIcon(":/number/res/number_green_8.png");
    numberIconGreen[9] = QIcon(":/number/res/number_green_9.png");
    numberIconBlue[0] = QIcon(":/number/res/number_blue_0.png");
    numberIconBlue[1] = QIcon(":/number/res/number_blue_1.png");
    numberIconBlue[2] = QIcon(":/number/res/number_blue_2.png");
    numberIconBlue[3] = QIcon(":/number/res/number_blue_3.png");
    numberIconBlue[4] = QIcon(":/number/res/number_blue_4.png");
    numberIconBlue[5] = QIcon(":/number/res/number_blue_5.png");
    numberIconBlue[6] = QIcon(":/number/res/number_blue_6.png");
    numberIconBlue[7] = QIcon(":/number/res/number_blue_7.png");
    numberIconBlue[8] = QIcon(":/number/res/number_blue_8.png");
    numberIconBlue[9] = QIcon(":/number/res/number_blue_9.png");
    blockIconBlue[0] = "border-image:url(:/block/res/block_blue1.png);";
    blockIconBlue[1] = "border-image:url(:/block/res/block_blue2.png);";
    blockIconBlue[2] = "border-image:url(:/block/res/block_blue3.png);";
    blockIconBlue[3] = "border-image:url(:/block/res/block_blue4.png);";
    blockIconBlue[4] = "border-image:url(:/block/res/block_blue5.png);";
    blockIconBlue[5] = "border-image:url(:/block/res/block_blue6.png);";
    blockIconWhite[0] = "border-image:url(:/block/res/block_white1.png);";
    blockIconWhite[1] = "border-image:url(:/block/res/block_white2.png);";
    blockIconWhite[2] = "border-image:url(:/block/res/block_white3.png);";
    blockIconWhite[3] = "border-image:url(:/block/res/block_white4.png);";
    blockIconWhite[4] = "border-image:url(:/block/res/block_white5.png);";
    blockIconWhite[5] = "border-image:url(:/block/res/block_white6.png);";
    numberPixmap[0] = QPixmap(":/number/res/number_0.png");
    numberPixmap[1] = QPixmap(":/number/res/number_1.png");
    numberPixmap[2] = QPixmap(":/number/res/number_2.png");
    numberPixmap[3] = QPixmap(":/number/res/number_3.png");
    numberPixmap[4] = QPixmap(":/number/res/number_4.png");
    numberPixmap[5] = QPixmap(":/number/res/number_5.png");
    numberPixmap[6] = QPixmap(":/number/res/number_6.png");
    numberPixmap[7] = QPixmap(":/number/res/number_7.png");
    numberPixmap[8] = QPixmap(":/number/res/number_8.png");
    numberPixmap[9] = QPixmap(":/number/res/number_9.png");
}

void MainWindow::on_pushButton_new_clicked()
{
    qDebug() << "New Puzzle!" << endl;
    ans.clear();
    clickAble = true;
    ui->player_status->setText("Ongoing...\n:)");
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            button[i][j]->setIcon(QIcon());
        }
    }

    quiz.create();
    player = quiz;
    quiz.printMap();
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(quiz.getMap(i, j) != 0){
                button[i][j]->setIcon(numberIconBlue[static_cast<int>(quiz.getMap(i, j))]);
            }
        }
    }
    timeStart();
}

void MainWindow::on_comboBox_difficulty_currentIndexChanged(int index)
{
    quiz.setDif((char)index);
}

void MainWindow::on_pushButton_solve_clicked()
{
    qDebug() << "solve clicked";
    if(player.mapIsEmpty()){
        ui->player_status->setText("Map is empty!");
        return;
    }
    if(quiz.mapIsEmpty()){
        quiz = player;
    }
    if(quiz.hasDuplicate()){ // if quiz have duplicate, it's bound to be unsolvable!
        ui->player_status->setText("Unsolvable!");
        return;
    }
    ans.clear();
    ui->comboBox_ans->clear();
    quiz.printMap();
    ans = quiz.multiSolve();

    qDebug() << "ans.size() = " << ans.size();
    ui->comboBox_ans->addItem("Common Sol");
    ans[0].printMap();
    if(ans.size() > 1){
        ans[1].printMap();
        ui->player_status->setText(QString::number(ans.size() - 1) + "\n solves.");
        clickAble = false;
    }else{
        ui->player_status->setText("No solve!");
        quiz.clearData(); // if user enter an unsolvable quiz, don't solidify quiz
    }
    for(int i = 1; static_cast<uint>(i) < ans.size(); ++i){ //set combox texts
        char temp[100];
        sprintf(temp, "Sol %d", i); // Sol 0 = Common Sol
        QString qtemp(temp);
        ui->comboBox_ans->addItem(qtemp);
    }
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(player.getMap(i, j) != ans[0].getMap(i, j)){
                if(player.getMap(i, j) != 0){
                    button[i][j]->setIcon(numberIconRed[static_cast<int>(ans[0].getMap(i, j))]);
                }else{
                    button[i][j]->setIcon(numberIconGreen[static_cast<int>(ans[0].getMap(i, j))]);
                }
            }
        }
    }
}

void MainWindow::button_pressed(int i, int j){
    if(nowI != -1 && nowJ != -1){
        disconnect(imageTimer, SIGNAL(timeout()), 0, 0);
        if((nowI / 3 == 1 && nowJ / 3 != 1) || (nowI / 3 != 1 && nowJ / 3 == 1)){
            button[nowI][nowJ]->setStyleSheet(blockIconBlue[0]);
        }else{
            button[nowI][nowJ]->setStyleSheet(blockIconWhite[0]);
        }
        qDebug() << "release" << nowI << "," << nowJ;
    }
    if(i == nowI && j == nowJ){ // de-focus any button on the board
        nowI = -1;
        nowJ = -1;
        return;
    }
    connect(imageTimer, &QTimer::timeout, [this, i, j](void){
        animation(i, j);
    });
    nowI = i;
    nowJ = j;
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    if(nowI == -1 || nowJ == -1){ // if no button was focused
        return;
    }

    if(moveSet.count(e->key())){
        switch(e->key()){
        case Qt::Key_W:
            if(nowI == 0) return;
            button_pressed((nowI - 1), nowJ);
            break;
        case Qt::Key_S:
            if(nowI == 8) return;
            button_pressed((nowI + 1), nowJ);
            break;
        case Qt::Key_A:
            if(nowJ == 0) return;
            button_pressed(nowI, (nowJ - 1));
            break;
        case Qt::Key_D:
            if(nowJ == 8) return;
            button_pressed(nowI, (nowJ + 1));
            break;
        }
    }else if(numberSet.count(e->key())){
        char number = player.getMap(nowI, nowJ);
        number = e->key() - Qt::Key_0;
        enterHandel(number);
    }else if(e->key() == Qt::Key_N){
        on_toolButton_note_toggled(!noteMode);
    }else if(eraseSet.count(e->key())){
        if(noteMode){
            for(int i = 1; i <= 9; ++i){
                if(player.getNote(nowI, nowJ, i - 1) == 1){
                    enterNote(i);
                }
            }
        }else{
            enterNumber(player.getMap(nowI, nowJ));
        }
    }
}

void MainWindow::enterNumber(int number){
    if(quiz.getMap(nowI, nowJ) == 0 && clickAble){ // you can't fill a number into quiz
        if(player.getMap(nowI, nowJ) == number){ // if number has aleady been taken
            player.setMap(nowI, nowJ, 0);
            button[nowI][nowJ]->setText("");
            button[nowI][nowJ]->setIcon(QIcon());
        }else{
            player.clearNote(nowI, nowJ);
            player.setMap(nowI, nowJ, number);
            button[nowI][nowJ]->setIcon(numberIcon[number]);
        }
        if(player.mapIsFinished()){ // check if finish
            if(player.isCorrect()){
                ui->player_status->setText("You Win!");
                clickAble = false;
                timeStop();
            }else{
                ui->player_status->setText("something \nwrong...");
            }
        }
    }
}

void MainWindow::enterNote(int number){
    if(player.getMap(nowI, nowJ) == 0 && clickAble){
        if(player.getNote(nowI, nowJ, number - 1)){ // if note has aleady been taken
            player.setNote(nowI, nowJ, number - 1, 0);
        }else{
            player.setNote(nowI, nowJ, number - 1, 1);
        }
        collage = new QPixmap(300, 300);
        collage->fill(Qt::transparent);
        painter = new QPainter(collage);
        for(int i = 0; i < 9; ++i){
            if(player.getNote(nowI, nowJ, i) == 1){
                painter->drawPixmap((i % 3) * 100, (i / 3) * 100, 100, 100, numberPixmap[i + 1]);
            }
        }
        button[nowI][nowJ]->setIcon(*collage);
        delete collage;
    }
}

void MainWindow::on_pushButton_clear_clicked()
{
    quiz.clearData();
    player.clearData();
    ans.clear();
    clickAble = true;
    ui->player_status->setText("Status");
    ui->comboBox_ans->clear();
    ui->gameTimeLabel->clear();
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            button[i][j]->setIcon(QIcon());
        }
    }
}

void MainWindow::on_pushButton_hint_clicked()
{
    if(nowI == -1 || nowJ == -1){ // if no button was focused
        return;
    }
    if(quiz.getMap(nowI, nowJ) == 0 && player.getMap(nowI, nowJ) == 0){
        Sudoku refAns(quiz);
        refAns.solve();
        if(refAns.getMap(nowI, nowJ) != 0){
            enterNumber(refAns.getMap(nowI, nowJ));
        }
    }
}

void MainWindow::on_comboBox_ans_currentIndexChanged(int index)
{
    qDebug() << "ans index = " << index;
    if(index < 0 || index > static_cast<int>(ans.size())){
        return;
    }
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(player.getMap(i, j) == 0){
                button[i][j]->setIcon(QIcon());
            }
        }
    }
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(player.getMap(i, j) != ans.at(index).getMap(i, j)){
                if(player.getMap(i, j) != 0){
                    button[i][j]->setIcon(numberIconRed[static_cast<int>(ans.at(index).getMap(i, j))]);
                }else{
                    button[i][j]->setIcon(numberIconGreen[static_cast<int>(ans.at(index).getMap(i, j))]);
                }
            }
        }
    }
}

void MainWindow::displayTime(){
    int time = ceil(gameTime.elapsed() / 1000);
    char temp[100];
    sprintf(temp, "%02d : %02d", time / 60, time % 60);
    ui->gameTimeLabel->setText(QString::fromLocal8Bit(temp));
}

void MainWindow::timeStart(){
    connect(myTimer, &QTimer::timeout, this, &MainWindow::displayTime);
    gameTime.start();
}

void MainWindow::timeStop(){
    disconnect(myTimer, &QTimer::timeout, this, &MainWindow::displayTime);
}

void MainWindow::on_toolButton_note_toggled(bool checked)
{
    qDebug() << "noteMode = " << noteMode;
    noteMode = checked;
    if(noteMode){
        ui->toolButton_note->setStyleSheet(blockIconWhite[3]);
    }else{
        ui->toolButton_note->setStyleSheet(blockIconWhite[0]);
    }
}

void MainWindow::enterHandel(int number){
    if(noteMode){
        enterNote(number);
    }else{
        enterNumber(number);
    }
}

void MainWindow::animation(int i, int j){
    static int frame = 0, direction = 1;
    if((i / 3 == 1 && j / 3 != 1) || (i / 3 != 1 && j / 3 == 1)){
        button[i][j]->setStyleSheet(blockIconBlue[frame]);
    }else{
        button[i][j]->setStyleSheet(blockIconWhite[frame]);
    }
    if(frame >= 5){
        direction = -1;
    }
    if(frame <= 0){
        direction = 1;
    }
    frame += direction;
}
