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
#define NUM_OF_OUTPUT_IMAGES 6


Gui::Gui(QWidget *parent) :
    QMainWindow(parent),
    ui_options(this),
    current_image(0),
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

    cvWindow->resize(ui->frame->size());

    readSettings();

    // check consistency - for the first time load the default param.
    if( Color.isEmpty() )
    {
        // ui_options Contructor load the default parameters in case of errors
        ui_options.save();
    }

    // So in the first time we display the images.
    is_display_images = true;

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
    // Main windows settings
    settings.beginGroup("MainWindow");

    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    restoreState(settings.value("state", QByteArray()).toByteArray());

    settings.endGroup();


    // Options
    settings.beginGroup("Options");
    Color = settings.value("Color").toString();
    Normalization = settings.value("Normalization").toBool();

    Function   = settings.value("Function").toString();
    low_ratio  = settings.value("low_ratio").toFloat();
    high_ratio = settings.value("high_ratio").toFloat();
    aspect_are = settings.value("aspect_are").toFloat();
    Threshold  = settings.value("Threshold").toFloat();
    settings.endGroup();
}

void Gui::writeSettings( void )
{
    // Main windows settings
    settings.beginGroup("MainWindow");

    // Save postion/size of main window
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("state", saveState());

    settings.endGroup();


    // Options
    settings.beginGroup("Options");
    settings.setValue("Color", Color);
    settings.setValue("Normalization", Normalization);
    settings.setValue("Function", Function);
    settings.setValue("low_ratio", low_ratio);
    settings.setValue("high_ratio", high_ratio);
    settings.setValue("aspect_are", aspect_are);
    settings.setValue("Threshold", Threshold);
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

    ui->pushButton_Delete->setEnabled(true);
}

void Gui::on_tableImage_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if( ui->tableImage->rowCount() == 0 || currentRow == previousRow )
        return;

    // FileName is in the column 1
    currentColumn = 1;
    QTableWidgetItem * item = ui->tableImage->item( currentRow, 1 );
    if( item == 0 )
        return;

//    // Ask to the user if he want to lose all the images output
//    if( ui->pushButton_SaveAll->isEnabled() )
//    {
//        QMessageBox::StandardButton ans = QMessageBox::question( this, "Change current cell",
//                                                                 "You are going to lose all the output images. "
//                                                                 "Do you want to continue?",
//                                                                 QMessageBox::Ok | QMessageBox::Cancel);
//        if( ans == QMessageBox::Cancel )
//        {
////            ui->tableImage->disconnect(SIGNAL(currentCellChanged(int,int,int,int)));
////            QTableWidgetItem * item = ui->tableImage->item( previousRow, previousColumn );
////            qWarning() << " " << currentRow << " " << previousRow;
////            ui->tableImage->setCurrentCell( currentRow, currentColumn, QItemSelectionModel::SelectCurrent );
////            connect(ui->tableImage, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(on_tableImage_currentCellChanged(int,int,int,int)));
//            return;
//        }

//        // Disable buttons
//        ui->pushButton_SaveAll->setEnabled(false);
//        ui->pushButton_Next->setEnabled(false);
//        ui->pushButton_Prev->setEnabled(false);
//    }

    QString fileName = item->text();

    qDebug() << fileName;

    updateImage( fileName );

    QFileInfo fi( fileName );
    ui->label_CurrentImage->setText( fi.fileName() );
}

void Gui::on_pushButton_Next_clicked()
{
    current_image = (current_image + 1) % NUM_OF_OUTPUT_IMAGES;

    int currentRow = ui->tableImage->currentRow();
    QTableWidgetItem *currentItem = ui->tableImage->item( currentRow, 1 );
    if( currentItem == 0 ) {
        qWarning() << "Selection is empty!";
        return;
    }
    // Get filename of the selected image
    QString fileName = currentItem->text();
    QFileInfo fi( fileName );

    switch (current_image)
    {
    case 0:
        if( ui->NHSSegmentation->isChecked() ) {
            ui->label_CurrentImage->setText( "NHS Segmentation: " + fi.fileName() );
            break;
        }
        current_image++;
    case 1:
        if( ui->NoiseRemoval->isChecked() ) {
            ui->label_CurrentImage->setText( "Noise Removal: " + fi.fileName() );
            break;
        }
        current_image++;
    case 2:
        if( ui->ObjectElimination->isChecked() ) {
            ui->label_CurrentImage->setText( "Object Elimination: " + fi.fileName() );
            break;
        }
        current_image++;
    case 3:
        if( ui->ContourExtaction->isChecked() ) {
            ui->label_CurrentImage->setText( "Contour Extaction: " + fi.fileName() );
            break;
        }
        current_image++;
    case 4:
        if( ui->GetRotationOffset->isChecked() ) {
            ui->label_CurrentImage->setText( "Get Rotation Offset: " + fi.fileName() );
            break;
        }
        current_image++;
    case 5:
        if( ui->GielisShapeReconstruction->isChecked() ) {
            ui->label_CurrentImage->setText( "Gielis Shape Reconstruction: " + fi.fileName() );
            break;
        }
        current_image++;
    }

    if (current_image >= output_images.size())
        current_image = 0;

    if (current_image < 0)
        current_image = output_images.size() - 1;

    updateImage( output_images.at(current_image) );
}

void Gui::on_pushButton_Prev_clicked()
{
    current_image = (current_image - 1) % NUM_OF_OUTPUT_IMAGES;

    if (current_image < 0)
        current_image = output_images.size() - 1;

    int currentRow = ui->tableImage->currentRow();
    QTableWidgetItem *currentItem = ui->tableImage->item( currentRow, 1 );
    if( currentItem == 0 ) {
        qWarning() << "Selection is empty!";
        return;
    }
    // Get filename of the selected image
    QString fileName = currentItem->text();
    QFileInfo fi( fileName );

    switch (current_image)
    {
    case 5:
        if( ui->GielisShapeReconstruction->isChecked() ) {
            ui->label_CurrentImage->setText( "Gielis Shape Reconstruction: " + fi.fileName() );
            break;
        }
        current_image--;
    case 4:
        if( ui->GetRotationOffset->isChecked() ) {
            ui->label_CurrentImage->setText( "Get Rotation Offset: " + fi.fileName() );
            break;
        }
        current_image--;
    case 3:
        if( ui->ContourExtaction->isChecked() ) {
            ui->label_CurrentImage->setText( "Contour Extaction: " + fi.fileName() );
            break;
        }
        current_image--;
    case 2:
        if( ui->ObjectElimination->isChecked() ) {
            ui->label_CurrentImage->setText( "Object Elimination: " + fi.fileName() );
            break;
        }
        current_image--;
    case 1:
        if( ui->NoiseRemoval->isChecked() ) {
            ui->label_CurrentImage->setText( "Noise Removal: " + fi.fileName() );
            break;
        }
        current_image--;
    case 0:
        if( ui->NHSSegmentation->isChecked() ) {
            ui->label_CurrentImage->setText( "NHS Segmentation: " + fi.fileName() );
            break;
        }
        current_image--;
    }

    if (current_image >= output_images.size())
        current_image = 0;

    if (current_image < 0)
        current_image = output_images.size() - 1;

    updateImage( output_images.at(current_image) );
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

    for(int i = number_images - 1; i >= 0; i--)
    {
        ui->tableImage->setCurrentCell( i, 1 );
        on_pushButton_Delete_clicked();
    }
    output_images.clear();
    is_display_images = true;
    statusBar()->showMessage( "Process for all images is completed!" );
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

    // Setting the status bar
    // Get filename of the selected image
    QFileInfo fi( currentItem->text() );
    statusBar()->showMessage( "Process for file " + fi.fileName() + " has started!" );

    // ihls_nhs
    Mat image = imread( fileName );
    Mat ihls_image = convert_rgb_to_ihls(image);
    // The second argument menas if it's red or blue.
    // 0 is red, blue is 1. You can put 2 here for others, but then
    // you have to provide the hue max and min, sat min values. e.g. :
    // convert_ihls_to_nhs(ihls_image, 2, 163, 134, 60);
    int nhs_mode = 0;
    if (Color.toLower().compare("blue") == 0)
    {
        nhs_mode = 1;
    }
    Mat nhs_image = convert_ihls_to_nhs(ihls_image, nhs_mode);
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
    vector<vector<Point> > copyCont;
    Mat eimg = p.Elimination( fimg, copyCont, aspect_are, low_ratio, high_ratio );
    Mat eimg_save = eimg.clone();
    if(is_display_images)
    {
        updateImage( eimg_save );
    }
    output_images.push_back(eimg_save);


    // PostProcessing - Convex
    vector<vector<Point> >hull( copyCont.size() );
    Mat himg = p.Convex( eimg, hull, copyCont );
    Mat himg_save = himg.clone();
    if(is_display_images)
    {
        updateImage( himg_save );
    }
    output_images.push_back(himg_save);


    // PostProcessing - Get contour
    vector<IRO::Contour> extractedCont;
    Mat cimg = p.ThresholdedContour( hull, copyCont, extractedCont, Threshold );
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

        int functionUsed = 0;
        if( Function == "Func1" )
            functionUsed = 1;
        else if( Function == "Func2" )
            functionUsed = 2;
        else if( Function == "Func3" )
            functionUsed = 3;

        vector<Vector2d> output = rationalSuperShape2d.Run( contourVector, offsets, true, functionUsed );
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

    current_image = output_images.size() - 1;

    // Enable buttons
    ui->pushButton_SaveAll->setEnabled(true);
    ui->pushButton_Next->setEnabled(true);
    ui->pushButton_Prev->setEnabled(true);

    statusBar()->showMessage( "Process for " + QString(fi.fileName()) +  " is completed!" );
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
    ui_options.load();
    ui_options.show();
}


/*
 * Saving the output images into hard disk.
 */
void
Gui::save_output_images(vector<Mat> images, vector<bool> flags, const string file_name, const string save_folder)
{
    char *names[6] = {"_nhs", "_noiseRemoval", "_elimination", "_convexHull", "_reducedContour", "_shape"};

    cout << "save_folder: " << save_folder << endl;


    if( !QDir( QString(save_folder.c_str())+"/output/" ).exists() )
    {
        QDir().mkdir( QString(save_folder.c_str())+"/output/" );
    }


    for (unsigned int i = 0; i < images.size(); i++)
    {
        if (flags[i])
        {
            string image_name;
            image_name.append(save_folder);            
            image_name.append("/output/");
            image_name.append(file_name);
            image_name.append(names[i]);
            image_name.append(".jpg");

            imwrite(image_name, images[i]);
        }
    }
}

void Gui::on_pushButton_Delete_clicked()
{
    if( ui->tableImage == 0 || ui->tableImage->rowCount() == 0 )
    {
        qWarning() << "Table is empty!";
        return;
    }

    QList<QTableWidgetItem *> selected = ui->tableImage->selectedItems();
    for( int i=0; i < selected.size(); i++ )
    {
        int row = ui->tableImage->row( selected.at(i) );
        ui->tableImage->removeRow(row);
    }

    if( ui->tableImage->rowCount() == 0 )
    {
        ui->pushButton_Delete->setEnabled(false);
    }
    else
    {
        ui->tableImage->setCurrentCell(0,1);
    }
}

void Gui::on_pushButton_SaveAll_clicked()
{
    vector<bool> flags;
    flags.push_back(true);
    flags.push_back(true);
    flags.push_back(true);
    flags.push_back(true);
    flags.push_back(true);
    flags.push_back(true);

    QTableWidgetItem *currentItem = ui->tableImage->currentItem();
    QFileInfo fi(currentItem->text());
    QString base = fi.baseName();
    save_output_images(output_images, flags, base.toStdString(), fi.canonicalPath().toStdString());

    ui->pushButton_SaveAll->setEnabled(false);
}
