#ifndef GUI_H
#define GUI_H

#include <cv.h>
#include <QMainWindow>
#include "window_QT.h"


namespace Ui {
    class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gui(QWidget *parent = 0);
    ~Gui();

private slots:
    void updateImage( const cv::Mat &img );
    void updateImage( const QString &str );

    void on_pushButton_LoadImage_clicked();
    void on_tableImage_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_pushButton_Next_clicked();

    void on_pushButton_Prev_clicked();

    void on_pushButton_Process_clicked();

private:
    Ui::Gui *ui;
    CvWindow *cvWindow;


    int question( const QString & title );
};

#endif // GUI_H
