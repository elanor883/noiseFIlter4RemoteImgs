#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>
#include <numeric>
#include <math.h>
#include "filter.h"
#include <time.h>

#define PI 3.14159265359

using namespace std;
using namespace cv;

void sort_desc(int mask[], int size);
Mat filter_by_radius(Mat spectrum, double radius);

void Filter::median(cv::Mat image, int size, Mat *result){

    int offset = size/2;
    int mask[size*size];
    Size size_img = image.size();
    int width = size_img.width;
    int height = size_img.height;
    time_t timer1, timer2;


    timer1 = time(NULL);

    for(int i= offset; i< height-offset; i++)
    {
        for(int j = offset; j< width-offset; j++)
        {
            int maskIndex = 0;
            for(int k = -offset; k < offset; ++k)
            {
                for(int l = -offset; l < offset; ++l)
                {

                    mask[maskIndex++] = image.at<uchar>(i+l, j+k);
                }
            }
            sort_desc(mask, size);
            result->at<uchar>(i, j) = mask[size/2+1];
        }
    }
    timer2 = time(NULL);
    cout << "eltelt ido: " << difftime(timer2, timer1) << endl;
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

    int offset = size/2;
    int mask[size*size];
    Size size_img = image.size();
    int width = size_img.width;
    int height = size_img.height;
    time_t timer1, timer2;

    timer1 = time(NULL);
    for(int i= offset; i< height-offset; i++)
    {
        for(int j = offset; j< width-offset; j++)
        {
            int sum = 0;
            for(int k = -offset; k < offset; ++k)
            {
                for(int l = -offset; l < offset; ++l)
                {
                    sum = sum + image.at<uchar>(i+l, j+k);
                }
            }

            result->at<uchar>(i, j) = sum/(size*size);
        }
    }
    timer2 = time(NULL);
    cout << "eltelt ido: " << difftime(timer2, timer1) << endl;

}


void Filter::gauss_filter(cv::Mat image, int size, Mat *result, int sigma){

    int offset = (size-1)/2;
    int half = offset+1;
    int mask[size*size];
    Size size_img = image.size();
    int width = size_img.width;
    int height = size_img.height;
    int sum;
    time_t timer1, timer2;

    timer1 = time(NULL);


    for(int i= offset; i< height-offset; i++)
    {
        for(int j = offset; j< width-offset; j++)
        {
            sum = 0;
            for(int k = -offset; k < offset; ++k)
            {
                for(int l = -offset; l < offset; ++l)
                {
                    int fx = abs(k);
                    int fy = abs(l);

                    double w = exp(-(fx*fx + fy*fy) / (2.0 * sigma * sigma)) / (2 * 3.14 * sigma * sigma);
                    // mask[k*size+l] = image.at<uchar>(i+l, j+k);
                    sum = sum + w*image.at<uchar>(i+l, j+k);
                }
            }

            result->at<uchar>(i, j) = sum;
        }
    }
    timer2 = time(NULL);
    cout << "eltelt ido: " << difftime(timer2, timer1) << endl;

}



Mat filter_by_radius(Mat spectrum, double radius){

    Size size_img = spectrum.size();
    int width = size_img.width;
    int height = size_img.height;
    Mat lp_filtered = spectrum.clone();
    cout << width << " " << height;

    Point centre = Point(spectrum.rows / 2, spectrum.cols / 2);
   // cout << spectrum.rows/2 << endl;
    double dist;

    for (int i = 0; i<width; i++){
        for(int j = 0; j< height; j++){


            dist = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
            if (dist<radius){


                lp_filtered.at<float>(i, j) = lp_filtered.at<float>(i, j)*0;
            }
        }


    }



    return lp_filtered;
}


void Filter::low_pass(cv::Mat image, int size, cv::Mat* result, int radius){

    time_t timer1, timer2;

    timer1 = time(NULL);

    cv::Mat fImage;
    image.convertTo(fImage, CV_32F);


    cv::Mat fourierTransform;
    cv::dft(fImage, fourierTransform, cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);


    cv::Mat filtered = filter_by_radius(fourierTransform,radius);

    cv::Mat inverseTransform;
    cv::dft(filtered, inverseTransform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);


    cv::Mat finalImage;
    inverseTransform.convertTo(finalImage, CV_8U);

    *result = finalImage;
    timer2 = time(NULL);
    cout << "eltelt ido: " << difftime(timer2, timer1) << endl;
}

Mat Filter::tresholding(cv::Mat base, int tresh){

    Size size = base.size();
    int width = size.width;
    int height = size.height;
    time_t timer1, timer2;
    timer1 = time(NULL);

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
    timer2 = time(NULL);
    cout << "eltelt ido: " << difftime(timer2, timer1) << endl;
    return otsu;
}


int Filter::otsu(Mat base,cv::Mat* result)
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
