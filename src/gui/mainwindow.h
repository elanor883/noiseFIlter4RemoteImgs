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
    void zoomIn();
    void zoomOut();
    void applyFilter();
    void scaleImage(double factor);
    void handleSelectionChanged(int);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void display(cv::Mat* img);


private:
    Ui::MainWindow *ui;
    double scaleFactor;
    int kernelSize;
    int kernelType;
    cv::Mat img;

};

#endif // MAINWINDOW_H
