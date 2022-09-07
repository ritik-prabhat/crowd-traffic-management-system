#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <set>
#include "LicencePlate.h"
#include "DetectPlates.h"
#include "PossiblePlate.h"
#include "DetectChars.h"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <ctime>
fstream fio;
set<string> code({"RJ","DL","GA","MP","PB","UK","UP"} );
using namespace cv;
using namespace std;
string licenceNum;
set<string> previousOutput;
void GetLicenceNum()
{
   
    Mat img;
        VideoCapture cap(0);
     
        CascadeClassifier plateCascade;
        plateCascade.load("/Users/shwetankmaheshwari/Documents/c++/opencv_c++/Smart Traffic Managment System/indian_license_plate.xml");
     
        if (plateCascade.empty()) { cout << "XML file not loaded" << endl; }
     
        vector<Rect> plates;
     
        while (true)
        {
     
            cap.read(img);
            plateCascade.detectMultiScale(img, plates, 1.1, 10);
     
            for (int i = 0; i < plates.size(); i++)
            {
                Mat imgCrop = img(plates[i]);
                imshow(to_string(i), imgCrop);
                // imwrite("Resources/Plates/" + to_string(i) + ".png", imgCrop);
                rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);

                licenceNum = LicencePlateNum(imgCrop);
                string stateCode, lastNum, midnum;
                
                if(licenceNum.length()==9 ||licenceNum.length()==10 )
                {
                    stateCode.push_back(licenceNum[0]);
                    stateCode.push_back(licenceNum[1]);
                    midnum.push_back(licenceNum[2]);
                    midnum.push_back(licenceNum[3]);
                    lastNum= licenceNum.substr(licenceNum.size()-4);
                    auto pos = code.find(stateCode);
                      if(pos != code.end())
                      {
                          if (all_of(lastNum.begin(), lastNum.end(), ::isdigit)&&all_of(midnum.begin(), midnum.end(), ::isdigit))
                          {
                             
                              if (previousOutput.empty())
                              {
                                  cout<<licenceNum<<endl;
                                  
                                  
                                  
                                  previousOutput.insert(licenceNum);
                              }
                              else if (previousOutput.find(licenceNum)==previousOutput.end())
                              {
                                  cout<<licenceNum<<endl;
                                  
                                  
                                  previousOutput.insert(licenceNum);
                                  
                              }
                                  
                          }
                          
                      
                      }
                    
                }
            }
     
            imshow("Image", img);
            waitKey(10);
        
            
        }

}



int main()
{
    GetLicenceNum();
    
    return 0;
}
