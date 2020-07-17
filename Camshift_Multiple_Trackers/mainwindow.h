#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QInputDialog>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QPicture>
#include <QImage>
#include <QPixmap>
#include <QTextStream>
#include <QList>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "qcvcamshifttracker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_checkBox_toggled(bool checked);

    void on_pushButton_2_pressed();

    void on_pushButton_pressed();

    void on_actionDestroy_All_Windows_triggered();

    void on_actionDestroy_All_Trackers_triggered();

    void on_actionAbout_Me_triggered();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
