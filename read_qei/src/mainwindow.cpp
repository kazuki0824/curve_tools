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
    connect(poll,SIGNAL(timeout()), this, SLOT(update_value()));
    poll->start(100);
}

MainWindow::~MainWindow()
{
    delete poll;
    delete ui;
}

#define wheel_radius 0.03
const double wheel_coef = wheel_radius * M_PI / 180.0;
void MainWindow::update_value()
{
    auto o1 = QString::fromStdString(std::to_string(wheels[0]));
    ui->lcdNumber->display(o1);
    double current_pos = ui->Wheel->value();
    double new_pos = std::fmod(current_pos + wheels[0] / wheel_coef, 360.0);
    ui->Wheel->setValue(new_pos);

    auto o2 = QString::fromStdString(std::to_string(wheels[1]));
    ui->lcdNumber_2->display(o2);
    current_pos = ui->Wheel_2->value();
    new_pos = std::fmod(current_pos + wheels[1] / wheel_coef, 360.0);
    ui->Wheel_2->setValue(new_pos);

    auto o3 = QString::fromStdString(std::to_string(wheels[2]));
    ui->lcdNumber_3->display(o3);
    current_pos = ui->Wheel_3->value();
    new_pos = std::fmod(current_pos + wheels[2] / wheel_coef, 360.0);
    ui->Wheel_3->setValue(new_pos);

    auto o4 = QString::fromStdString(std::to_string(wheels[3]));
    ui->lcdNumber_4->display(o4);
    current_pos = ui->Wheel_4->value();
    new_pos = std::fmod(current_pos + wheels[3] / wheel_coef, 360.0);
    ui->Wheel_4->setValue(new_pos);
}
