#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maze.cpp"


Maze* maze = new Maze;

vector<Node*>* mynodes = maze->getNodes();

int MPdir=0;
int MP_nextdir=-1;
int MPanim=1;
int lives=3;

int score=0;
int RAVE=0;     //RAVE BOIIIIIIIII
int RAVE_cycle=0;
int back_cycle=0;

int RED_move1[]= {2,1,1,1,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,3,3,3,3,3,3,3,2,2,2,2,2,3,3,3,3,3,3,2,2,2,3,3,3,3,3,3,2,2,2,3,3,3,2,2,2,1,1,1,2,2,2,2,2,2,3,3,3,0,0,0,3,3,3,0,0,0,3,3,3,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,2,2,2,2,2};
//size of Array = 125

int RED_release=100;
int RED_move1_cur=0;
int REDanim=0;


int fruit=0;
bool cherry_spawn=false;
bool apple_spawn=false;
bool orange_spawn=false;
bool paused=false;


int smoothing=1;
int smoothing_ghosts=0;

QSoundEffect waka;
//QSoundEffect raver;
QSoundEffect begin;
QSoundEffect chomp;
QSoundEffect death;
QSoundEffect eat_ghost;
QSoundEffect intermission;

bool isPaused=false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setStyleSheet("background-color: black;");
    ui->score_val->setStyleSheet("QLabel { color : yellow; }");
    ui->endgametxt->setStyleSheet("QLabel { color : yellow; }");
    ui->name_label->setStyleSheet("QLabel { color : yellow; }");
    ui->score_label->setStyleSheet("QLabel { color : yellow; }");
    ui->highscores->setStyleSheet("QLabel { color : yellow; }");
    ui->wintxt->setStyleSheet("QLabel { color : yellow; }");
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QImage(":/images/images/layout.png"));
    ui->graphicsView->setScene(scene);
    const QPixmap pm(":/images/images/layout.png");
    ui->graphicsView->setFixedSize(pm.width(),pm.height());

    ui->wintxt->hide();
    ui->trophy->hide();
    ui->highscores->hide();
    ui->HSview->hide();
    ui->name->setStyleSheet("QPlainTextEdit { color : black; background-color: yellow; }");
    ui->HSview->setStyleSheet("QTextBrowser { color : yellow; background-color: black; }");
    ui->Pause->setStyleSheet("QPushButton { color : black; background-color: yellow; }");
    ui->textBrowser->setStyleSheet("QTextBrowser {color : yellow}");
    ui->Ready->setStyleSheet("QPushButton { color : black; background-color: yellow; }");
    ui->CDown->setStyleSheet("QLabel { color : yellow;  background-color: rgba(0,0,0,0%); }");
    ui->score_val->hide();
    ui->Pause->hide();
    ui->graphicsView->hide();
    ui->score_label->hide();
    ui->CDown->hide();
    ui->endgametxt->hide();


    waka.setSource(QUrl::fromLocalFile(":/sounds/Sound/waka_two_long.wav"));
   // raver.setSource(QUrl::fromLocalFile(":/sounds/Sound/points.wav"));
    begin.setSource(QUrl::fromLocalFile(":/sounds/Sound/pacman_beginning_tw.wav"));
    chomp.setSource(QUrl::fromLocalFile(":/sounds/Sound/pacman_chomp_tw.wav"));
    death.setSource(QUrl::fromLocalFile(":/sounds/Sound/pacman_death_tw.wav"));
    eat_ghost.setSource(QUrl::fromLocalFile(":/sounds/Sound/pacman_eatghost_tw.wav"));
    intermission.setSource(QUrl::fromLocalFile(":/sounds/Sound/pacman_intermission_tw.wav"));


    srand(time(NULL));



    draw_nodes();

    QPixmap pause(":/images/images/Pause.png");
    ui->Pause->setIcon(pause);

    QPixmap mpmap(":/images/images/MPRO1.png");
    QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);

    QGraphicsPixmapItem *MP = scene->addPixmap(mpmap2);


    QPixmap redmap(":/images/images/red-down.png");
    QPixmap red_scaled = redmap.scaled(QSize(18,24),Qt::KeepAspectRatio);

    QGraphicsPixmapItem *RED = scene->addPixmap(red_scaled);

    RED->setPos(271,258); //-5,-10 for const offset needed in movements

    MP->setPos(maze->getManPac()->getx()-6,maze->getManPac()->gety()-7);

    int i = maze->getManPac()->getIndex();

    if(mynodes->at(i)->getconsumed()==false) {
        mynodes->at(i)->setConsumed(true);
        if(mynodes->at(i)->getSpecial()==false) {
        score+=10;
        }
        else {
        score+=40;
        }
    }
    ui->score_val->setText(QString::number(score));


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));



}

MainWindow::~MainWindow()
{
    scene->clear();
    delete mynodes;
    delete ui;
}

void MainWindow::tick() {


    check_win();

    scene->clear();

    if(MPanim<5) { MPanim++;}
    else { MPanim=0;}

    if(smoothing==0) {

    RAVE_time();

    int i = maze->getManPac()->getIndex();

    if(allow_move(mynodes->at(i)->getNT(),MP_nextdir)==true) {

        MPdir=MP_nextdir;

    }

    if(allow_move(mynodes->at(i)->getNT(),MPdir)==true) {

        int next = next_location(i,MPdir);
        maze->setMP(next);

        int smooth_i = maze->getManPac()->getIndex();


        if(allow_move(mynodes->at(smooth_i)->getNT(),MPdir)==true && mynodes->at(smooth_i)->getIndex()!=128 && mynodes->at(smooth_i)->getIndex()!=147) { smoothing ++;}

        QPixmap newmap = animate(MPdir, MPanim, 0);
        QGraphicsPixmapItem *MP = scene->addPixmap(newmap); MP->setPos(maze->getManPac()->getx()-6,maze->getManPac()->gety()-7);MP->setZValue(6);
        draw_fruit(MP);



    }

    else {



        QPixmap newmap = animate(MPdir, MPanim, 0);
        QGraphicsPixmapItem *MP = scene->addPixmap(newmap); MP->setPos(maze->getManPac()->getx()-6,maze->getManPac()->gety()-7);MP->setZValue(5);
        draw_fruit(MP);
    }


    i = maze->getManPac()->getIndex();
    if(mynodes->at(i)->getconsumed()==false) {
        mynodes->at(i)->setConsumed(true);
        if(mynodes->at(i)->getSpecial()==false) {

        if(RAVE==0) {
         waka.play();
        }
        score+=10;
        fruit+=1;
        }
        else {
        RAVE=93;
        score+=40;
        fruit+=1;
        intermission.play();
        }
    }

    ui->score_val->setText(QString::number(score));

    draw_nodes();



    }

    else {

        RAVE_time();

        int i = maze->getManPac()->getIndex();

        if(allow_move(mynodes->at(i)->getNT(),MP_nextdir)==true) {

            MPdir=MP_nextdir;

        }

        draw_MP_smooth();

        draw_nodes();

        if(smoothing<3) { smoothing ++; }
        else {
        smoothing =0;
        }




    }



    /*
     * Below lines check whether the ghost has been releasd and then
     * draws them appropriately on the maze
     */

    if(RED_release==0) {

    if(smoothing_ghosts==0) {

       draw_ghosts();
       smoothing_ghosts+=1;

    }

    else {

                draw_ghosts_smooth();

       if(smoothing_ghosts<3) {

           smoothing_ghosts+=1;

       }



       else { smoothing_ghosts=0; }

    }
    }

    else {RED_release-=1;

        QPixmap redmap(":/images/images/red-down.png");
        QPixmap red_scaled = redmap.scaled(QSize(18,24),Qt::KeepAspectRatio);

        QGraphicsPixmapItem *RED = scene->addPixmap(red_scaled);

        if(RED_release==0) {

            RED->setPos(maze->getRED()->getx()-5,maze->getRED()->gety()-9);

        }

        else {
             RED->setPos(271,258);

        }
        }


    collide();              //Check collision after every tick is made

    draw_lives();

    if(lives!=0) {

    timer->start(20);           //Reset the timer and continue the game only if the player has lives

    }

    }




void MainWindow::keyPressEvent(QKeyEvent *k) {

    //Controls ManPac's movement by changing his direction based on keyboard input


    if(k->key()==Qt::Key_W && MPdir!=1) {

        MP_nextdir=1;

    }

    else if(k->key()==Qt::Key_S && MPdir!=3) {

        MP_nextdir=3;


    }
    else if(k->key()==Qt::Key_D && MPdir!=0) {

        MP_nextdir=0;


    }
    else if(k->key()==Qt::Key_A && MPdir!=2) {

            MP_nextdir=2;


}
}
int MainWindow::next_location(int index, int dir) {

    //Finds the next node ManPac will move to based on his move direction

    switch(dir) {

        case 0: if(index==147) {return 128;}
                else { return index+1;}
    case 1: {int new_idex=maze->find_node(index,-20); return new_idex;}

    case 2:  if(index==128) { return 147;}
             else { return index-1; }
    case 3: {int new_idex=maze->find_node(index,20); return new_idex;}
    }

    return 0;

}


QPixmap MainWindow::animate(int dir, int anim, int type) {

    /*
     *Animates the various game characters by using direction and animation counters
     * to determine which pixmap to use. The type var determines which character we
     * are dealing with.
     */

    switch(type) {

        case 0: {


        switch(dir) {

            case 0:
                    switch(anim) {

                        case 0: {   QPixmap mpmap(":/images/images/MPW.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2; }
                        case 1: {   QPixmap mpmap(":/images/images/MPRO1.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 2: {   QPixmap mpmap(":/images/images/MPRO2.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 3: {   QPixmap mpmap(":/images/images/MPRO3.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio); return mpmap2;}
                        case 4: {   QPixmap mpmap(":/images/images/MPRO2.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 5: {   QPixmap mpmap(":/images/images/MPRO1.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}


                    }


            case 1: {

                    switch(anim) {

                        case 0: {   QPixmap mpmap(":/images/images/MPW.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 1: {   QPixmap mpmap(":/images/images/MPUO1.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 2: {   QPixmap mpmap(":/images/images/MPUO2.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 3: {   QPixmap mpmap(":/images/images/MPUO3.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 4: {   QPixmap mpmap(":/images/images/MPUO2.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio); return mpmap2; }
                        case 5: {   QPixmap mpmap(":/images/images/MPUO1.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}


                    }


        }

            case 2:
                    switch(anim) {

                        case 0: {   QPixmap mpmap(":/images/images/MPW.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 1: {   QPixmap mpmap(":/images/images/MPLO1.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 2: {   QPixmap mpmap(":/images/images/MPLO2.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 3: {   QPixmap mpmap(":/images/images/MPLO3.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 4: {   QPixmap mpmap(":/images/images/MPLO2.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                        case 5: {   QPixmap mpmap(":/images/images/MPLO1.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}


                }




            case 3: {

                switch(anim) {

                    case 0: {   QPixmap mpmap(":/images/images/MPW.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                    case 1: {   QPixmap mpmap(":/images/images/MPDO1.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                    case 2: {   QPixmap mpmap(":/images/images/MPDO2.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                    case 3: {   QPixmap mpmap(":/images/images/MPDO3.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}
                    case 4: {   QPixmap mpmap(":/images/images/MPDO2.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio); return mpmap2;}
                    case 5: {   QPixmap mpmap(":/images/images/MPDO1.png"); QPixmap mpmap2 = mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);  return mpmap2;}


                    }


            }

        }



        }

    case 1: { switch(REDanim) {

                case 0: { QPixmap mpmap(":/images/images/red-down.png"); QPixmap mpmap2 = mpmap.scaled(QSize(18,24),Qt::KeepAspectRatio); REDanim++; return mpmap2;}

                case 1: { QPixmap mpmap(":/images/images/red-down-2.png"); QPixmap mpmap2 = mpmap.scaled(QSize(18,24),Qt::KeepAspectRatio); REDanim=0; return mpmap2;}
        }

    }


    }
    QPixmap mpmap(":/images/images/error.png");
    return mpmap.scaled(QSize(20,20),Qt::KeepAspectRatio);

}

bool MainWindow::allow_move(const int& nt, int movedir) {

    //Checks whether a move is allowed to be made by checking the node type and then which move directions are possible at it

    switch(nt) {

    case 0: if(movedir==0 || movedir==3) {return true;} else {return false;}
            break;
    case 1: if(movedir==2 || movedir==3) {return true;} else {return false;}
            break;
    case 2: if(movedir==0 || movedir==1) {return true;} else {return false;}
            break;
    case 3: if(movedir==1 || movedir==2) {return true;} else {return false;}
            break;
    case 4: if(movedir==0 || movedir==2 || movedir==3) {return true;} else {return false;}
            break;
    case 5: if(movedir==0 || movedir==1 || movedir==2) {return true;} else {return false;}
            break;
    case 6: if(movedir==1 || movedir==2 || movedir==3) {return true;} else {return false;}
            break;
    case 7: if(movedir==0 || movedir==1 || movedir==3) {return true;} else {return false;}
            break;
    case 8: if(movedir==1 || movedir==3) {return true;} else {return false;}
            break;
    case 9: if(movedir==0 || movedir==2) {return true;} else {return false;}
            break;
    case 10: if(movedir==0 || movedir==1 || movedir==2 || movedir==3) {return true;} else {return false;}
            break;
    default: return false;

    }

    return false;
}



void MainWindow::on_Pause_clicked() {

    //Manages to pause button and switches the state of the icon based on whether the game is paused or playing

    if(paused==false) {
        timer->stop();
        QPixmap play(":/images/images/Play.png");
        ui->Pause->setIcon(play);
        paused=true;
    }

    else {

        timer->start(30);
        QPixmap pause(":/images/images/Pause.png");
        ui->Pause->setIcon(pause);
        paused=false;
    }
}

void delay() {

    //Delay method to add a constant 1 second delay to certain events without calling timers

    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_Ready_clicked() {

    //Sets up the game once the opening screen's 'Ready' button has been clicked

    begin.play();
    ui->graphicsView->show();
    ui->score_val->show();
    text = ui->name->toPlainText();
    ui->name->hide();
    ui->name_label->hide();
    ui->score_label->show();
    ui->textBrowser->hide();
    ui->Ready->hide();
    ui->splash->hide();
    ui->CDown->show();
    delay();
    ui->CDown->setText("2");
    delay();
    ui->CDown->setText("1");
    delay();
    ui->CDown->setText("GO!");
    delay();
    ui->CDown->hide();
    ui->Pause->show();
    draw_lives();

    timer->start(20);

}

void MainWindow::draw_fruit(QGraphicsPixmapItem *MP){

    /*
     * Draws the various fruit in the game based on how many nodes ManPac has eaten.
     * Initialises the spawn variables to true once the requirement is met and
     * only sets them to false once they have been eaten.
     */

    switch(fruit) {

    case 70: cherry_spawn=true; break;
    case 140: apple_spawn=true; break;
    case 210: orange_spawn=true; break;

    }

    if(cherry_spawn==true) {

        QPixmap cherry(":/images/images/Cherry.png");
        QPixmap cherry_scaled = cherry.scaled(QSize(22,22),Qt::KeepAspectRatio);
        QGraphicsPixmapItem *cherry_item = scene->addPixmap(cherry_scaled);
        cherry_item->setPos(270,338);
        if(eat_fruit(MP, cherry_item)==true) {
            cherry_spawn=false;
            score+=100;

        }
    }

    else if(apple_spawn==true) {

        QPixmap apple(":/images/images/Apple.png");
        QPixmap apple_scaled = apple.scaled(QSize(24,24),Qt::KeepAspectRatio);
        QGraphicsPixmapItem *apple_item = scene->addPixmap(apple_scaled);
        apple_item->setPos(270,338);
        if(eat_fruit(MP, apple_item)==true) {
            apple_spawn=false;
            score+=250;

        }

    }

    else if(orange_spawn==true) {

        QPixmap orange(":/images/images/Orange.png");
        QPixmap orange_scaled = orange.scaled(QSize(24,24),Qt::KeepAspectRatio);
        QGraphicsPixmapItem *orange_item = scene->addPixmap(orange_scaled);
        orange_item->setPos(270,338);
        if(eat_fruit(MP,orange_item)==true) {
            orange_spawn=false;
            score+=500;

        }

    }

}

bool MainWindow::eat_fruit(QGraphicsPixmapItem *MP,QGraphicsPixmapItem *fruit){

    //Checks whether ManPac has collided with a fruit => Eat the fruit.

    if(MP->collidesWithItem(fruit)==true) {

        chomp.play();

        return true;
    }

    return false;

}

void MainWindow::draw_nodes() {

    /*
     * Draws the nodes in the game in various colours using a constant pattern if a special node has not been eaten.
     * If a special node has been eaten then Rave>0 and the nodes will alternate between colors randomly
     * using the SRand seed and a random number generated from it.
     * Nodes are not drawn if they have been eaten (i.e node.consumed==true)
     */

    QColor aqua(0, 255, 255);
    QColor chartreuse(127, 255, 0);
    QColor deepPink(255, 20, 147);
    QColor magenta(255, 0, 255);
    QColor orangeRed(255, 69, 0);
    QColor yellow(255, 255, 0);
    QColor blue(0, 0, 255);
    QColor springGreen(0, 255, 127);
    QColor orange(255, 165, 0);

    QVector<QColor> colors;

    colors.append(aqua);
    colors.append(chartreuse);
    colors.append(deepPink);
    colors.append(magenta);
    colors.append(orangeRed);
    colors.append(yellow);
    colors.append(blue);
    colors.append(springGreen);
    colors.append(orange);

    int pattern=0;

    for(std::size_t counter=0;counter<mynodes->size();counter++) {

        if(RAVE>0) {

            if(mynodes->at(counter)->getconsumed()==false) {

                int r=rand()%9;

                if(mynodes->at(counter)->getSpecial()==true) {scene->addEllipse(mynodes->at(counter)->getx()-5,mynodes->at(counter)->gety()-5,15,15)->setBrush(QBrush(colors.at(r))); }
                else {
                scene->addEllipse(mynodes->at(counter)->getx(),mynodes->at(counter)->gety(),5,5)->setBrush(QBrush(colors.at(r)));
                }



            }

        }

        else {

        if(pattern==9) {pattern=0;}

        if(mynodes->at(counter)->getconsumed()==false) {

            if(mynodes->at(counter)->getSpecial()==true) {scene->addEllipse(mynodes->at(counter)->getx()-5,mynodes->at(counter)->gety()-5,15,15)->setBrush(QBrush(colors.at(pattern))); }
            else {
            scene->addEllipse(mynodes->at(counter)->getx(),mynodes->at(counter)->gety(),5,5)->setBrush(QBrush(colors.at(pattern)));
            }



        }
                    pattern ++;


    }
    }

}

void MainWindow::RAVE_time() {

    //Controls the flashing background and maze when a pellet is eaten by cycling through values in a switch statement

    if(RAVE>0) {
        RAVE--;


        back_cycle++;

        int c = back_cycle%5;

        switch (c) {
        case 0: scene->setBackgroundBrush(QImage(":/images/images/layout.png"));
            break;
        case 1: scene->setBackgroundBrush(QImage(":/images/images/layout2.png"));
            break;
        case 2: scene->setBackgroundBrush(QImage(":/images/images/layout3.png"));
            break;
        case 3: scene->setBackgroundBrush(QImage(":/images/images/layout4.png"));
            break;
        case 4: scene->setBackgroundBrush(QImage(":/images/images/layout5.png"));
            break;

        }




        if (RAVE==0) { this->setStyleSheet("background-color: black;");}

        else {
        switch(RAVE_cycle) {

            case 0: this->setStyleSheet("background-color: red;"); RAVE_cycle+=1; break;
            case 1: this->setStyleSheet("background-color: orange;"); RAVE_cycle+=1; break;
            case 2: this->setStyleSheet("background-color: yellow;"); RAVE_cycle+=1; break;
            case 3: this->setStyleSheet("background-color: green;"); RAVE_cycle+=1; break;
            case 4: this->setStyleSheet("background-color: blue;"); RAVE_cycle+=1; break;
            case 5: this->setStyleSheet("background-color: magenta;"); RAVE_cycle+=1; break;
            case 6: this->setStyleSheet("background-color: chartreuse;"); RAVE_cycle+=1; break;
            case 7: this->setStyleSheet("background-color: indigo;"); RAVE_cycle+=1; break;
            case 8: this->setStyleSheet("background-color: deeppink;"); RAVE_cycle+=1; break;
            case 9: this->setStyleSheet("background-color: mediumturquoise;"); RAVE_cycle+=1; break;
            case 10: this->setStyleSheet("background-color: cyan;"); RAVE_cycle=0; break;

        }
        }
    }

}


void MainWindow::check_win(){

    //Checks whether the player has eaten all of the nodes and then presents the 'win' screen

    if(fruit==244) {   //244 nodes on screen

        timer->stop();
        ui->graphicsView->hide();
        ui->score_label->hide();
        ui->score_val->hide();
        ui->Pause->hide();
        lives=0;
        scorer();
        ui->wintxt->show();
        ui->trophy->show();
    }

}


void MainWindow::draw_ghosts() {

    //Draws the ghosts at the normal nodes

    if(RED_move1_cur==126) {

        RED_move1_cur=0;
    }

    int next_R = next_location(maze->getRED()->getIndex(),RED_move1[RED_move1_cur]);
    maze->setRED(next_R);

    QPixmap redmap(":/images/images/red-down.png");
    QPixmap red_scaled = redmap.scaled(QSize(18,24),Qt::KeepAspectRatio);

    QGraphicsPixmapItem *RED = scene->addPixmap(red_scaled);
    RED->setPos(maze->getRED()->getx()-5,maze->getRED()->gety()-9);
    RED_move1_cur+=1;

}

void MainWindow::draw_MP_smooth() {

    //Draws ManPac at the 3 smoothing nodes between each real node

    QPixmap newmap = animate(MPdir, MPanim, 0);
    switch(MPdir) {

    case 0:  {QGraphicsPixmapItem *MP = scene->addPixmap(newmap); MP->setPos(maze->getManPac()->getx()-6+5*smoothing,maze->getManPac()->gety()-7);MP->setZValue(6);draw_fruit(MP);break;}
    case 1:  {QGraphicsPixmapItem *MP = scene->addPixmap(newmap); MP->setPos(maze->getManPac()->getx()-6,maze->getManPac()->gety()-7-5*smoothing);MP->setZValue(6);draw_fruit(MP);break;}
    case 2:  {QGraphicsPixmapItem *MP = scene->addPixmap(newmap); MP->setPos(maze->getManPac()->getx()-6-5*smoothing,maze->getManPac()->gety()-7);MP->setZValue(6);draw_fruit(MP);break;}
    case 3:  {QGraphicsPixmapItem *MP = scene->addPixmap(newmap); MP->setPos(maze->getManPac()->getx()-6,maze->getManPac()->gety()-7+5*smoothing);MP->setZValue(6);draw_fruit(MP);break;}
    }


}

void MainWindow::draw_ghosts_smooth() {

     //Draws ghosts at the 3 smoothing nodes between each real node

    if(RED_move1_cur==126) {

        RED_move1_cur=0;
    }


    QPixmap newmap = animate(0,REDanim,1);
    switch(RED_move1[RED_move1_cur]) {

    case 0:  {QGraphicsPixmapItem *RED = scene->addPixmap(newmap); RED->setPos(maze->getRED()->getx()-5+5*smoothing_ghosts,maze->getRED()->gety()-9);RED->setZValue(5);break;}
    case 1:  {QGraphicsPixmapItem *RED = scene->addPixmap(newmap); RED->setPos(maze->getRED()->getx()-5,maze->getRED()->gety()-9-5*smoothing_ghosts);RED->setZValue(5);break;}
    case 2:  {QGraphicsPixmapItem *RED = scene->addPixmap(newmap); RED->setPos(maze->getRED()->getx()-5-5*smoothing_ghosts,maze->getRED()->gety()-9);RED->setZValue(5);break;}
    case 3:  {QGraphicsPixmapItem *RED = scene->addPixmap(newmap); RED->setPos(maze->getRED()->getx()-5,maze->getRED()->gety()-9+5*smoothing_ghosts);RED->setZValue(5);break;}
    }


}

void MainWindow::draw_lives() {

    //Draws the life icons at the base of the screen

    switch (lives) {
    case 1: {QPixmap life(":/images/images/lives.png"); QPixmap life2=life.scaled(QSize(32,32),Qt::KeepAspectRatio); ui->lives_label->setPixmap(life2);}

        break;
    case 2: {QPixmap life(":/images/images/lives2.png");QPixmap life2=life.scaled(QSize(64,32),Qt::KeepAspectRatio); ui->lives_label->setPixmap(life2);}
        break;

    case 3: {QPixmap life(":/images/images/lives3.png");QPixmap life2=life.scaled(QSize(96,32),Qt::KeepAspectRatio); ui->lives_label->setPixmap(life2);}
        break;

    }
}

void MainWindow::collide() {

    /*
     * Checks whether ManPac has collided with a ghost by using the QList of items in the scene
     * ManPac is set to the highest z-index, with each ghost 1 value behind ManPac,
     * allowing for collision detection in by accessing the first 5 elements of the list
     */

    QList<QGraphicsItem*> mylist = scene->items(Qt::SortOrder(1));

    if(mylist.at(0)->collidesWithItem(mylist.at(1))==true) {

        if(RAVE>0) {

            eat_ghost.play();
            score+=200;
            maze->setRED(113);
            RED_move1_cur=0;
            RED_release=100;

        }

        else {

            death_anim();

            if(lives>0) {

                lives--;
                maze->setMP(133);
                MPdir=0;
                MP_nextdir=0;
                score-=200;
                maze->setRED(113);
                RED_move1_cur=0;
                RED_release=100;

            }

            else {

                ui->graphicsView->hide();
                ui->lives_label->hide();
                timer->stop();

                endgame();

            }

        }

    }

}

void MainWindow::death_anim(){

    //Plays the death sound and gives a delay when ManPac has died

    death.play();
    timer->stop();
    delay();
    timer->start(30);

}

void MainWindow::endgame() {

    //Draws the 'lost' game screen

    ui->endgametxt->show();
    ui->score_label->hide();
    ui->score_val->hide();
    ui->Pause->hide();
    scorer();


}

void MainWindow::scorer() {

    // Writes the score and name of the player to the txt file and loads the file displaying the scores of all games played.

    ui->highscores->show();

    QFile file("scores.txt");

    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream in( &file );
    in <<text<<'\t'<<'\t'<<'\t'<<'\t'<<QString::number(score)<< endl;

    file.close();

    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString content = stream.readAll();
    file.close();
    ui->HSview->setText(content);

    ui->HSview->show();

}
