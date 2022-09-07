//
//  LicencePlate.cpp
//  Smart Traffic Managment System
//
//  Created by Shwetank Maheshwari on 26/08/22.
//

#include <stdio.h>
#include "LicencePlate.h"
using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////////
string LicencePlateNum(Mat imgOriginalScene)
{
    string result;

    bool blnKNNTrainingSuccessful = loadKNNDataAndTrainKNN(); // attempt KNN training

    if (blnKNNTrainingSuccessful == false)
    { // if KNN training was not successful
      // show error message

        return ("error: error: KNN traning was not successful"); // and exit program
    }

   // Mat imgOriginalScene; // input image

   // imgOriginalScene = imread("/Users/shwetankmaheshwari/Downloads/10.webp"); // open image

    if (imgOriginalScene.empty())
    {                                                  // if unable to open image
        
                                                       // _getch();                                               // may have to modify this line if not using Windows
        return ("error: image not read from file\n\n");                                    // and exit program
    }

    vector<PossiblePlate> vectorOfPossiblePlates = detectPlatesInScene(imgOriginalScene); // detect plates

    vectorOfPossiblePlates = detectCharsInPlates(vectorOfPossiblePlates); // detect chars in plates

    //imshow("imgOriginalScene", imgOriginalScene); // show scene image

    if (vectorOfPossiblePlates.empty())
    { // if no plates were found
        return("no license plates were detected"); // inform user no plates were found
    }
    else
    { // else
      // if we get in here vector of possible plates has at leat one plate

        // sort the vector of possible plates in DESCENDING order (most number of chars to least number of chars)
        sort(vectorOfPossiblePlates.begin(), vectorOfPossiblePlates.end(), PossiblePlate::sortDescendingByNumberOfChars);

        // suppose the plate with the most recognized chars (the first plate in sorted by string length descending order) is the actual plate
        PossiblePlate licPlate = vectorOfPossiblePlates.front();

       // imshow("imgPlate", licPlate.imgPlate); // show crop of plate and threshold of plate
        //imshow("imgThresh", licPlate.imgThresh);

        if (licPlate.strChars.length() == 0)
        { // if no chars were found in the plate
            cout << endl
                 << "no characters were detected" << endl
                 << endl; // show message
            return ("NULL");   // and exit program
        }

        else
            result = licPlate.strChars;

        return (result);
    }
}
