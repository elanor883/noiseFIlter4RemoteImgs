#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../filter/filter.h"
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

    connect(ui->applyFilterBtn, SIGNAL(clicked()), this, SLOT(applyFilter()));
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(openFile, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->zoomInBtn, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(ui->zoomOutBtn, SIGNAL(clicked()), this, SLOT(applyFilter()));
    //  connect(ui->comboFilter, SIGNAL(ui->comboFilter->currentIndexChanged(int)),this,SLOT(handleSelectionChanged(int)));
    //};


    //connect(ui->comboFilter, SIGNAL(highlighted(int)), this, SLOT(applyFilter));
    //new QPushButton(this);
    //zoomInBtn = ui->pushButton;

}

void MainWindow::handleSelectionChanged(int index){
    cout  << "szar";
    QMessageBox* msg = new QMessageBox();
    msg->setWindowTitle("Hello !");
    msg->setText("Selected Index is :"+QString::number(index));
    msg->show();
}
void MainWindow::applyFilter()
{

    Size size_img = img.size();
    int width = size_img.width;
    int height = size_img.height;
    Mat* output = new Mat(height, width, CV_8UC1);


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

   /* if(ui->comboFilter->currentIndex() == 0)
    {
        cout << "kernel: median";
        Filter::median(img,kernelSize, output);

        kernelType = 0;

    }
    else if(ui->comboFilter->currentIndex() == 1)
    {
        kernelType = 1;
        Filter::avarage(img, kernelSize, output);
        cout << "kernel: avarage ";
    }
    else if (ui->comboFilter->currentIndex() == 2)
    {
        kernelType = 2;
        Filter::low_pass(img,kernelSize,output);
        cout << "kernel: low-pass ";
    }
    else if (ui->comboFilter->currentIndex() == 3)
    {
        kernelType = 3;
        cout << "kernel: gauss";
    }*/

    switch (ui->comboFilter->currentIndex())
    {
    case 0: Filter::median(img,kernelSize, output);
        break;
    case 1: Filter::avarage(img, kernelSize, output);
        break;
    case 2: Filter::low_pass(img, kernelSize, output);
        break;
    case 3: cout << "gauss";
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
    cout<< "display start" ;
    //imshow("Otsu" , res);
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
        scaleFactor = 1.0;

        //printAct->setEnabled(true);
        // fitToWindowAct->setEnabled(true);
        //updateActions();

        //if (!fitToWindowAct->isChecked())
        //  imageLabelIn->adjustSize();

        string fname = fileName.toUtf8().constData();
        img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
        if(img.empty())
        {
            //return -1;
        }



        /*      // imshow("Original", img);
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

        //return 0;*/

    }
}

void MainWindow::zoomIn()
{
    cout << "scaleeee" ;
    //scaleImage(1.25);

}

void MainWindow::zoomOut()
{
    cout << "scaleeee" ;
    //scaleImage(0.8);
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
