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
        {"Yellow", Vec3b(3, 109, 183)},
        {"Pink", Vec3b(89, 66, 182)},
        {"Orange", Vec3b(3, 79, 201)},
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

    double frameWidth = cap.get(CAP_PROP_FRAME_WIDTH);  // Width of the frame
    double frameHeight = cap.get(CAP_PROP_FRAME_HEIGHT); // Height of the frame

    cout << "Resolution: " << frameWidth << " x " << frameHeight << endl;

    Mat frame;

    int gridSize = 40;
    int spacing = 160;
    Point gridOrigin(140,70);

    while(true){
        cap >> frame;
        if(frame.empty()){
            cerr << "[ERROR]: Frame blank" << endl;
            break;
        }

        //flipping the frame for a mirrored view
        flip(frame,frame,1);
        
        for(int row=0;row<3;row++){
            for(int col=0;col<3;col++){
                Point topLeft = {gridOrigin.x + col*spacing, gridOrigin.y + row*spacing};
                Point bottomRight = {topLeft.x + gridSize, topLeft.y + gridSize};
                rectangle(frame,topLeft,bottomRight,Scalar(0,0,255),2);

                /*
                int x = grid.x + row*gridSize;
                int y = grid.y + col*gridSize;
                Rect box(x,y,gridSize/3,gridSize/3);
                */

                //rectangle(frame,box,Scalar(0,0,255),2);
            }
        }
        
        imshow("Camera Feed",frame);        
        
        //vector<float> B={};
        //vector<float> G={};
        //vector<float> R={};
        //float sumB=0;
        //float sumG=0;
        //float sumR=0;
        

        char pressed_key = (char)waitKey(30);
        if(pressed_key == 'q') break;
        if(pressed_key == 'c'){

            for (int row = 0; row < 3; row++){
                for (int col = 0; col < 3; col++){

                    Point topLeft = {gridOrigin.x + col*spacing, gridOrigin.y + row*spacing};
                    Point bottomRight = {topLeft.x + gridSize, topLeft.y + gridSize};

                    //int x = grid.x + row * gridSize;
                    //int y = grid.y + col * gridSize;

                    //Rect box(topLeft.x,topLeft.y, gridSize,gridSize);
        
                    //Mat roi = frame(box);

                    //Scalar avgColor = mean(roi);

                    Point center = {(topLeft.x+bottomRight.x)/2,(topLeft.y+bottomRight.y)/2};

                    Vec3b pixel = frame.at<Vec3b>(center);

                    string approxColor = closestColor(pixel);

                    
                    //cout << "Square[" << row << "]["<< col << "] BGR values= " << "[" << avgColor[0] 
                    //<<  "," << avgColor[1] << "," << avgColor[2] << "]" << endl;
                    cout << "Square[" << col << "]["<< row << "]: " << approxColor << endl;

                    //B.push_back(avgColor[0]);
                    //G.push_back(avgColor[1]);
                    //R.push_back(avgColor[2]);
                }
            }
            
            //for(float num : B){
                //sumB+=num;
            //}

            //cout << "Average B:" << static_cast<double>(sumB)/B.size() << endl;

            //for(float num : G){
                //sumG+=num;
            //}

            //cout << "Average G:" << static_cast<double>(sumG)/G.size() << endl;

            //for(float num : R){
                //sumR+=num;
            //}

            //cout << "Average R:" << static_cast<double>(sumR)/R.size() << endl;
            
        }

    }

    cap.release();
    destroyAllWindows();
    return 0;
}