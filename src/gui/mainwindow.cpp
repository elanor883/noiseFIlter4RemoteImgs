#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "low_pass_dialog.h"
#include "gauss_parameter.h"
#include "../filter/filter.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QMessageBox>
#include <QImage>
#include <QFileDialog>
#include <QDir>
#include <QDialog>
#include <QSizePolicy>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>
#include <numeric>
#include <math.h>

using namespace std;
using namespace cv;



QImage Mat2QImage(const cv::Mat3b &src);
static QImage IplImage2QImage(const IplImage *iplImage);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kernelSize = 0;
    kernelType = 0;

    QAction *openFile = ui->actionOpen;
    QAction *quit = ui->actionQuit;



    QMenu *file = ui->menuFile;
    file->addAction(openFile);
    file->addAction(quit);

    connect(ui->applyFilterBtn, SIGNAL(clicked()), this, SLOT(applyFilter()));
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(openFile, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->saveImgBtn, SIGNAL(clicked()), this, SLOT(saveImage()));
}

void MainWindow::applyFilter()
{

    Size size_img = img.size();
    int width = size_img.width;
    int height = size_img.height;
    output = new Mat(height, width, CV_8UC1);


    cout << "applyFilter, combobox text: " << ui->comboFilter->currentIndex() << "\n";
    if(ui->comboSize->currentIndex() == 0)
    {
        kernelSize=3;
        cout << "kernel merete: 3 x 3";
    }
    else if(ui->comboSize->currentIndex() == 1)
    {
        kernelSize=5;
        cout << "kernel merete: 5 x 5 ";
    }
    else if (ui->comboSize->currentIndex() == 2)
    {
        kernelSize=7;
        cout << "kernel merete: 7 x 7 ";
    }
    else if (ui->comboSize->currentIndex() == 3)
    {
        kernelSize=9;
        cout << "kernel merete: 9 x 9 ";
    }




    switch (ui->comboFilter->currentIndex())
    {
    case 0: Filter::median(img,kernelSize, output);
        break;
    case 1: Filter::avarage(img, kernelSize, output);
        break;
    case 2:
    {
        low_pass_dialog* lp = new low_pass_dialog(this);
        lp->exec();
        if(lp->ok_pushed() == 1)
        {
            Filter::low_pass(img,kernelSize, output, lp->get_parameter());
        }



    }
        break;
        /* case 3:
    {
        low_pass_dialog* lp = new low_pass_dialog(this);
        lp->exec();
        if(lp->ok_pushed() == 1)
        {
            Filter::high_pass(img,kernelSize, output, lp->get_parameter());
        }



    }
        break;*/
    case 3:
    {
        gauss_parameter* gp = new gauss_parameter(this);
        gp->exec();
        if(gp->ok_pushed() == 1)
        {
            Filter::gauss_filter(img,kernelSize, output, gp->get_parameter());
        }



    }
        break;
    case 4:  *output = Filter::tresholding(img,Filter::otsu(img, output));
        break;

    default: Filter::median(img,kernelSize, output);
        break;
    }

    display(output);
}


void MainWindow::display(Mat* img){
    IplImage im = *img;
    cout<< "display start" ;
    QImage result = IplImage2QImage(&im);
    ui->imageLabelOut->setScaledContents(true);
    ui->imageLabelOut->setPixmap(QPixmap::fromImage(result));
    ui->imageLabelOut->setMargin(40);
}


void MainWindow::open(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), "/home/elanor/");
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        ui->imageLabelIn->setScaledContents(true);
        ui->imageLabelIn->setMouseTracking(true);
        ui->imageLabelIn->setPixmap(QPixmap::fromImage(image));
        ui->imageLabelIn->setMargin(40);

        string fname = fileName.toUtf8().constData();
        img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
        if(img.empty())
        {
            //return -1;
        }

    }
}

void MainWindow::saveImage(){
    QString fileName = QFileDialog::getSaveFileName (this,
                                                     tr("Save File"), "/home/elanor/");
    const char* fn = fileName.toUtf8().constData();
    cout << fn;
    Mat tmp = *output;
    imwrite( fn, tmp );

}



MainWindow::~MainWindow()
{
    delete ui;
}



static QImage IplImage2QImage(const IplImage *iplImage)
{
    int height = iplImage->height;
    int width = iplImage->width;

    if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
    {
        const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
        return img.rgbSwapped();
    } else if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1){
        const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_Indexed8);

        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++){
            colorTable.push_back(qRgb(i, i, i));
        }
        img.setColorTable(colorTable);
        return img;
    }
}
