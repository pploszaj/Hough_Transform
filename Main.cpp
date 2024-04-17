#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

class HoughTransform {
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int HoughDist;
    int HoughAngle;
    int **imgArray;
    int **CartesianArray;
    int **PolarArray;
    int angleInDegree;
    double angleInRadians;
    int offSet;

    HoughTransform(int numRows, int numCols, int minVal, int maxVal, int HoughAngle, int HoughDist, int offSet)
            : numRows(numRows), numCols(numCols), minVal(minVal), maxVal(maxVal),
              HoughAngle(HoughAngle), HoughDist(HoughDist), offSet(offSet){

        imgArray = new int*[numRows];
        for(int i = 0; i < numRows; i++) {
            imgArray[i] = new int[numCols];
            fill_n(imgArray[i], numCols, 0);
        }

        CartesianArray = new int*[HoughDist];
        for(int i = 0; i < HoughDist; i++) {
            CartesianArray[i] = new int[HoughAngle];
            fill_n(CartesianArray[i], HoughAngle, 0);
        }

        PolarArray = new int*[HoughDist];
        for(int i = 0; i < HoughDist; i++) {
            PolarArray[i] = new int[HoughAngle];
            fill_n(PolarArray[i], HoughAngle, 0);
        }

    }

    ~HoughTransform(){
        for (int i = 0; i < numRows; i++) {
            delete[] imgArray[i];
        }
        delete[] imgArray;

        for (int i = 0; i < HoughDist; i++) {
            delete[] CartesianArray[i];
            delete[] PolarArray[i];
        }
        delete[] CartesianArray;
        delete[] PolarArray;
    }

    void loadImage(ifstream& inFile) {
        if(!inFile){
            std::cerr << "File is not open" << std::endl;
            return;
        }

        int pixelVal;

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (inFile >> pixelVal) {
                    imgArray[i][j] = pixelVal;
                }
            }
        }
    }

    void reformatPrettyPrint(int** inAry, ofstream& outFile){
        outFile << numRows << numCols << minVal << maxVal << endl;
        string str = to_string(maxVal);
        int width = str.length();
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                if(inAry[i][j] > 0){
                    outFile << inAry[i][j];
                } else {
                    outFile << '.';
                }
                string str2 = to_string(inAry[i][j]);
                int WW = str2.length();
                while (WW <= width){
                    outFile << " ";
                    WW++;
                }
            }
            outFile << endl;
        }
    }

    void computeSinusoid(int x, int y, ofstream& debugFile){
        debugFile << "Entering computeSinusoid()" << endl;
        angleInDegree = 0;
        while(angleInDegree <= 179){
            angleInRadians = (double)angleInDegree / 180.00 * M_PI;
            double dist = cartesianDist(x, y, angleInDegree);
            int distInt = (int)dist;
            if (distInt >= 0 && distInt < HoughDist) {
                CartesianArray[distInt][angleInDegree]++;
            }
            dist = polarDist(x, y, angleInRadians);
            distInt = (int)dist;
            if (distInt >= 0 && distInt < HoughDist) {
                PolarArray[distInt][angleInDegree]++;
            }
            angleInDegree++;
        }

        debugFile << "Leaving computeSinusoid()" << endl;

    }

    void buildHoughSpace(ofstream& debugFile) {
        debugFile << "Entering buildHoughSpace" << endl;
        for (int x = 0; x < numRows; x++) {
            for (int y = 0; y < numCols; y++) {
                if (imgArray[x][y] > 0) {
                    computeSinusoid(x, y, debugFile);
                }
            }
        }
        debugFile << "Leaving buildHoughSpace" << endl;
    }

    double cartesianDist(int x, int y, int angleInDeg) {
        double base = sqrt(x * x + y * y);

        double t = angleInDeg - atan2(y, x) - M_PI / 2;

        double distance = base + cos(t);
        return distance;
    }
    double polarDist(int x, int y, double angleInRad) {
        return x * cos(angleInRad) + y * sin(angleInRad);
    }

};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Incorrect number of arguments" << std::endl;
        return 1;
    }

    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);
    ofstream debugFile(argv[3]);


    if (!inFile || !outFile || !debugFile) {
        std::cerr << "File open error." << std::endl;
        return 1;
    }

    int numRows, numCols, minVal, maxVal;
    inFile >> numRows >> numCols >> minVal >> maxVal;

    int HoughAngle = 180;
    int HoughDist = sqrt(numRows*numRows + numCols*numCols) * 2;
    int offSet = HoughDist / 2;

    HoughTransform houghTransform(numRows, numCols, minVal, maxVal, HoughAngle, HoughDist, offSet);

    houghTransform.loadImage(inFile);
    houghTransform.reformatPrettyPrint(houghTransform.imgArray, outFile);
    houghTransform.buildHoughSpace(debugFile);
    houghTransform.reformatPrettyPrint(houghTransform.CartesianArray, outFile);
    houghTransform.reformatPrettyPrint(houghTransform.PolarArray, outFile);


    inFile.close();
    outFile.close();
    debugFile.close();
}

