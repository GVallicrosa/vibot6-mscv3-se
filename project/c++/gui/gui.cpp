#include "gui.h"
#include "ui_gui.h"
#include <QStringList>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "ihls.h"
#include "nhs.h"
#include "PostProcessing.h"
#include "rationalsupershape2d.h"


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
    if( ui->tableImage->rowCount() == 0 )
        return;

    int currentRow = ui->tableImage->currentRow();
    string fileName = ui->tableImage->item( currentRow, 1 )->text().toStdString();

    if( fileName == "" )
        return;

    /* Steps */
    /* ToDo: use the checkBox to decide, instead of "question()" function */

    // ihls_nhs
    Mat image = imread( fileName );
    Mat ihls_image = convert_rgb_to_ihls(image);
		// The second argument menas if it's red or blue.
		// 0 is red, blue is 1. You can put 2 here for others, but then
		// you have to provide the hue max and min, sat min values. e.g. :
		// convert_ihls_to_nhs(ihls_image, 2, 163, 134, 60);
    Mat nhs_image = convert_ihls_to_nhs(ihls_image, 0);
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

		// Converting the copyCont to IRO:Contour so the rotational offset module
		// can use it.
    IRO::Contour contour;
		for ( int i = 0; i < copyCont[0].size(); i++ )
    {				
        contour.push_back( make_pair( copyCont[0][i].x, copyCont[0][i].y ) );
    }

    // PostProcessing - rotational offset
    if( question("Rotational Offset") != QMessageBox::Ok )
        return;

		// calculate the Rotational Offset
    cRotationalOffset RO;
    vector<float>  offsets = RO.GetMinRadius( contour );		

    // rational_supershape_2d
    if( question("rational_supershape_2d") != QMessageBox::Ok )
        return;

    /* instead of offsets
     * receive this vector of float
     * from rotationaloffset class     
    vector<float> offsets;
    offsets.push_back(0);
    offsets.push_back(3.14/4);
    offsets.push_back(3.14/2);
    offsets.push_back(3*3.14/4);
    */

    Mat dimg = rational_supershape_2d( image, offsets );
    updateImage( dimg );
}


Mat Gui::rational_supershape_2d( const Mat &image, const vector< float > &offsets )
{
    vector<Vector2d> data = readImage_rational_supershape_2d( image );

    qWarning() << "Starting optimizing...";
    RationalSuperShape2D rationalSuperShape2d;
    data = rationalSuperShape2d.Run(data, offsets, true, 1);
    qWarning() << "Optimizing finished successfully.";

    Mat dest = drawPoints( image, data );

    return dest;
}

vector<Vector2d> Gui::readImage_rational_supershape_2d( const Mat &image )
{
    vector<Vector2d> data;

    for (int i = 0; i < image.rows; ++i)
    {
        const uchar* img_data = image.ptr<uchar> (i);
        for (int j = 0; j < image.cols; ++j)
        {
            unsigned int b = *img_data++;
            unsigned int g = *img_data++;
            unsigned int r = *img_data++;

            if (r > 200 && g > 200 && b > 200) {
                data.push_back(Vector2d(double(i), double(j)));
            }
        }
    }

    return data;
}

Mat Gui::drawPoints( const Mat &image, const vector<Vector2d> &data )
{
    Mat dest = Mat::zeros( image.size(), CV_8U );

    int size = data.size();
    for( int i = 0; i < size; i++ )
    {
        circle( dest, Point( data[i][0], data[i][1] ), 1, cvScalar( 255, 0, 0 ), 3 ); //blue
    }

    return dest;
}
