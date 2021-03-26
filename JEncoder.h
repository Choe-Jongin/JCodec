#pragma once
#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace cv;

class JEncoder
{
private:
    static JEncoder *instance;

    //hmmm.... (2, 3, 4, etc...)
    int quarterScale;

    JEncoder(int quartScale = 2);
    ~JEncoder();

public:
    static JEncoder *getInstance();

    //peaking quatermovie frame(imgvec) from original frame(img) [start col/row depends on the quartermovie class]
    static void peakquartervec(Mat *img, std::vector<uchar> *imgvec, int startcol = 0, int startrow = 0);

    void setScale( int scale){
        quarterScale = scale;
        std::cout<<"set quarterscale : " << quarterScale << std::endl;
    }
    int getScale( int _pow = 1){
        return pow(quarterScale,_pow);
    }

};