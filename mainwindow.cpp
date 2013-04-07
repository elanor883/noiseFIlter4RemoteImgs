#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QMessageBox>
#include <QImage>
#include <QFileDialog>
#include <QDir>
#include <QSizePolicy>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>
#include<numeric>
#include<math.h>

using namespace std;
using namespace cv;


Mat tresholding(Mat base, int tresh);
int otsu(Mat base);
QImage Mat2QImage(const cv::Mat3b &src);
static QImage IplImage2QImage(const IplImage *iplImage);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
 /*   zoomInBtn = ui->pushButton;
    zoomOutBtn = ui->pushButton_2;
    moveBtn = ui->pushButton_3;
    applyFilter = ui->pushButton_4;
    saveImg = ui->pushButton_5;*/

   // ui->scrollAreaIn->setHorizontalScrollBar(ui->horizontalScrollBar);
    //ui->scrollAreaIn->horizontalScrollBar()->setValue(100);
 //   QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   // sizePolicy.setHorizontalStretch(2000);
    //sizePolicy.setVerticalStretch(0200);
    //ui->scrollAreaIn->setSizePolicy(sizePolicy);
    //ui->scrollAreaIn->scrollBarWidgets(Qt::AlignRight);
    //ui->scrollAreaIn->addScrollBarWidget(ui->horizontalScrollBar,Qt::AlignCenter);

    QAction *openFile = ui->actionOpen;
    QAction *quit = ui->actionQuit;
    //ui->scrollAreaIn->horizontalScrollBar()->mouseMoveEvent();
    //ui->scrollAreaWidgetContents->scroll(300,300, QRect(500,500,60,60));


    QMenu *file = ui->menuFile;
 //   file = menuBar()->addMenu("&File");
  //  file = ui->menubar->addMenu("&file");
    file->addAction(openFile);
    file->addAction(quit);


    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(openFile, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->zoomInBtn, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(ui->zoomOutBtn, SIGNAL(clicked()), this, SLOT(zoomOut()));
    //new QPushButton(this);
    //zoomInBtn = ui->pushButton;

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
        scaleFactor = 1.0;

        //printAct->setEnabled(true);
       // fitToWindowAct->setEnabled(true);
        //updateActions();

        //if (!fitToWindowAct->isChecked())
          //  imageLabelIn->adjustSize();

        string fname = fileName.toUtf8().constData();
        Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
            if(img.empty())
            {
                //return -1;
            }



           // imshow("Original", img);
           // imshow("Otsu" , tresholding(img,otsu(img)));
            Mat res = tresholding(img,otsu(img));
           //QImage result = Mat2QImage(res);
            IplImage im = res;
            QImage result = IplImage2QImage(&im);

             //imshow("Otsu" , res);
            ui->imageLabelOut->setScaledContents(true);
            ui->imageLabelOut->setPixmap(QPixmap::fromImage(result));
            ui->imageLabelOut->setMargin(40);
            scaleFactor = 1.0;

            //return 0;

    }
}

void MainWindow::zoomIn()
{
    scaleImage(1.25);
}

void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(imageLabelIn->pixmap());
    scaleFactor *= factor;
    ui->imageLabelIn->resize(scaleFactor * ui->imageLabelIn->pixmap()->size());

    ui->imageLabelIn->scroll(60,60,QRect(100,100,600,600));
    ui->imageLabelIn->pixmap()->scaled(400,400,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    ui->imageLabelIn->setScaledContents(true);
    adjustScrollBar(ui->scrollAreaIn->horizontalScrollBar(), 400);
    adjustScrollBar(ui->scrollAreaOut->verticalScrollBar(), 400);

    //zoomInAct->setEnabled(scaleFactor < 3.0);
    //zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
 {
    scrollBar->setValue(int(factor * scrollBar->value()
                                 + ((factor - 1) * scrollBar->pageStep()/2)));
 }

MainWindow::~MainWindow()
{
    delete ui;
}


Mat tresholding(Mat base, int tresh){
    Size size = base.size();
    int width = size.width;
    int height = size.height;

    Mat otsu = Mat(height, width, CV_8UC1);

    for(int i=0; i<height; ++i){
        for(int j=0; j<width; ++j)
        {
            if(base.at<uchar>(i, j) > tresh)
            {
                otsu.at<uchar>(i, j) = 255;
            }
            else
            {
                otsu.at<uchar>(i, j) = 1;
            }
        }
    }
    return otsu;
}


int otsu(Mat base)
{
    Size size = base.size();
    int width = size.width;
    int height = size.height;

    double q[256];
    double m = 0;
    double m1[256];
    double m2[256];
    double p[256];
    double P[256];
    int sum_pixel = width*height;
    double szoras_b[256];


    for(int c = 0; c<256; ++c)
    {
        p[c]=0;
        for(int i=0; i<height; ++i)
        {
            for(int j=0; j<width; ++j)
            {
                if(base.at<uchar>(i, j) == c)
                {
                    p[c]++;
                }
            }

        }
        P[c]=(p[c])/sum_pixel;
    }
    for(int i=0; i<256; ++i)
    {
        m += i*P[i];
    }

    q[0]=P[0];
    m1[0]=0;
    m2[0]=m;
    for(int t=0; t<255; ++t)
    {

        q[t+1] = q[t] + P[t+1];
        m1[t+1] = (q[t]*m1[t]+ (t+1)*P[t+1])/q[t+1];
        m2[t+1] = (m-q[t+1]*m1[t+1])/(1-q[t+1]);
        double negyzet = pow(m1[t]-m2[t],2);
        szoras_b[t] = (q[t]*(1-q[t]))*negyzet;
    }


    int argmax = 0;
    double max = 0;
    for(int t=0; t<255; ++t)    {
        if(max < szoras_b[t])
        {
            argmax=t;
            max = szoras_b[t];
        }
    }
    cout << "optimalis treshold " << argmax << endl;
    return argmax;
}

QImage Mat2QImage(const cv::Mat3b &src) {
        QImage dest(src.cols, src.rows, QImage::Format_RGB888);
        for (int y = 0; y < src.rows; ++y) {
                const cv::Vec3b *srcrow = src[y];
                QRgb *destrow = (QRgb*)dest.scanLine(y);
                for (int x = 0; x < src.cols; ++x) {
                        destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
                }
        }
        return dest;
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
