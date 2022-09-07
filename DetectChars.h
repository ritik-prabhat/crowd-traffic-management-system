//
//  DetectChars.h
//  openCV_course
//
//  Created by Shwetank Maheshwari on 24/08/22.
//

#ifndef DetectChars_h
#define DetectChars_h
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>
using namespace std;
using namespace cv;
#include "Main.h"
#include "PossibleChar.h"
#include "PossiblePlate.h"
#include "Preprocess.h"

// global constants ///////////////////////////////////////////////////////////////////////////////
// constants for checkIfPossibleChar, this checks one possible char only (does not compare to another char)
const int MIN_PIXEL_WIDTH = 2;
const int MIN_PIXEL_HEIGHT = 8;

const double MIN_ASPECT_RATIO = 0.25;
const double MAX_ASPECT_RATIO = 1.0;

const int MIN_PIXEL_AREA = 80;

// constants for comparing two chars
const double MIN_DIAG_SIZE_MULTIPLE_AWAY = 0.3;
const double MAX_DIAG_SIZE_MULTIPLE_AWAY = 5.0;

const double MAX_CHANGE_IN_AREA = 0.5;

const double MAX_CHANGE_IN_WIDTH = 0.8;
const double MAX_CHANGE_IN_HEIGHT = 0.2;

const double MAX_ANGLE_BETWEEN_CHARS = 12.0;

// other constants
const int MIN_NUMBER_OF_MATCHING_CHARS = 3;

const int RESIZED_CHAR_IMAGE_WIDTH = 20;
const int RESIZED_CHAR_IMAGE_HEIGHT = 30;

const int MIN_CONTOUR_AREA = 100;

// external global variables //////////////////////////////////////////////////////////////////////
extern const bool blnShowSteps;
extern  Ptr< ml::KNearest>  kNearest;

// function prototypes ////////////////////////////////////////////////////////////////////////////

bool loadKNNDataAndTrainKNN(void);

 vector<PossiblePlate> detectCharsInPlates( vector<PossiblePlate> &vectorOfPossiblePlates);

 vector<PossibleChar> findPossibleCharsInPlate( Mat &imgGrayscale,  Mat &imgThresh);

bool checkIfPossibleChar(PossibleChar &possibleChar);

 vector< vector<PossibleChar> > findVectorOfVectorsOfMatchingChars(const  vector<PossibleChar> &vectorOfPossibleChars);

 vector<PossibleChar> findVectorOfMatchingChars(const PossibleChar &possibleChar, const  vector<PossibleChar> &vectorOfChars);

double distanceBetweenChars(const PossibleChar &firstChar, const PossibleChar &secondChar);

double angleBetweenChars(const PossibleChar &firstChar, const PossibleChar &secondChar);

 vector<PossibleChar> removeInnerOverlappingChars( vector<PossibleChar> &vectorOfMatchingChars);

 string recognizeCharsInPlate( Mat &imgThresh,  vector<PossibleChar> &vectorOfMatchingChars);



#endif /* DetectChars_h */
