//
//  PossiblePlate.h
//  openCV_course
//
//  Created by Shwetank Maheshwari on 24/08/22.
//

#ifndef PossiblePlate_h
#define PossiblePlate_h
#include <string>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////
class PossiblePlate {
public:
    // member variables ///////////////////////////////////////////////////////////////////////////
    cv::Mat imgPlate;
    cv::Mat imgGrayscale;
    cv::Mat imgThresh;

    cv::RotatedRect rrLocationOfPlateInScene;

    std::string strChars;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    static bool sortDescendingByNumberOfChars(const PossiblePlate &ppLeft, const PossiblePlate &ppRight) {
        return(ppLeft.strChars.length() > ppRight.strChars.length());
    }

};


#endif /* PossiblePlate_h */
