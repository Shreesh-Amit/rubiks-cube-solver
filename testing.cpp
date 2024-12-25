#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

float distanceEuclidean(Vec3b color,Vec3b target){
    return sqrt(pow(target[0]-color[0],2)+pow(target[1]-color[1],2)+pow(target[2]-color[2],2));
}

string closestColor(Vec3b color){

    map<string, Vec3b> targetColors = {
        {"White", Vec3b(112,118,156)},
        {"Yellow", Vec3b(3, 155, 165)},
        {"Pink", Vec3b(89, 66, 182)},
        {"Orange", Vec3b(44, 120, 202)},
        {"Blue", Vec3b(112, 114, 97)},
        {"Green", Vec3b(44, 136, 137)}
    };

    string closeColor;
    float mindistance = FLT_MAX;

    for(const auto& target : targetColors){
        float distance = distanceEuclidean(color,target.second);
        if(distance < mindistance){
            mindistance = distance;
            closeColor = target.first;
        }
    }

    return closeColor;
}

int main(){
    VideoCapture cap(0);
    if(!cap.isOpened()){
        cerr << "[ERROR]: Camera unable to open" << endl;
        return -1;
    }

    Mat frame;

    while(true){
        cap >> frame;
        if(frame.empty()){
            cerr << "[ERROR]: Frame blank" << endl;
            break;
        }

        //flipping the frame for a mirrored view
        flip(frame,frame,1);

        Rect box(320,240,50,50);
        rectangle(frame,box,Scalar(255,255,255),2);

        imshow("Camera Feed",frame);

        char key =(char)waitKey(30);
        if(key=='q')break;
        if(key=='c'){

         Vec3b center = frame.at<Vec3b>(345,265);

         string approxColor = closestColor(center);

        cout << approxColor << " BGR:" << (int)center[0] << "," << (int)center[1] << "," << (int)center[2]<< endl;

         //Mat roi = frame(box);

         //Scalar avgColor = mean(roi);

         //cout << "BGR:" << avgColor[0] << "," << avgColor[1] << "," << avgColor[2] << endl;

        }

    }

    cap.release();
    destroyAllWindows();
    return 0;
}