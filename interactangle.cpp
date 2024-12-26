#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

struct interactiveRect{
    Point topLeft;
    Point bottomRight;
    Scalar color;
    bool isClicked=false;
};

vector<interactiveRect> rectangles;

void onMouse(int event,int x,int y,int,void*){
    if(event==EVENT_LBUTTONDOWN){
        for(auto& rect: rectangles){
            if(rect.topLeft.x <= x && x <= rect.bottomRight.x &&
            rect.topLeft.y <= y && y <= rect.bottomRight.y){
                rect.isClicked=!rect.isClicked;
                rect.color = rect.isClicked?Scalar(0,255,0):Scalar(0,0,255);
                break;
            }
        }
    }
}

int main(){
    Mat canvas = Mat::zeros(Size(600,400),CV_8UC3);
    canvas.setTo(Scalar(255,255,255));
    
    rectangles.push_back({Point(100,100),Point(200,200),Scalar(169,169,169)});
    rectangles.push_back({Point(200,100),Point(300,200),Scalar(169,169,169)});

    namedWindow("Interactive Rectangles");
    setMouseCallback("Interactive Rectangles",onMouse);

    while(true){
        //Redraw canvas
        Mat display=canvas.clone();

        for(const auto& rect:rectangles){
            rectangle(display,rect.topLeft,rect.bottomRight,rect.color,-1);
        }

        imshow("Interactive Rectangles",display);

        if(waitKey(30)=='q')break;
    }

    destroyAllWindows();
    return 0;
}