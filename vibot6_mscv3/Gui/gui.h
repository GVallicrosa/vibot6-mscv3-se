#ifndef GUI_H
#define GUI_H

#include <QMainWindow>

class QGraphicsScene;

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
    void on_pushButton_LoadImage_clicked();
    void on_tableImage_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    Ui::Gui *ui;
    QGraphicsScene *scene;
};

#endif // GUI_H
