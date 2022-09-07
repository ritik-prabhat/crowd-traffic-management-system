//
//  PossibleChar.cpp
//  openCV_course
//
//  Created by Shwetank Maheshwari on 24/08/22.
//

#include <stdio.h>
#include "PossibleChar.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
PossibleChar::PossibleChar(std::vector<cv::Point> _contour) {
    contour = _contour;

    boundingRect = cv::boundingRect(contour);

    intCenterX = (boundingRect.x + boundingRect.x + boundingRect.width) / 2;
    intCenterY = (boundingRect.y + boundingRect.y + boundingRect.height) / 2;

    dblDiagonalSize = sqrt(pow(boundingRect.width, 2) + pow(boundingRect.height, 2));

    dblAspectRatio = (float)boundingRect.width / (float)boundingRect.height;
}
