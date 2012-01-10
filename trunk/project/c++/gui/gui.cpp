#include "gui.h"
#include "ui_gui.h"
#include <QStringList>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <string>

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

    readSettings();
}

Gui::~Gui()
{
    writeSettings();
    delete ui;
}

//void Gui::default_settings( void )
//{

//}


void Gui::readSettings( void )
{
    settings.beginGroup("MainWindow");

    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    restoreState(settings.value("state", QByteArray()).toByteArray());

    settings.endGroup();
}

void Gui::writeSettings( void )
{
    settings.beginGroup("MainWindow");

    // Save postion/size of main window
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("state", saveState());

    settings.endGroup();
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
    const QString DEFAULT_DIR_KEY("default_dir");

    // it allows multiple file selections
    QStringList files = QFileDialog::getOpenFileNames(
                this,
                "Select one or more files to open",
                settings.value(DEFAULT_DIR_KEY).toString(),
                "Images (*.bmp *.png *.jpg)" );

    if( !files.isEmpty() )
    {
        QDir CurrentDir;
        settings.setValue( DEFAULT_DIR_KEY, CurrentDir.absoluteFilePath(files.at(0)) );
    }

    // Debug
    //    QListIterator<QString> i(files);
    //    while( i.hasNext() )
    //        qDebug() << i.next();

    // Total number of item before addition
    int total_before_addition = ui->tableImage->rowCount();

    // Default Image size
    QSize size(SIZE,SIZE);

    QListIterator<QString> it(files);
    QPixmap pixmap;
    while( it.hasNext() )
    {
        // Get the current file name (and move the iterator)
        QString current_file = it.next();

        // If the file doesn't exists, continue with the next image
        pixmap.load(current_file);
        if( pixmap.isNull() )
            continue;

        // Insert a new row in the table
        int current_row = ui->tableImage->rowCount();
        ui->tableImage->insertRow( current_row );

        // Set the fileName in the table (column 1)
        QTableWidgetItem *fileNameItem = new QTableWidgetItem(current_file);
        ui->tableImage->setItem( current_row, 1, fileNameItem );

        // Set the Image in the table (column 0)
        pixmap.scaled(size, Qt::KeepAspectRatioByExpanding);   // scaled the image
        QTableWidgetItem *imageItem = new QTableWidgetItem(QIcon(pixmap), "");
        ui->tableImage->setItem( current_row, 0, imageItem );

        // Change the current row size
        ui->tableImage->setRowHeight( current_row, SIZE );

        // Sort By FileName
        //        ui->tableImage->sortByColumn(1);
    }

    if( total_before_addition < ui->tableImage->rowCount() )
    {
        ui->tableImage->setCurrentCell( total_before_addition, 1 );
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

void Gui::on_pushButton_Process_All_clicked()
{
    // We have this boolean so when we do process all, we don't
    // update the images on screen.
    is_display_images = false;

    int number_images = ui->tableImage->rowCount();
    if( ui->tableImage == 0 || number_images == 0 )
    {
        qWarning() << "Table is empty!";
        return;
    }

    vector<bool> flags;
    flags.push_back(true);
    flags.push_back(true);
    flags.push_back(true);
    flags.push_back(true);
    flags.push_back(true);

    for(int i = 0; i < number_images; i++)
    {
        ui->tableImage->setCurrentCell( i, 1 );
        on_pushButton_Process_clicked();
        QTableWidgetItem *currentItem = ui->tableImage->item( i, 1 );
        QFileInfo fi(currentItem->text());
        QString base = fi.baseName();
        save_output_images(output_images, flags, base.toStdString(), fi.canonicalPath().toStdString());
    }

    is_display_images = true;
}

void Gui::on_pushButton_Process_clicked()
{
    output_images.clear();
    if( ui->tableImage == 0 || ui->tableImage->rowCount() == 0 )
    {
        qWarning() << "Table is empty!";
        return;
    }

    int currentRow = ui->tableImage->currentRow();
    QTableWidgetItem *currentItem = ui->tableImage->item( currentRow, 1 );
    if( currentItem == 0 )
    {
        qWarning() << "Selection is empty!";
        return;
    }

    // Get filename of the selected image
    string fileName = currentItem->text().toStdString();

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
    // We need a seperate file for post processign, because they override it
    // and we cant later on save this image and interval step.
    Mat nhs_image_for_post = nhs_image.clone();
    if(is_display_images)
    {
        updateImage( nhs_image );
    }
    output_images.push_back(nhs_image);


    // PostProcessing - FilteredImage
    PostProcessing p( nhs_image_for_post );
    Mat fimg = p.FilterImage();
    Mat fimg_save = fimg.clone();
    if(is_display_images)
    {
        updateImage( fimg_save );
    }
    output_images.push_back(fimg_save);

    // PostProcessing - Elimination
    long int aspectAria = 1500;
    double lowRatio = 0.25;
    double highRatio = 1.3;
    vector<vector<Point> > copyCont;
    Mat eimg = p.Elimination( fimg, copyCont, aspectAria, lowRatio, highRatio );
    Mat eimg_save = eimg.clone();
    if(is_display_images)
    {
        updateImage( eimg_save );
    }
    output_images.push_back(eimg_save);


    // PostProcessing - Convex
    vector<vector<Point> >hull( copyCont.size() );
    Mat himg = p.Convex( eimg, hull, copyCont );
    if(is_display_images)
    {
        updateImage( himg );
    }
    // output_images.push_back(himg);


    // PostProcessing - Get contour   
    vector<IRO::Contour> extractedCont;
    float dist_threshold = 20;
    Mat cimg = p.ThresholdedContour( hull, copyCont, extractedCont, dist_threshold );
    if(is_display_images)
    {
        updateImage( cimg );
    }
    output_images.push_back(cimg);

    // PostProcessing - rotational offset

    // For rotational offset
    IRO::Contour contourPoint;
    // For shape reconstruction
    vector<Vector2d> contourVector;
    // For displaying
    vector<Vector2d> data;

    // Going through all the contours and extract points from
    // shape reconstruction.
    for (unsigned i = 0; i < extractedCont.size(); i++)
    {
        contourPoint = extractedCont[i];

        contourVector.clear();
        contourVector.resize( contourPoint.size() );
        for (unsigned j = 0; j < contourPoint.size(); j++)
        {
            contourVector[j][0] = contourPoint[j].first;
            contourVector[j][1] = contourPoint[j].second;
        }

        // calculate the Rotational Offset
        cRotationalOffset RO;
        vector<float>  offsets = RO.GetMinRadius( contourPoint );

        // TODO: get rid of it later, just for testing.
        //        printf("Offsets size: %d\n", offsets.size());
        //        for (int w = 0; w < offsets.size(); w++)
        //        {
        //            printf("Offsets: %f\n", offsets[w]);
        //        }


        qWarning() << "Starting optimizing...";
        RationalSuperShape2D rationalSuperShape2d;
        vector<Vector2d> output = rationalSuperShape2d.Run( contourVector, offsets, true, 1);
        qWarning() << "Optimizing finished successfully.";

        for (unsigned j = 0; j < output.size(); j++)
        {
            data.push_back(output[j]);
        }
    }

    Mat dimg = drawPoints( image, data );
    if(is_display_images)
    {
        updateImage( dimg );
    }
    output_images.push_back(dimg);

}

Mat Gui::drawPoints( const Mat &image, const vector<Vector2d> &data )
{
    //    Mat dest = Mat::zeros( image.size(), CV_8U );
    Mat dest = image;

    int size = data.size();
    for( int i = 0; i < size; i++ )
    {
        circle( dest, Point( data[i][0], data[i][1] ), 1, cvScalar( 255, 0, 0 ), 3 ); //blue
    }

    return dest;
}


void Gui::on_pushButton_Options_clicked()
{
    ui_options.show();
}


/*
 * Saving the output images into hard disk.
 */
void
Gui::save_output_images(vector<Mat> images, vector<bool> flags, const string file_name, const string save_folder)
{
    char *names[5] = {"_nhs", "_noiseRemoval", "_elimination", "_reducedContour", "_shape"};

    for (unsigned int i = 0; i < images.size(); i++)
    {
        if (flags[i])
        {
            string image_name;
            image_name.append(save_folder);
            image_name.append("/");
            image_name.append(file_name);
            image_name.append(names[i]);
            image_name.append(".jpg");

            imwrite(image_name, images[i]);
        }
    }
}
