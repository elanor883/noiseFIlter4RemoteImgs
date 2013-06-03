#ifndef FILTER_H
#define FILTER_H

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>
#include <numeric>
#include <math.h>


class Filter {

public:
    static void median(cv::Mat input, int size, cv::Mat* result);
    static void avarage(cv::Mat image, int size, cv::Mat* result);
    static void low_pass(cv::Mat image, int size, cv::Mat *result, int radius);
    static void high_pass(cv::Mat image, int size, cv::Mat *result, int radius);
    static void gauss_filter(cv::Mat image, int size, cv::Mat *result, int sigma);
    static cv::Mat tresholding(cv::Mat base, int tresh);
    static int otsu(cv::Mat base, cv::Mat *result);

};

#endif // FILTER_H
