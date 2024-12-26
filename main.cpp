#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
using namespace cv;

struct interactiveRect{
    Point topLeft;
    Point bottomRight;
    Scalar color;
    string currentColor;
    string nextColor;
};

int cycle=0;
int squareSize = 50; // Size of each small square
int margin = 10;     // Space between faces
int width = squareSize * 12 + margin * 3 + 50;
int height = squareSize * 9 + margin * 2 + 50;

vector <string> colors = {"White","Yellow","Pink","Orange","Blue","Green"};

map<string, Scalar> colorMap = {
        {"White", Scalar(255, 255, 255)},
        {"Yellow", Scalar(0, 255, 255)},
        {"Green", Scalar(144, 238, 144)},
        {"Blue", Scalar(255, 191, 0)},
        {"Pink", Scalar(203, 192, 255)},
        {"Orange", Scalar(0, 165, 255)}
    };

map<string,vector<vector<interactiveRect>>> rectangles;

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

void pythonAlgorithm(string cubeState){
    string command = "python3 algorithm.py "+cubeState+" > solution.json";
    system(command.c_str());

    ifstream solutionFile("solution.json");
    if(!solutionFile.is_open()){
        cerr << "[ERROR]: Unable to open solution.json" << endl;
        return;
    }

    json data;
    solutionFile >> data;

    if(data.contains("Solution")){
        cout << "[SOLUTION]: " << data["Solution"] << endl;
    }else{
        cout << "[ERROR]: Invalid Cube State" << endl;
    }

    solutionFile.close();
}

void onMouse(int event,int x,int y,int,void*){

    if(event==EVENT_LBUTTONDOWN){
        if(cycle!=0){
            Point topLeft={25+8*squareSize+2*margin,25+7*squareSize+2*margin};
            Point bottomRight=topLeft+Point(100,40);

            if(topLeft.x <= x && x <=bottomRight.x && topLeft.y <= y && y <=bottomRight.y){
                vector<string> centers = {};
                vector<string> faces={"U","R","F","D","L","B"};
                for(const auto& face:faces){
                    const auto& facelets=rectangles[face];
                    const auto& center = facelets[1][1].currentColor;
                    int cnt = count(centers.begin(),centers.end(),center);
                    if(cnt>0){
                        cout << "[ERROR]: Invalid Cube State" << endl;
                        return;
                    }
                    centers.push_back(center); 
                }

                string cubeState="";
                map<string,string> colorTofaceMap;
                for(int i=0;i<6;i++) colorTofaceMap[centers[i]]=faces[i];
                
                for(const auto& face:faces){
                    const auto& facelets=rectangles[face];
                    for(int row=0;row<3;row++){
                        for(int col=0;col<3;col++){
                            cubeState+=colorTofaceMap[facelets[row][col].currentColor];
                        }
                    }
                }
                
                pythonAlgorithm(cubeState);
            } 
        }

        for (auto &rectmap : rectangles)
            {
                auto &rect = rectmap.second;
                for (int row = 0; row < 3; row++)
                {
                    for (int col = 0; col < 3; col++)
                    {
                        if (rect[row][col].topLeft.x <= x && x <= rect[row][col].bottomRight.x &&
                            rect[row][col].topLeft.y <= y && y <= rect[row][col].bottomRight.y)
                        {
                            rect[row][col].currentColor = rect[row][col].nextColor;
                            rect[row][col].nextColor = colors[((find(colors.begin(), colors.end(), rect[row][col].nextColor) - colors.begin()) + 1) % 6];
                            rect[row][col].color = colorMap[rect[row][col].currentColor];
                            break;
                        }
                    }
                }
            }
        
    }
    
}

int main(){

    map<string, Point> facePositions = {
        {"U", Point(25 + squareSize * 3 + margin, 25)},
        {"R", Point(25 + squareSize * 6 + margin * 2, 25 + squareSize * 3 + margin)},
        {"F", Point(25 + squareSize * 3 + margin, 25 + squareSize * 3 + margin)},
        {"D", Point(25 + squareSize * 3 + margin, 25 + squareSize * 6 + margin * 2)},
        {"L", Point(25, 25 + squareSize * 3 + margin)},
        {"B", Point(25 + squareSize * 9 + margin * 3, 25 + squareSize * 3 + margin)},
    };

    for(const auto& face:facePositions){
        string faceName = face.first;
        Point origin = face.second;

        vector<vector<interactiveRect>> face_rectangles;

        for(int row=0;row<3;row++){

            vector<interactiveRect> rectangle_row;
            for (int col = 0; col < 3; col++) {
                    Point topLeft = origin + Point(col * squareSize, row * squareSize);
                    Point bottomRight = topLeft + Point(squareSize, squareSize);
                    rectangle_row.push_back({topLeft,bottomRight,Scalar(160,169,169),"","White"});
                }
            face_rectangles.push_back(rectangle_row);
        }

        rectangles[faceName]=face_rectangles;
    }

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

    Mat canvas = Mat::zeros(Size(width, height), CV_8UC3);
    canvas.setTo(Scalar(255, 255, 224));

    vector<string> listFaces={"U","R","F","D","L","B"};
    string currentFace=listFaces[0];
    string nextFace=listFaces[1];


    namedWindow("Preview");
    setMouseCallback("Preview",onMouse);

    while(true){

        //Redrawing the preview Screen
        Mat preview=canvas.clone();

        for (const auto &rectmap : rectangles){
            string faceName = rectmap.first;
            vector<vector<interactiveRect>> rect = rectmap.second;

            for (int row = 0; row < 3; row++){
                for (int col = 0; col < 3; col++){
                    rectangle(preview, rect[row][col].topLeft,rect[row][col].bottomRight,rect[row][col].color, -1);
                    rectangle(preview, rect[row][col].topLeft,rect[row][col].bottomRight,Scalar(0,0,0), 1);
                }
            }

            Point center = rect[0][0].topLeft + Point(70, 80);
            putText(preview, faceName, center, FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0), 2);
        }
        
        putText(preview,"Scan the Face: "+currentFace,Point(450,50),FONT_HERSHEY_COMPLEX,0.7,Scalar(0,0,0),1);

        if(cycle!=0){
            Point topLeft={25+8*squareSize+2*margin,25+7*squareSize+2*margin};
            Point bottomRight=topLeft+Point(100,40);
            rectangle(preview,topLeft,bottomRight,Scalar(0,215,255),-1);
            rectangle(preview,topLeft,bottomRight,Scalar(0,0,0),1);
            Point center = topLeft + Point(30,25);
            putText(preview,"Solve",center,FONT_HERSHEY_COMPLEX,0.5,Scalar(0,0,0),1);
        }

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
                Point topLeft = {gridOrigin.x + col*gridSpacing, gridOrigin.y + row*gridSpacing};

                Point bottomRight = {topLeft.x + gridSize, topLeft.y + gridSize};

                Point center = {(topLeft.x + bottomRight.x) / 2, (topLeft.y + bottomRight.y) / 2};

                Vec3b pixel = frame.at<Vec3b>(center);

                string approxColor = closestColor(pixel);

                topLeft = {refOrigin.x + col*(refSize+10), refOrigin.y + row*(refSize+10)};

                bottomRight = {topLeft.x + refSize, topLeft.y + refSize};

                rectangle(frame,topLeft,bottomRight,colorMap[approxColor],-1);

                //Draw borders for the reference square
                rectangle(frame,topLeft,bottomRight,Scalar(0,0,0),1);
            }
        }

        //display the frame
        imshow("Camera Feed",frame);        
        
        imshow("Preview",preview);
        
        
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

                    if(col==0){
                        rectangles[currentFace][row][col+2].color=colorMap[approxColor];
                        rectangles[currentFace][row][col+2].currentColor=approxColor;
                        rectangles[currentFace][row][col+2].nextColor=colors[((find(colors.begin(),colors.end(),approxColor)-colors.begin())+1)%6];
                    }
                    
                    else if(col==2) {
                        rectangles[currentFace][row][col-2].color=colorMap[approxColor];
                        rectangles[currentFace][row][col-2].currentColor=approxColor;
                        rectangles[currentFace][row][col-2].nextColor=colors[((find(colors.begin(),colors.end(),approxColor)-colors.begin())+1)%6];
                    }

                    else {
                        rectangles[currentFace][row][col].color=colorMap[approxColor];
                        rectangles[currentFace][row][col].currentColor=approxColor;
                        rectangles[currentFace][row][col].nextColor=colors[((find(colors.begin(),colors.end(),approxColor)-colors.begin())+1)%6];
                    }

                    //cout << "Square[" << col << "]["<< row << "]: " << approxColor << endl;

                }
            }
            
            currentFace=nextFace;
            nextFace=listFaces[((find(listFaces.begin(),listFaces.end(),nextFace)-listFaces.begin())+1)%6];

            //completed all six faces
            if(currentFace==listFaces[0])cycle++;

        }

        

    }

    //releases the camera
    cap.release();

    //destroys all open opencv windows
    destroyAllWindows();

    return 0;
}