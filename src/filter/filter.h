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
    static void low_pass(cv::Mat image, int size, cv::Mat *result);
    static cv::Mat tresholding(cv::Mat base, int tresh);
    static int otsu(cv::Mat base);
};

#endif // FILTER_H
