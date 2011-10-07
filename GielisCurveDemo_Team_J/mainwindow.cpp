#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gieliscurvegenerator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initialize the parameters, and update the GUI
    m = 4;
    n1 = 4;
    n2 = 7;
    n3 = 7;

    //Parameter m
    ui->Label_m->setText("m: " + QString::number(m));
    ui->slider_m->setValue(m);

    //Parameter n1
    ui->Label_n1->setText("n1: " + QString::number(n1));
    ui->Slider_n1->setValue(n1);

    //Parameter n2
    ui->Label_n2->setText("n2: " + QString::number(n2));
    ui->Slider_n2->setValue(n2);

    //Parameter n3
    ui->Label_n3->setText("n3: " + QString::number(n3));
    ui->Slider_n3->setValue(n3);


    m_img = GielisCurveGenerator::generateGielisCurve(1,1,m,n1,n2,n3);
    m_scene.addPixmap(m_img);
    ui->gielisScene->setScene(&m_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_slider_m_valueChanged(int value)
{
    m = value;
    ui->Label_m->setText(QString("m: ") + QString::number(value));
    m_img = GielisCurveGenerator::generateGielisCurve(1,1,m,n1,n2,n3);
    m_scene.addPixmap(m_img);
}

void MainWindow::on_Slider_n1_valueChanged(int value)
{
    n1 = value;
    ui->Label_n1->setText(QString("n1: ") + QString::number(value));
    m_img = GielisCurveGenerator::generateGielisCurve(1,1,m,n1,n2,n3);
    m_scene.addPixmap(m_img);
}

void MainWindow::on_Slider_n2_valueChanged(int value)
{
    n2 = value;
    ui->Label_n2->setText(QString("n2: ") + QString::number(value));
    m_img = GielisCurveGenerator::generateGielisCurve(1,1,m,n1,n2,n3);
    m_scene.addPixmap(m_img);
}

void MainWindow::on_Slider_n3_valueChanged(int value)
{
    n3 = value;
    ui->Label_n3->setText(QString("n3: ") + QString::number(value));
    m_img = GielisCurveGenerator::generateGielisCurve(1,1,m,n1,n2,n3);
    m_scene.addPixmap(m_img);
}
