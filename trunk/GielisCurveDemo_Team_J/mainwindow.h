#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_slider_m_valueChanged(int value);

    void on_Slider_n1_valueChanged(int value);

    void on_Slider_n2_valueChanged(int value);

    void on_Slider_n3_valueChanged(int value);

    void on_btn_zoomIn_clicked();

    void on_btn_zoomOut_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene m_scene;
    QPixmap m_img;
    QGraphicsPixmapItem *m_pixItem;
    int m;
    double n1, n2, n3;
};

#endif // MAINWINDOW_H
