//
//  Preprocess.cpp
//  openCV_course
//
//  Created by Shwetank Maheshwari on 24/08/22.
//

#include <stdio.h>
// Preprocess.cpp

#include "Preprocess.h"
using namespace cv;
///////////////////////////////////////////////////////////////////////////////////////////////////
void preprocess( Mat &imgOriginal,  Mat &imgGrayscale,  Mat &imgThresh) {
    imgGrayscale = extractValue(imgOriginal);                           // extract value channel only from original image to get imgGrayscale

     Mat imgMaxContrastGrayscale = maximizeContrast(imgGrayscale);       // maximize contrast with top hat and black hat

     Mat imgBlurred;

     GaussianBlur(imgMaxContrastGrayscale, imgBlurred, GAUSSIAN_SMOOTH_FILTER_SIZE, 0);          // gaussian blur

                                                                                                    // call adaptive threshold to get imgThresh
     adaptiveThreshold(imgBlurred, imgThresh, 255.0,  ADAPTIVE_THRESH_GAUSSIAN_C,  THRESH_BINARY_INV, ADAPTIVE_THRESH_BLOCK_SIZE, ADAPTIVE_THRESH_WEIGHT);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
 Mat extractValue( Mat &imgOriginal) {
     Mat imgHSV;
    std::vector< Mat> vectorOfHSVImages;
     Mat imgValue;

     cvtColor(imgOriginal, imgHSV,  COLOR_BGR2HSV);

     split(imgHSV, vectorOfHSVImages);

    imgValue = vectorOfHSVImages[2];

    return(imgValue);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
 Mat maximizeContrast( Mat &imgGrayscale) {
     Mat imgTopHat;
     Mat imgBlackHat;
     Mat imgGrayscalePlusTopHat;
     Mat imgGrayscalePlusTopHatMinusBlackHat;

     Mat structuringElement =  getStructuringElement( MORPH_RECT,  Size(3, 3));

     morphologyEx(imgGrayscale, imgTopHat,  MORPH_TOPHAT, structuringElement);
     morphologyEx(imgGrayscale, imgBlackHat,  MORPH_BLACKHAT, structuringElement);

    imgGrayscalePlusTopHat = imgGrayscale + imgTopHat;
    imgGrayscalePlusTopHatMinusBlackHat = imgGrayscalePlusTopHat - imgBlackHat;

    return(imgGrayscalePlusTopHatMinusBlackHat);
}

