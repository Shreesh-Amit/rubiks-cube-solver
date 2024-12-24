#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){
    VideoCapture cap(0);
    if(!cap.isOpened()){
        cerr << "[ERROR] Camera unable to open" << endl;
        return -1;
    }

    Mat frame,mframe,grayframe,edgeframe,roi;
    while(true){
        cap >> frame;
        if(frame.empty()){
            cerr << "[ERROR] Frame empty" << endl;
            break;
        }

        flip(frame,mframe,1);

        cvtColor(mframe,grayframe,COLOR_BGR2GRAY);

        Canny(grayframe,edgeframe,50,150);

        vector<vector<Point>> contours;
        findContours(edgeframe, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (size_t i = 0; i < contours.size(); i++){
            Rect boundingBox = boundingRect(contours[i]);

            float aspectRatio = (float)boundingBox.width / boundingBox.height;

            // checking if the countours are a square
            // aspectRatio for square ~ 1
            if ((0.9 < aspectRatio) && (aspectRatio < 1.1) && boundingBox.area() > 5000){

                // Draws boundingBox on frame
                rectangle(mframe, boundingBox, Scalar(0, 0, 255), 2);

                // Extract Region of Interest on frame wrt boundingBox
                roi = mframe(boundingBox);

                // Draw Contours on roi
                drawContours(roi, contours, i, Scalar(0, 0, 255), 2);
            }
        }

        imshow("Edges", edgeframe);
        imshow("Camera Feed", mframe);

        if (waitKey(30) == 'q')break;

    }

    cap.release();
    destroyAllWindows();
    return 0;
}