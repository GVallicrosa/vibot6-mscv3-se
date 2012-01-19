#ifndef GUI_H
#define GUI_H

#include <cv.h>
#include <highgui.h>
#include <QMainWindow>
#include <Eigen/StdVector>
#include <vector>
#include "window_QT.h"
#include "options.h"

using namespace std;
using namespace Eigen;
using namespace cv;

namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gui(QWidget *parent = 0);
    vector<Mat> output_images;
    int current_image;
    bool is_display_images;
    ~Gui();

    // Options
    QString Color;
    bool Normalization;
    QString Function;
    float low_ratio, high_ratio, aspect_are, Threshold;


private slots:
    void updateImage( const Mat &img );
    void updateImage( const QString &str );
    void updateImageSize( const QSize &size);

    void on_pushButton_LoadImage_clicked();
    void on_tableImage_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_pushButton_Next_clicked();
    void on_pushButton_Prev_clicked();

    void readSettings( void );
    void writeSettings( void );

    void on_pushButton_Process_clicked();
    void on_pushButton_SaveAll_clicked();
    void on_pushButton_Delete_clicked();
    void on_pushButton_Process_All_clicked();

    void on_pushButton_Options_clicked();

private:
    Ui::Gui *ui;
    CvWindow *cvWindow;

    Mat drawPoints( const Mat &image, const vector<Vector2d> &data );

    void save_output_images(vector<Mat> images, vector<bool> flags, const string file_name, const string save_folder);
    void save_rational_output(vector<Vector2d> output, vector<float> gielis_curve_params, const string file_name, const string save_folder);
    void save_rotaional_offsets(vector<vector<float> > output, const string file_name, const string save_folder);
    void save_log_files();

    // Setting
    QSettings settings;

    // Options
    Options ui_options;

};

#endif // GUI_H
