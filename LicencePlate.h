//
//  LicencePlate.h
//  Smart Traffic Managment System
//
//  Created by Shwetank Maheshwari on 26/08/22.
//

#ifndef LicencePlate_h
#define LicencePlate_h
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "DetectPlates.h"
#include "PossiblePlate.h"
#include "DetectChars.h"

#include<iostream>
//#include<conio.h>           // may have to modify this line if not using Windows

//#define SHOW_STEPS            // un-comment or comment this line to show steps or not

string LicencePlateNum(Mat imgOriginalScene);

#endif /* LicencePlate_h */
