#include "mainwindow.h"
#include "ui_mainwindow.h"

QList<QcvCAMshiftTracker> camShiftTrackers;

// Variables needed by callback func ...
bool selectObject = false;
cv::Point origin;
cv::Rect selection;

// Callback function used in labeler image show window ...
static void camShiftLabelerOnMouse( int event, int x, int y, int, void* )
{
    if( selectObject )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);

        selection &= cv::Rect(0, 0, QcvCAMshiftTracker::getMainImage().cols, QcvCAMshiftTracker::getMainImage().rows);
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = cv::Point(x,y);
        selection = cv::Rect(x,y,0,0);
        selectObject = true;
        break;
    case CV_EVENT_LBUTTONUP:
        selectObject = false;
        if( selection.width > 0 && selection.height > 0 )
        {

            QcvCAMshiftTracker newTracker;
            newTracker.setCurrentRect(selection);
            camShiftTrackers.push_back(newTracker);

        }
        break;
    }



}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    camShiftTrackers.clear();
}

MainWindow::~MainWindow()
{
    camShiftTrackers.clear();
    delete ui;
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEdit->setDisabled(true);
        ui->pushButton->setDisabled(true);
    }
    else
    {
        ui->lineEdit->setDisabled(false);
        ui->pushButton->setDisabled(false);
    }
}

void MainWindow::on_pushButton_2_pressed()
{
    cv::VideoCapture cap;

    if(ui->checkBox->isChecked())
    {
        cap.open(0);
    }
    else
    {
        cap.open(ui->lineEdit->text().toStdString());
    }

    if( !cap.isOpened() )
    {
        QMessageBox msg;
        msg.setText("Error : Can't open video source!");
        return;
    }

    cv::namedWindow( "CAMShift Video Labeler", 0 );
    cv::setMouseCallback( "CAMShift Video Labeler", camShiftLabelerOnMouse, 0 );

    cv::Mat frame;
    bool paused = false;

    for(;;)
    {
        if( !paused )
        {
            cap >> frame;
            if( frame.empty() )
                break;
            //cv::resize(frame, frame, cv::Size(), 0.5, 0.5);
            QcvCAMshiftTracker::setMainImage(frame);
            for(int i=0; i<camShiftTrackers.size(); i++)
            {
                if(camShiftTrackers[i].trackCurrentRect().boundingRect().area() <= 1)
                    continue;
                cv::ellipse(frame, camShiftTrackers[i].trackCurrentRect(), cv::Scalar(0, 255, 0), 2, CV_AA);
                cv::rectangle(frame, camShiftTrackers[i].trackCurrentRect().boundingRect(), cv::Scalar(0, 255, 0), 2, CV_AA);
            }
        }

        if( selectObject && selection.width > 0 && selection.height > 0 )
        {
            cv::Mat roi(frame, selection);
            cv::bitwise_not(roi, roi);
        }

        cv::imshow( "CAMShift Video Labeler", frame );

        char c = (char)cv::waitKey(10);
        if( c == 27 )
            break;
        switch(c)
        {
        case 'p':
            paused = !paused;
            break;
        case 'k':
            camShiftTrackers.clear();
            break;

        default:
            ;
        }
    }
}

void MainWindow::on_pushButton_pressed()
{
    QString tmpString = QFileDialog::getOpenFileName(this, "Open Video File");
    if(!tmpString.trimmed().isEmpty())
    {
        ui->lineEdit->setText(tmpString);
    }
}

void MainWindow::on_actionDestroy_All_Windows_triggered()
{
    cv::destroyAllWindows();
}

void MainWindow::on_actionDestroy_All_Trackers_triggered()
{
    camShiftTrackers.clear();
}

void MainWindow::on_actionAbout_Me_triggered()
{
    QMessageBox::about(this, "About Me", "Amin Ahmadi"
                                         "<br>Camshift (Continuously Adaptive Mean Shift) Multiple Trackers"
                                         "<br>"
                                         "<a href=\"http://www.amin-ahmadi.com\">http://www.amin-ahmadi.com</a>");
}
