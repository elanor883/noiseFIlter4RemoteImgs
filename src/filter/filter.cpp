#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>
#include <numeric>
#include <math.h>
#include "filter.h"

#define PI 3.14159265359

using namespace std;
using namespace cv;

Mat create_spectrum(Mat image);
void sort_desc(int mask[], int size);
Mat filter_by_radius(Mat spectrum, double radius);

void Filter::median(cv::Mat image, int size, Mat *result){
    cout << "median filter fv";
    int offset = size/2;
    int mask[size*size];
    Size size_img = image.size();
    int width = size_img.width;
    int height = size_img.height;
    Mat output = Mat(height, width, CV_8UC1);
    cout<< "median st" ;

    for (int ind = 0; ind< 10; ++ ind){
        for(int i= 0; i< height-offset; i++)
        {
            for(int j = 0; j< width-offset; j++)
            {
                for(int k = 0; k < size; ++k)
                {
                    for(int l = 0; l < size; ++l)
                    {
                        mask[k*size+l] = image.at<uchar>(i+l, j+k);
                    }
                }
                sort_desc(mask, size);
                result->at<uchar>(i+offset, j+offset) = mask[size/2+1];
            }
        }
    }
    cout << "lefutott";
}

void sort_desc(int mask[], int size)
{
    for(int j = 0; j < size - 1; j++)
    {
        for (int k = j + 1; k < size; k++)
        {
            if(mask[j] < mask[k])
            {
                int temp = mask[j];
                mask[j] = mask[k];
                mask[k] = temp;
            }
        }
    }

}

void Filter::avarage(Mat image, int size, Mat *result){
    cout << "median filter fv";
    int offset = (size-1)/2;
    int mask[size*size];
    Size size_img = image.size();
    int width = size_img.width;
    int height = size_img.height;
    int sum;
    //cout << "csatornak: " << image.depth();
    Mat output = Mat(height, width, CV_8UC1);
    //cout <<
    for (int ind = 0; ind< 10; ++ ind){
        for(int i= 0; i< height-offset; i++)
        {
            for(int j = 0; j< width-offset; j++)
            {
                sum = 0;
                for(int k = 0; k < size; ++k)
                {
                    for(int l = 0; l < size; ++l)
                    {
                        mask[k*size+l] = image.at<uchar>(i+l, j+k);
                        sum = sum + mask[k*size+l] ;
                    }
                }

                result->at<uchar>(i+offset, j+offset) = sum/(size*size);
            }
        }
    }

}
/*
static void dft(double inreal[], double inimag[], double outreal[], double outimag[]) {
    int n = sizeof(inreal);
    for (int k = 0; k < n; k++) {  // For each output element
        double sumreal = 0;
        double sumimag = 0;
        for (int t = 0; t < n; t++) {  // For each input element
            sumreal +=  inreal[t]*cos(2*PI * t * k / n) + inimag[t]*sin(2*PI * t * k / n);
            sumimag += -inreal[t]*sin(2*PI * t * k / n) + inimag[t]*cos(2*PI * t * k / n);
        }
        outreal[k] = sumreal;
        outimag[k] = sumimag;
    }
}*/

Mat create_spectrum(Mat image){

    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( image.rows );
    int n = getOptimalDFTSize( image.cols ); // on the border add zero values
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);

    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).

    // imshow("spectrum magnitude", magI);
    // waitKey();
//return magI;
    return filter_by_radius(magI,4.0);
}

Mat filter_by_radius(Mat spectrum, double radius){

    Size size_img = spectrum.size();
    int width = size_img.width;
    int height = size_img.height;
    Mat lp_filtered = Mat(height, width, CV_8UC1);

    for (int i = 0; i<height; i++){
        for(int j = 0; i< width; j++){
            double dst = sqrt(i*i + j*j);
            if (dst <= radius) {
                lp_filtered.at<int>(i, j) = 1;
            }
            else {
                lp_filtered.at<int>(i, j) = 0;
            }
        }
    }

  /*  Mat inverseTransform;
    dft(lp_filtered, inverseTransform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);

    // Back to 8-bits
    Mat finalImage;
    inverseTransform.convertTo(finalImage, CV_8U);
    return finalImage;*/
    return lp_filtered;
}

void Filter::low_pass(cv::Mat image, int size, cv::Mat* result){
    //result = &create_spectrum(image);
    imshow("spectrum magnitude", create_spectrum(image));
}

Mat Filter::tresholding(Mat base, int tresh){
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


int Filter::otsu(Mat base)
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
