
#include "Blob.h"
#include <fstream>
#include <string>
#include <iomanip>
#pragma warning(disable : 4996)
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;
//#include<conio.h>//  // remove this line if not using Windows OS
//#define SHOW_STEPS //// un-comment | comment this line to show steps or not

// const global variables
const Scalar SCALAR_BLACK = Scalar(0.0, 0.0, 0.0);
const Scalar SCALAR_WHITE = Scalar(255.0, 255.0, 255.0);
const Scalar SCALAR_YELLOW = Scalar(0.0, 255.0, 255.0);
const Scalar SCALAR_GREEN = Scalar(0.0, 200.0, 0.0);
const Scalar SCALAR_RED = Scalar(0.0, 0.0, 255.0);

// function prototypes
void matchCurrentFrameBlobsToExistingBlobs(vector<Blob> &existingBlobs, vector<Blob> &currentFrameBlobs);
void addBlobToExistingBlobs(Blob &currentFrameBlob, vector<Blob> &existingBlobs, int &intIndex);
void addNewBlob(Blob &currentFrameBlob, vector<Blob> &existingBlobs);
double distanceBetweenPoints(Point point1, Point point2);
void drawAndShowContours(Size imageSize, vector<vector<Point> > contours, string strImageName);
void drawAndShowContours(Size imageSize, vector<Blob> blobs, string strImageName);
bool checkIfBlobsCrossedTheLineRight(vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCountRight);
bool checkIfBlobsCrossedTheLineLeft(vector<Blob> &blobs, int &intHorizontalLinePositionLeft, int &carCountLeft);
void drawBlobInfoOnImage(vector<Blob> &blobs, Mat &imgFrame2Copy);
void drawCarCountOnImage(int &carCountRight, Mat &imgFrame2Copy);

// global variables
stringstream date;
int carCountLeft, intVerticalLinePosition, carCountRight = 0;

int main(void) {
    VideoCapture capVideo;
    
    Mat imgFrame1;
    Mat imgFrame2;
    vector<Blob> blobs;
    Point crossingLine[2];
    Point crossingLineLeft[2];
    
  capVideo.open("/Users/shwetankmaheshwari/Downloads/vehicle_counting-master-2/src/HSCC Interstate Highway Surveillance System - TEST VIDEO.mp4");

   if (!capVideo.isOpened()) {                                                 // if unable to open video file
      cout << "error reading video file" << endl << endl;      // show error message
                                                                      // remove this line if not using Windows OS
    return(0);                                                              // and exit program
   }

    if (capVideo.get(CAP_PROP_FRAME_COUNT) < 2) {
       cout << "error: video file must have at least two frames";
                                                                   // remove this line if not using Windows OS
    return(0);
    }

    capVideo.read(imgFrame1);
    capVideo.read(imgFrame2);

    //CONTROL LINE FOR CARCOUNT ~AREA1 (RIGHT WAY)
    int intHorizontalLinePosition = (int)round((double)imgFrame1.rows * 0.35);
    intHorizontalLinePosition = intHorizontalLinePosition*1.40;
    intVerticalLinePosition = (int)round((double)imgFrame1.cols * 0.35);

    crossingLine[0].x = 515;
    crossingLine[0].y = intHorizontalLinePosition;

    crossingLine[1].x = imgFrame1.cols - 1;
    crossingLine[1].y = intHorizontalLinePosition;

    //CONTROL LINE FOR CARCOUNT ~AREA2 (LEFT WAY)
    crossingLineLeft[0].x = 0;
    crossingLineLeft[0].y = intHorizontalLinePosition;

    crossingLineLeft[1].x = 300;
    crossingLineLeft[1].y = intHorizontalLinePosition;

    char chCheckForEscKey = 0;
    bool blnFirstFrame = true;
    int frameCount = 2;

    while (true) {
        vector<Blob> currentFrameBlobs;
        Mat imgFrame1Copy = imgFrame1.clone();
        Mat imgFrame2Copy = imgFrame2.clone();
        Mat imgDifference;
        Mat imgThresh;
        cvtColor(imgFrame1Copy, imgFrame1Copy, COLOR_BGR2GRAY);
        cvtColor(imgFrame2Copy, imgFrame2Copy, COLOR_BGR2GRAY);
        GaussianBlur(imgFrame1Copy, imgFrame1Copy, Size(5, 5), 0);
        GaussianBlur(imgFrame2Copy, imgFrame2Copy, Size(5, 5), 0);
        absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);
        threshold(imgDifference, imgThresh, 30, 255.0, THRESH_BINARY);
        imshow("imgThresh", imgThresh);
        Mat structuringElement3x3 = getStructuringElement(MORPH_RECT, Size(3, 3));
        Mat structuringElement5x5 = getStructuringElement(MORPH_RECT, Size(5, 5));
        Mat structuringElement7x7 = getStructuringElement(MORPH_RECT, Size(7, 7));
        Mat structuringElement15x15 = getStructuringElement(MORPH_RECT, Size(15, 15));

        for (unsigned int i = 0; i < 2; i++) {
            dilate(imgThresh, imgThresh, structuringElement5x5);
            dilate(imgThresh, imgThresh, structuringElement5x5);
            erode(imgThresh, imgThresh, structuringElement5x5);
        }

        Mat imgThreshCopy = imgThresh.clone();
        vector<vector<Point> > contours;
        findContours(imgThreshCopy, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        
        drawAndShowContours(imgThresh.size(), contours, "imgContours");

        vector<vector<Point> > convexHulls(contours.size());

        for (unsigned int i = 0; i < contours.size(); i++) {
            convexHull(contours[i], convexHulls[i]);
        }

        drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

        for (auto &convexHull : convexHulls) {
            Blob possibleBlob(convexHull);

            if (possibleBlob.currentBoundingRect.area() > 400 &&
                possibleBlob.dblCurrentAspectRatio > 0.2 &&
                possibleBlob.dblCurrentAspectRatio < 4.0 &&
                possibleBlob.currentBoundingRect.width > 30 &&
                possibleBlob.currentBoundingRect.height > 30 &&
                possibleBlob.dblCurrentDiagonalSize > 60.0 &&
                (contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.50) {
                    currentFrameBlobs.push_back(possibleBlob);
            }
        }

        drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

        if (blnFirstFrame == true) {
            for (auto &currentFrameBlob : currentFrameBlobs) {
                blobs.push_back(currentFrameBlob);
            }
        }
        else {
            matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
        }

        drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

        imgFrame2Copy = imgFrame2.clone();    // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

        drawBlobInfoOnImage(blobs, imgFrame2Copy);

        // Check the rightWay
        bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLineRight(blobs, intHorizontalLinePosition, carCountRight);
        // Check the leftWay
        bool blnAtLeastOneBlobCrossedTheLineLeft = checkIfBlobsCrossedTheLineLeft(blobs, intHorizontalLinePosition, carCountLeft);
        
        //rightWay
        if (blnAtLeastOneBlobCrossedTheLine == true) {
            line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
        }
        else if (blnAtLeastOneBlobCrossedTheLine == false) {
            line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
        }

        //leftway
        if (blnAtLeastOneBlobCrossedTheLineLeft == true) {
            line(imgFrame2Copy, crossingLineLeft[0], crossingLineLeft[1], SCALAR_WHITE, 2);
        }
        else if (blnAtLeastOneBlobCrossedTheLineLeft == false) {
            line(imgFrame2Copy, crossingLineLeft[0], crossingLineLeft[1], SCALAR_YELLOW, 2);
        }

        drawCarCountOnImage(carCountRight, imgFrame2Copy);

        imshow("imgFrame2Copy", imgFrame2Copy);

        //waitKey(0);    // uncomment this line to go frame by frame for debugging
        
        // now we prepare for the next iteration
        currentFrameBlobs.clear();

        imgFrame1 = imgFrame2.clone();    // move frame 1 up to where frame 2 is

        if ((capVideo.get(CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CAP_PROP_FRAME_COUNT)) {
            capVideo.read(imgFrame2);
        }
        else {
            cout << "end of video\n";
            break;
        }

        blnFirstFrame = false;
        frameCount++;
        chCheckForEscKey = waitKey(1);
    }

    if (chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
        waitKey(0);                         // hold the windows open to allow the "end of video" message to show
    }

    
    waitKey(1);
    // note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows
    return(0);
}


void matchCurrentFrameBlobsToExistingBlobs(vector<Blob> &existingBlobs, vector<Blob> &currentFrameBlobs) {
    for (auto &existingBlob : existingBlobs) {
        existingBlob.blnCurrentMatchFoundOrNewBlob = false;
        existingBlob.predictNextPosition();
    }

    for (auto &currentFrameBlob : currentFrameBlobs) {
        int intIndexOfLeastDistance = 0;
        double dblLeastDistance = 100000.0;

        for (unsigned int i = 0; i < existingBlobs.size(); i++) {

            if (existingBlobs[i].blnStillBeingTracked == true) {
                double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

                if (dblDistance < dblLeastDistance) {
                    dblLeastDistance = dblDistance;
                    intIndexOfLeastDistance = i;
                }
            }
        }

        if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 0.5) {
            addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
        }
        else {
            addNewBlob(currentFrameBlob, existingBlobs);
        }

    }

    for (auto &existingBlob : existingBlobs) {
        if (existingBlob.blnCurrentMatchFoundOrNewBlob == false) {
            existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;
        }
        if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 5) {
            existingBlob.blnStillBeingTracked = false;
        }
    }
}


void addBlobToExistingBlobs(Blob &currentFrameBlob, vector<Blob> &existingBlobs, int &intIndex) {
    existingBlobs[intIndex].currentContour = currentFrameBlob.currentContour;
    existingBlobs[intIndex].currentBoundingRect = currentFrameBlob.currentBoundingRect;
    existingBlobs[intIndex].centerPositions.push_back(currentFrameBlob.centerPositions.back());
    existingBlobs[intIndex].dblCurrentDiagonalSize = currentFrameBlob.dblCurrentDiagonalSize;
    existingBlobs[intIndex].dblCurrentAspectRatio = currentFrameBlob.dblCurrentAspectRatio;
    existingBlobs[intIndex].blnStillBeingTracked = true;
    existingBlobs[intIndex].blnCurrentMatchFoundOrNewBlob = true;
}


void addNewBlob(Blob &currentFrameBlob, vector<Blob> &existingBlobs) {
    currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;
    existingBlobs.push_back(currentFrameBlob);
}


double distanceBetweenPoints(Point point1, Point point2) {
    int intX = abs(point1.x - point2.x);
    int intY = abs(point1.y - point2.y);

    return(sqrt(pow(intX, 2) + pow(intY, 2)));
}


void drawAndShowContours(Size imageSize, vector<vector<Point> > contours, string strImageName) {
    Mat image(imageSize, CV_8UC3, SCALAR_BLACK);
    drawContours(image, contours, -1, SCALAR_WHITE, -1);
    imshow(strImageName, image);
}


void drawAndShowContours(Size imageSize, vector<Blob> blobs, string strImageName) {
    Mat image(imageSize, CV_8UC3, SCALAR_BLACK);
    vector<vector<Point> > contours;

    for (auto &blob : blobs) {
        if (blob.blnStillBeingTracked == true) {
            contours.push_back(blob.currentContour);
        }
    }

    drawContours(image, contours, -1, SCALAR_WHITE, -1);
    imshow(strImageName, image);
}


bool checkIfBlobsCrossedTheLineRight(vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCountRight) {
    bool blnAtLeastOneBlobCrossedTheLine = false;

    for (auto blob : blobs) {
        if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
            int prevFrameIndex = (int)blob.centerPositions.size() - 2;
            int currFrameIndex = (int)blob.centerPositions.size() - 1;

            // Left way
            if (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition && blob.centerPositions[currFrameIndex].x > 350) {
                carCountRight++;
                blnAtLeastOneBlobCrossedTheLine = true;
            }
        }
    }

    return blnAtLeastOneBlobCrossedTheLine;
}


bool checkIfBlobsCrossedTheLineLeft(vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCountLeft) {
    bool blnAtLeastOneBlobCrossedTheLineLeft = false;

    for (auto blob : blobs) {
        if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
            int prevFrameIndex = (int)blob.centerPositions.size() - 2;
            int currFrameIndex = (int)blob.centerPositions.size() - 1;

            // Left way
            if (blob.centerPositions[prevFrameIndex].y <= intHorizontalLinePosition && blob.centerPositions[currFrameIndex].y > intHorizontalLinePosition && blob.centerPositions[currFrameIndex].x < 350 && blob.centerPositions[currFrameIndex].x > 0) {
                carCountLeft++;
                blnAtLeastOneBlobCrossedTheLineLeft = true;
            }
        }
    }

    return blnAtLeastOneBlobCrossedTheLineLeft;
}


void drawBlobInfoOnImage(vector<Blob> &blobs, Mat &imgFrame2Copy) {
    for (unsigned int i = 0; i < blobs.size(); i++) {
        if (blobs[i].blnStillBeingTracked == true) {
            rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_RED, 2);
            
            int intFontFace = FONT_HERSHEY_SIMPLEX;
            double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
            int intFontThickness = (int)round(dblFontScale * 1.0);

            putText(imgFrame2Copy, to_string(i), blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
        }
    }
}


void drawCarCountOnImage(int &carCountRight, Mat &imgFrame2Copy) {
    int intFontFace = FONT_HERSHEY_SIMPLEX;
    double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 450000.0;
    int intFontThickness = (int)round(dblFontScale * 2.5);
    
    // Right way
    Size textSize = getTextSize(to_string(carCountRight), intFontFace, dblFontScale, intFontThickness, 0);
    putText(imgFrame2Copy, "Vehicle count:" + to_string(carCountRight), Point(568,25), intFontFace, dblFontScale, SCALAR_RED, intFontThickness);

    // Left way
    Size textSize1 = getTextSize(to_string(carCountLeft), intFontFace, dblFontScale, intFontThickness, 0);
    putText(imgFrame2Copy, "Vehicle count:" + to_string(carCountLeft), Point(10, 25), intFontFace, dblFontScale, SCALAR_YELLOW, intFontThickness);
}


