#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>

class QAbstractButton;

namespace Ui {
    class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

    void load();  // load options from gui
    void save();  // save options to gui

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_buttonBox_clicked(QAbstractButton* button);

private:
    Ui::Options *ui;
    QWidget *parent;  // gui pointer (it allows access to the options which are public members)
};

#endif // OPTIONS_H
