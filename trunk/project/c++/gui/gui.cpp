#include "gui.h"
#include "ui_gui.h"
#include <QStringList>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include <cv.h>
#include <highgui.h>

#include "ihls.h"
#include "nhs.h"
#include "PostProcessing.h"


#define SIZE 100

Gui::Gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gui)
{
    ui->setupUi(this);
    cvWindow = new CvWindow(QString("cvWindow"), CV_WINDOW_AUTOSIZE);

    // Clear tableImage
    while( ui->tableImage->rowCount() )
        ui->tableImage->removeRow(ui->tableImage->rowCount()-1);

    // Set Images (icons) size in the tableView
    QSize size(SIZE,SIZE);
    ui->tableImage->setIconSize(size);

    // Set the column width
    ui->tableImage->setColumnWidth( 0, SIZE+10 );

    // set the location of 'cvWindow'
    cvWindow->setParent(ui->frame);

}

Gui::~Gui()
{
    delete ui;
}

void Gui::updateImage( const cv::Mat &img )
{
    CvMat c_img = img;
    cvWindow->updateImage( &c_img );
}

void Gui::updateImage( const QString &str )
{
    Mat img = imread(str.toStdString());
    updateImage(img);
}


void Gui::on_pushButton_LoadImage_clicked()
{
    // it allows multiple file selections
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "Select one or more files to open",
                            "../../../vibot6_mscv3/Images",
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

    updateImage( fileName );

    QFileInfo fi( fileName );
    ui->label_CurrentImage->setText( fi.fileName() );
}



void Gui::on_pushButton_Next_clicked()
{
    int currentRow = ui->tableImage->currentRow();
    int size = ui->tableImage->rowCount();

    if( size > 0 && currentRow < size-1 )
        ui->tableImage->setCurrentCell( currentRow+1, 1 );
}

void Gui::on_pushButton_Prev_clicked()
{
    int currentRow = ui->tableImage->currentRow();
    int size = ui->tableImage->rowCount();

    if( size > 0 && currentRow > 0 )
        ui->tableImage->setCurrentCell( currentRow-1, 1 );
}



int Gui::question( const QString & title )
{
    return QMessageBox::question( this, title, "Continue?",  QMessageBox::Ok | QMessageBox::Cancel);
}

void Gui::on_pushButton_Process_clicked()
{
    int currentRow = ui->tableImage->currentRow();
    string fileName = ui->tableImage->item( currentRow, 1 )->text().toStdString();
//    if( fileName == "" )
//        return;

    // ihls_nhs
    Mat image = imread( fileName );
    Mat ihls_image = convert_rgb_to_ihls(image);
    Mat nhs_image = convert_ihls_to_nhs(ihls_image);
    updateImage( nhs_image );

    // PostProcessing - FilteredImage
    if( question("FilteredImage") != QMessageBox::Ok )
        return;

    PostProcessing p( nhs_image );
    Mat fimg = p.FilterImage();
    updateImage( fimg );

    // PostProcessing - Elimination
    if( question("Elimination") != QMessageBox::Ok )
        return;

    long int aspectAria = 1500;
    double lowRatio = 0.25;
    double highRatio = 1.3;
    vector<vector<Point> > copyCont;
    Mat eimg = p.Elimination( fimg, copyCont, aspectAria, lowRatio, highRatio );
    updateImage( eimg );


    // PostProcessing - Convex
    if( question("Convex") != QMessageBox::Ok )
        return;

    vector<vector<Point> >hull( copyCont.size() );
    Mat himg = p.Convex( eimg, hull, copyCont );
    updateImage( himg );


    // PostProcessing - Get contour
    if( question("Get contour") != QMessageBox::Ok )
        return;

    vector<IRO::Contour> extractedCont;
    float dist_threshold = 70;
    Mat cimg = p.ThresholdedContour( hull, copyCont, extractedCont, dist_threshold );
    updateImage( cimg );


}

