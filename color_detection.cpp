#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//function that returns the Euclidean distance between two pixels
float distanceEuclidean(Vec3b color,Vec3b target){
    return sqrt(pow(target[0]-color[0],2)+pow(target[1]-color[1],2)+pow(target[2]-color[2],2));
}

//function used to approximate the color on the frame with the given 6 colors of the rubik's cube
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

    //instantiating an object cap and opening default camera - 0
    VideoCapture cap(0);


    if(!cap.isOpened()){
        cerr << "[ERROR]: Camera unable to open" << endl;
        return -1;
    }

    /*
    double frameWidth = cap.get(CAP_PROP_FRAME_WIDTH);  // Width of the frame
    double frameHeight = cap.get(CAP_PROP_FRAME_HEIGHT); // Height of the frame
    cout << "Resolution: " << frameWidth << " x " << frameHeight << endl;
    */

    //Mat short for Matrix , a default data structure for storing images 
    Mat frame;

    //The 9 grid squares for capturing color of the cube
    Point gridOrigin(120,120);
    int gridSize = 40;
    int gridSpacing = 100;

    //The reference color squares
    Point refOrigin(450,30);
    int refSize = 50;

    while(true){
        //capturing a image and storing it in frame
        cap >> frame;

        if(frame.empty()){
            cerr << "[ERROR]: Frame blank" << endl;
            break;
        }

        //flipping the frame for a mirrored view
        flip(frame,frame,1);
        
        //drawing the 9 grid squares on the frame
        for(int row=0;row<3;row++){
            for(int col=0;col<3;col++){
                Point topLeft = {gridOrigin.x + col*gridSpacing, gridOrigin.y + row*gridSpacing};
                Point bottomRight = {topLeft.x + gridSize, topLeft.y + gridSize};
                rectangle(frame,topLeft,bottomRight,Scalar(255,255,255),2);
            }
        }

        //drawing the reference squares on the frame
        for(int row=0;row<3;row++){
            for(int col=0;col<3;col++){
                map<string, Scalar> colorMap = {
                    {"White", Scalar(255, 255, 255)},
                    {"Yellow", Scalar(0, 255, 255)},
                    {"Green", Scalar(144, 238, 144)},
                    {"Blue", Scalar(255, 191, 0)},
                    {"Pink", Scalar(203, 192, 255)},
                    {"Orange", Scalar(0, 165, 255)}
                };

                Point topLeft = {gridOrigin.x + col*gridSpacing, gridOrigin.y + row*gridSpacing};

                Point bottomRight = {topLeft.x + gridSize, topLeft.y + gridSize};

                Point center = {(topLeft.x + bottomRight.x) / 2, (topLeft.y + bottomRight.y) / 2};

                Vec3b pixel = frame.at<Vec3b>(center);

                string approxColor = closestColor(pixel);

                topLeft = {refOrigin.x + col*(refSize+10), refOrigin.y + row*(refSize+10)};

                bottomRight = {topLeft.x + refSize, topLeft.y + refSize};

                rectangle(frame,topLeft,bottomRight,colorMap[approxColor],-1);

            }
        }

        //display the frame
        imshow("Camera Feed",frame);        
        
        
        //vector<float> B={};
        //vector<float> G={};
        //vector<float> R={};
        //float sumB=0;
        //float sumG=0;
        //float sumR=0;
        
        //waitKey() waits for key press for time in ms;  if not pressed returns -1 
        //else return ascii values of key pressed
        char pressed_key = (char)waitKey(30);

        if(pressed_key == 'q') break;

        if(pressed_key == 'c'){

            for (int row = 0; row < 3; row++){
                for (int col = 0; col < 3; col++){

                    Point topLeft = {gridOrigin.x + col*gridSpacing, gridOrigin.y + row*gridSpacing};

                    Point bottomRight = {topLeft.x + gridSize, topLeft.y + gridSize};

                    Point center = {(topLeft.x+bottomRight.x)/2,(topLeft.y+bottomRight.y)/2};

                    //extracting the pixel frame center of the square
                    Vec3b pixel = frame.at<Vec3b>(center);

                    //approximates the pixel color to nearest rubiks' cube color
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
    
    //releases the camera
    cap.release();

    //destroys all open opencv windows
    destroyAllWindows();

    return 0;
}