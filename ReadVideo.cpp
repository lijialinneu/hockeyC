#include "opencv2/opencv.hpp"
#include "highgui.h"
#include "sys/time.h"
#include "stdio.h"
#include "iostream"

using namespace cv;
using namespace std;

int main(int, char**)
{


    struct timeval tv;
    gettimeofday(&tv, NULL);
    long startTime = tv.tv_sec * 1000 + tv.tv_usec/1000;

    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    cap.set(3, 320);
    cap.set(4, 240);
    cap.set(5, 60);

    Mat puck, robot;
    namedWindow("video",1);

    int puck_x, puck_y, robot_x, robot_y;
    int puck_r, robot_r;
    int area;

    unsigned char message[20];
   
    for(;;)
    {
    
        Mat frame, hsv;
        cap >> frame; // get a new frame from camera     
        cvtColor(frame, hsv, CV_BGR2HSV);
                       
        inRange(hsv,  Scalar(1, 112, 144), Scalar(28, 255, 255), puck);
        inRange(hsv,  Scalar(90, 117, 79), Scalar(255, 255, 255), robot);
        
        vector<Vec3f> circles_puck, circles_robot;
        HoughCircles(puck, circles_puck, CV_HOUGH_GRADIENT, 1, 100, 15, 7, 6, 8 );
        HoughCircles(robot, circles_robot, CV_HOUGH_GRADIENT, 1, 100, 15, 7, 16, 19 );
   
        if(circles_puck.size() > 0) {
            puck_x = cvRound(circles_puck[0][0]);
            puck_y = cvRound(circles_puck[0][1]);
            puck_r = cvRound(circles_puck[0][2]);            
            Point puck_center(puck_x, puck_y);
            circle( frame, puck_center, 1, Scalar(0,255,0), 3, 8, 0 );
            area = 3.14 * puck_r * puck_r;
        }
        
        if(circles_robot.size() > 0) {
            robot_x = cvRound(circles_robot[0][0]);
            robot_y = cvRound(circles_robot[0][1]);
            robot_r = cvRound(circles_robot[0][2]);            
            Point robot_center(robot_x, robot_y);
            circle( frame, robot_center, 1, Scalar(0,255,0), 3, 8, 0 );                 
        }                
        
        /*
        Serial packet:
            Sync start: 2 bytes: mm
            TimeStamp: 2 bytes
            Pos_X:     2 bytes (0-640)
            Pos_Y:     2 bytes (0-480)
            Object size:     2 bytes (0-400)
            Robot Pos_X:     2 bytes (0-640)
            Robot Pos_Y:     2 bytes (0-480)
        */
        
        // create and send message       
        
        // time
        gettimeofday(&tv, NULL);
        long presentTime = tv.tv_sec * 1000 + tv.tv_usec/1000;  
        long time = presentTime - startTime; 
        
        message[0] = 0x6D; 
        message[1] = 0x6D;
        message[2] = (time >> 8) & 0xFF;
        message[3] = time & 0xFF;
        message[4] = (puck_x >> 8) & 0xFF;
        message[5] = puck_x & 0xFF;
        message[6] = (puck_y >> 8) & 0xFF;
        message[7] = puck_y & 0xFF;
        message[8] = (area >> 8) & 0xFF;
        message[9] = area & 0xFF;
        message[10] = (robot_x >> 8) & 0xFF;
        message[11] = robot_x & 0xFF;
        message[12] = (robot_y >> 8) & 0xFF;
        message[13] = robot_y & 0xFF;
        
        
        
        
        imshow("video", frame);        
        if(waitKey(1) >= 0) break;
    }
    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}