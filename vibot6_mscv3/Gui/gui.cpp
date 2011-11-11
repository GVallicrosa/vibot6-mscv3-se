#include "gui.h"
#include "ui_gui.h"
#include <QStringList>
#include <QFileDialog>
#include <QDebug>

#define SIZE 100

Gui::Gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gui)
{
    ui->setupUi(this);
    scene =  new QGraphicsScene;

    // Clear tableImage
    while( ui->tableImage->rowCount() )
        ui->tableImage->removeRow(ui->tableImage->rowCount()-1);

    // Set Images (icons) size in the tableView
    QSize size(SIZE,SIZE);
    ui->tableImage->setIconSize(size);

    // Set the column width
    ui->tableImage->setColumnWidth( 0, SIZE+10 );

    // Set the scene
    ui->graphicsView->setScene( scene );
}

Gui::~Gui()
{
    delete ui;
}

void Gui::on_pushButton_LoadImage_clicked()
{
    // it allows multiple file selections
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "Select one or more files to open",
                            "../Images",
                            "Images (*.bmp *.png *.jpg)" );


    // Debug
//    QListIterator<QString> i(files);
//    while( i.hasNext() )
//        qDebug() << i.next();


    // Default Image size
    QSize size(SIZE,SIZE);

    QListIterator<QString> it(files);
    while( it.hasNext() )
    {
        // Get the current file name (and move the iterator)
        QString current_file = it.next();

        // Insert a new row in the table
        int current_row = ui->tableImage->rowCount();
        ui->tableImage->insertRow( current_row );

        // Set the fileName in the table (column 1)
        QTableWidgetItem *fileNameItem = new QTableWidgetItem(current_file);
        ui->tableImage->setItem( current_row, 1, fileNameItem );

        // Set the Image in the table (column 0)
        QPixmap pixmap(current_file);
        pixmap.scaled(size, Qt::KeepAspectRatioByExpanding);   // scaled the image
        QTableWidgetItem *imageItem = new QTableWidgetItem(QIcon(pixmap), "");
        ui->tableImage->setItem( current_row, 0, imageItem );

        // Change the current row size
        ui->tableImage->setRowHeight( current_row, SIZE );

        // Sort By FileName
//        ui->tableImage->sortByColumn(1);
    }
}

void Gui::on_tableImage_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if( currentRow == previousRow )
        return;

    // FileName is in the column 1
    currentColumn = 1;
    QString fileName = ui->tableImage->item( currentRow, 1 )->text();

    qDebug() << fileName;
//    QImage img(fileName);
//    img.scaled( ui->graphicsView->size(), Qt::KeepAspectRatio );
//    ui->graphicsView->setBackgroundBrush( img );


    // clear the scene
    scene->clear();

    QPixmap pixmap(fileName);
    pixmap.scaled( ui->graphicsView->size(), Qt::KeepAspectRatio );

    scene->addPixmap(pixmap);
}
