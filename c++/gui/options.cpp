#include "options.h"
#include "ui_options.h"
#include <QAbstractButton>
#include "gui.h"

Options::Options(QWidget *p) :
    QDialog(p),
    ui(new Ui::Options)
{
    ui->setupUi(this);

    parent = p;

    // load options from the parent (Gui)
    load();

    // check consistency - first time
    if( !ui->red->isChecked() || !ui->blue->isChecked() )
        on_buttonBox_clicked( ui->buttonBox->button(QDialogButtonBox::RestoreDefaults) );
}

Options::~Options()
{
    delete ui;
}

void Options::load()
{
    Gui *gui = (Gui *) parent;

    if( gui->Color == "red" )
        ui->red->setChecked(true);
    else if( gui->Color == "blue" )
        ui->blue->setChecked(true);

    ui->low_ratio->setValue( gui->low_ratio );
    ui->high_ratio->setValue( gui->high_ratio );
    ui->aspect_are->setValue( gui->aspect_are );
    ui->Threshold->setValue( gui->Threshold );    

    if( gui->Function == "Func1" )
        ui->Func1->setChecked(true);
    else if( gui->Function == "Func2" )
        ui->Func2->setChecked(true);
    else if( gui->Function == "Func3" )
        ui->Func3->setChecked(true);

}

void Options::save()
{
    Gui *gui = (Gui *) parent;

    if( ui->red->isChecked() )
        gui->Color = "red";
    else if( ui->blue->isChecked() )
        gui->Color = "blue";

    gui->low_ratio  = ui->low_ratio->value();
    gui->high_ratio = ui->high_ratio->value();
    gui->aspect_are = ui->aspect_are->value();
    gui->Threshold  = ui->Threshold->value();  

    if( ui->Func1->isChecked() )
        gui->Function = "Func1";
    else if( ui->Func2->isChecked() )
        gui->Function = "Func2";
    else if( ui->Func3->isChecked() )
        gui->Function = "Func3";
}

void Options::on_buttonBox_accepted()
{
//    qDebug( "ButtonBox: Ok" );
    save();
}

void Options::on_buttonBox_rejected()
{
//    qDebug( "ButtonBox: Cancel" );
}


void Options::on_buttonBox_clicked(QAbstractButton* button)
{
    if( ui->buttonBox->button(QDialogButtonBox::RestoreDefaults) == (QPushButton *) button )
    {
        // RestoreDefaults was clicked
        qDebug( "ButtonBox: RestoreDefaults" );

        ui->red->setChecked(true);
        ui->low_ratio->setValue(0.25);
        ui->high_ratio->setValue(1.3);
        ui->aspect_are->setValue(1500);
        ui->Threshold->setValue(7.0);
        ui->Func1->setChecked(true);
    }
}
