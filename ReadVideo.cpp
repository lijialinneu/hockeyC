#include "opencv2/opencv.hpp"
#include "highgui.h"

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
        
    cap.set(3, 320);
    cap.set(4, 240);
    cao.set(5, 60);

    Mat puck, robot;
    namedWindow("puck",1);
    namedWindow("robot",1);

    for(;;)
    {
    
        Mat frame, hsv;
        cap >> frame; // get a new frame from camera     
        
        cvtColor(frame, hsv, CV_BGR2HSV);
                
        // puck         
        inRange(hsv,  Scalar(1, 112, 144), Scalar(28, 255, 255), puck);

        // robot
        inRange(hsv,  Scalar(90, 117, 79), Scalar(255, 255, 255), robot);
        
        vector<Vec3f> circles_puck, circles_robot;
        HoughCircles(puck, circles_puck, CV_HOUGH_GRADIENT, 1, 100, 15, 7, 6, 8 );
        HoughCircles(robot, circles_robot, CV_HOUGH_GRADIENT, 1, 100, 15, 7, 16, 19 );
   
        for( size_t i = 0; i < circles_puck.size(); i++ )
        {
            Point center(cvRound(circles_puck[i][0]), cvRound(circles_puck[i][1]));
            int radius = cvRound(circles_puck[i][2]);
            circle( frame, center, 1, Scalar(0,255,0), 3, 8, 0 );
        }
        
        for( size_t i = 0; i < circles_robot.size(); i++ )
        {
            Point center(cvRound(circles_robot[i][0]), cvRound(circles_robot[i][1]));
            int radius = cvRound(circles_robot[i][2]);
            circle( frame, center, 1, Scalar(0,255,0), 3, 8, 0 );
        }
        
        
        
        
        
        
        
        
        imshow("puck", frame);
        imshow("robot", frame);
        
        if(waitKey(1) >= 0) break;
    }
    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}