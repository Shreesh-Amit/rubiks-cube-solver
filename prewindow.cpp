#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){

    int squareSize = 50; // Size of each small square
    int margin = 10;     // Space between faces
    int width = squareSize * 12 + margin * 3 + 50;
    int height = squareSize * 9 + margin * 2 + 50;

    Mat preview = Mat::zeros(Size(width,height),CV_8UC3);

    preview.setTo(Scalar(255,255,224));

    map<string,Point> facePositions={
        {"L",Point(25,25+squareSize*3+margin)},
        {"F",Point(25+squareSize*3+margin,25+squareSize*3+margin)},
        {"U",Point(25+squareSize*3+margin,25)},
        {"D",Point(25+squareSize*3+margin,25+squareSize*6+margin*2)},
        {"R",Point(25+squareSize*6+margin*2,25+squareSize*3+margin)},
        {"B",Point(25+squareSize*9+margin*3,25+squareSize*3+margin)}
    };

    for(const auto& face: facePositions){
        string faceName = face.first;
        Point origin = face.second;

        for(int row=0;row<3;row++){
            for(int col=0;col<3;col++){
                Point topLeft = origin + Point(col*squareSize,row*squareSize);
                Point bottomRight = topLeft + Point(squareSize,squareSize);
                rectangle(preview,topLeft,bottomRight,Scalar(169,169,169),-1);
                rectangle(preview,topLeft,bottomRight,Scalar(0,0,0),1);
            }
        }

        Point center = origin + Point(70,80);
        putText(preview,faceName,center,FONT_HERSHEY_COMPLEX,0.5,Scalar(0,0,0),2);
    }

    imshow("Preview",preview);

    waitKey(0);
    destroyAllWindows();
    return 0;

}
