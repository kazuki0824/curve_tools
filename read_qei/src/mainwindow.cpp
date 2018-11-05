#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <geometry_msgs/Twist.h>
extern geometry_msgs::Twist msg;
extern pthread_mutex_t m;
extern float wheels[4];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    poll = new QTimer(this);
    connect(poll,SIGNAL(timeout()), this, SLOT(update()));
    poll->start(100);
}

MainWindow::~MainWindow()
{
    delete poll;
    delete ui;
}

void MainWindow::update_value()
{
    auto o1 = QString::fromStdString(std::to_string(wheels[0]));
    ui->lcdNumber->display(o1);
    auto o2 = QString::fromStdString(std::to_string(wheels[1]));
    ui->lcdNumber_2->display(o2);
    auto o3 = QString::fromStdString(std::to_string(wheels[2]));
    ui->lcdNumber_3->display(o3);
    auto o4 = QString::fromStdString(std::to_string(wheels[3]));
    ui->lcdNumber_4->display(o4);

}
