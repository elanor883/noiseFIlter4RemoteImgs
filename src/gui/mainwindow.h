#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QScrollBar>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>
#include <numeric>
#include <math.h>

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
    void open();
    void applyFilter();
    void display(cv::Mat* img);
    void saveImage();


private:
    Ui::MainWindow *ui;

    int kernelSize;
    int kernelType;
    cv::Mat img;
    cv::Mat* output ;

};

#endif // MAINWINDOW_H
