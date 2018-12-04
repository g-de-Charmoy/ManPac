#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QGraphicsEllipseItem>
#include <QKeyEvent>
#include <QSound>
#include <QTime>
#include <QtMultimedia/QSoundEffect>
#include <QTextStream>
#include "myscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString text;
    ~MainWindow();

private:
    bool allow_move(const int& nt, int movedir);
    void keyPressEvent(QKeyEvent *k);
    int next_location(int index, int dir);
    int next_smooth(int index, int dir);
    void draw_fruit(QGraphicsPixmapItem *MP);
    void scorer();
    void draw_nodes();
    void draw_ghosts();
    void death_anim();
    void RAVE_time();
    void check_win();
    void draw_MP_smooth();
    void draw_ghosts_smooth();
    void endgame();
    void collide();
    void draw_lives();
    bool eat_fruit(QGraphicsPixmapItem *MP,QGraphicsPixmapItem *fruit);
    QPixmap animate(int dir, int anim, int type);
    Ui::MainWindow *ui;
    QTimer *timer;
    QGraphicsScene* scene;
    QGraphicsEllipseItem* agent;
    QGraphicsEllipseItem* agent2;

private slots:
    void tick();
    void on_Ready_clicked();
    void on_Pause_clicked();
};

#endif // MAINWINDOW_H

