#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gieliscurvegenerator.h"

#include <QScrollBar>
#include <QGraphicsPixmapItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initialize the parameters, and update the GUI
    m = 4;
    n1 = 4.0;
    n2 = 7.0;
    n3 = 7.0;


    m_img = GielisCurveGenerator::generateGielisCurve(1,1,m,n1,n2,n3);
    m_pixItem = m_scene.addPixmap(m_img);
    ui->gielisScene->setScene(&m_scene);

    //Parameter m
    ui->Label_m->setText("m: " + QString::number(m));
    ui->slider_m->setValue(m);

    //Parameter n1
    ui->Label_n1->setText("n1: " + QString::number(n1));
    ui->Slider_n1->setValue(2*int(n1));

    //Parameter n2
    ui->Label_n2->setText("n2: " + QString::number(n2));
    ui->Slider_n2->setValue(2*int(n2));

    //Parameter n3
    ui->Label_n3->setText("n3: " + QString::number(n3));
    ui->Slider_n3->setValue(2*int(n3));

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
    m_scene.removeItem(m_pixItem);
    m_pixItem = m_scene.addPixmap(m_img);
}

void MainWindow::on_Slider_n1_valueChanged(int value)
{
    n1 = double(value)/2;
    ui->Label_n1->setText(QString("n1: ") + QString::number(n1));
    m_img = GielisCurveGenerator::generateGielisCurve(1,1,m,n1,n2,n3);
    m_scene.removeItem(m_pixItem);
    m_pixItem = m_scene.addPixmap(m_img);
}

void MainWindow::on_Slider_n2_valueChanged(int value)
{
    n2 = double(value)/2;
    ui->Label_n2->setText(QString("n2: ") + QString::number(n2));
    m_img = GielisCurveGenerator::generateGielisCurve(1,1,m,n1,n2,n3);
    m_scene.removeItem(m_pixItem);
    m_pixItem = m_scene.addPixmap(m_img);
}

void MainWindow::on_Slider_n3_valueChanged(int value)
{
    n3 = double(value)/2;
    ui->Label_n3->setText(QString("n3: ") + QString::number(n3));
    m_img = GielisCurveGenerator::generateGielisCurve(1,1,m,n1,n2,n3);
    m_scene.removeItem(m_pixItem);
    m_pixItem = m_scene.addPixmap(m_img);
}

void MainWindow::on_btn_zoomIn_clicked()
{
    ui->gielisScene->scale(1.25, 1.25);
}

void MainWindow::on_btn_zoomOut_clicked()
{
    ui->gielisScene->scale(0.8,0.8);
}
