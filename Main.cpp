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
            imgArray[i] = new int[numCols]{};
        }

        CartesianArray = new int*[HoughDist];
        for(int i = 0; i < HoughDist; i++) {
            CartesianArray[i] = new int[HoughAngle]{};
        }

        PolarArray = new int*[HoughDist];
        for(int i = 0; i < HoughDist; i++) {
            PolarArray[i] = new int[HoughAngle]{};
        }

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
        for(int i = 0; i <= numRows; i++){
            for(int j = 0; j <= numCols; j++){
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

};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Incorrect number of arguments" << std::endl;
        return 1;
    }

    ifstream inFile(argv[1]);
    ifstream outFile(argv[2]);
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
    houghTransform.reformatPrettyPrint();
    houghTransform.buildHoughSpace();
    houghTransform.reformatPrettyPrint();
    houghTransform.reformatPrettyPrint();


    inFile.close();
    outFile.close();
    debugFile.close();
}

