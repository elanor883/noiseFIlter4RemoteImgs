#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>
#include <numeric>
#include <math.h>
#include "filter.h"

using namespace std;
using namespace cv;

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
                int t =0;
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

void Filter::sort_desc(int mask[], int size)
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
            int t =0;
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
