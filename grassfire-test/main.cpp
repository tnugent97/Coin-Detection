/*
 * grassFire.cpp
 *
 *  Created on: Apr 27, 2016
 *      Author: MJordahn - Marco - Tom
 */
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

struct tile{
    int x, y, object, value;
};

tile list[4096];

void findNeighbours(tile image[64][64], tile &current, tile list[], int& listSize, int objectNumber);

int main(){

    ifstream infile;
    string inputFile;
    tile inputImage[64][64];

    infile.open("./matrix.txt");

    if (!infile.is_open()){
        cout << "Could not open file" << endl;
        exit(EXIT_FAILURE);
    }

    tile newTile;

    int tmp, i = 0, j = 0;

    while(infile >> tmp){
        newTile.x = i;
        newTile.y = j;
        newTile.object = 0;
        newTile.value = tmp;
        inputImage[i][j] = newTile;
        if(j==63){
            j=0;
            i++;
        }
        else{
            j++;
        }
    }

    int listSize = 0;
    int objectNumber = 0;

    for (int i = 0; i<64; i++){
        for (int j = 0; j<64; j++){
            if((inputImage[i][j]).value == 1){
                objectNumber++;
                (inputImage[i][j]).object = objectNumber;
                (inputImage[i][j]).value = 0;
                findNeighbours(inputImage, inputImage[i][j], list, listSize, objectNumber);
            }
            while (listSize > 0){
                listSize = listSize - 1;
                findNeighbours(inputImage, list[listSize], list, listSize, objectNumber);
            }
        }
    }

    for(int x = 0; x<64; x++){
        for(int y = 0; y<64; y++){
            cout << (inputImage[x][y]).object << "\t";
            if(y == 63){
                cout << endl;
            }
        }
    }

    int objectList[objectNumber];

    for (int i = 0; i<objectNumber; i++){
        objectList[i] = 0;
    }

    int var;

    if (objectNumber > 0){
        for (int i = 0; i<64; i++){
            for (int j = 0; j<64; j++){
                if ((inputImage[i][j]).object != 0){
                    var = (inputImage[i][j]).object;
                    objectList[var-1]++;
                }
            }
        }
    }

    for (int i = 0; i<objectNumber; i++){
        cout << objectList[i] << endl;
    }
}

void findNeighbours(tile image[64][64], tile &current, tile list[], int& listSize, int objectNumber){
    int currentX = current.x;
    int currentY = current.y;
    if (currentX != 0 && image[currentX-1][currentY].value == 1){
        listSize++;
        image[currentX-1][currentY].object = objectNumber;
        image[currentX-1][currentY].value = 0;
        list[listSize-1] = image[currentX-1][currentY];
    }
    if (currentY != 63 && image[currentX][currentY+1].value == 1){
        listSize++;
        image[currentX][currentY+1].object = objectNumber;
        image[currentX][currentY+1].value = 0;
        list[listSize-1] = image[currentX][currentY+1];
    }
    if (currentX != 63 && image[currentX+1][currentY].value == 1){
        listSize++;
        image[currentX+1][currentY].object = objectNumber;
        image[currentX+1][currentY].value = 0;
        list[listSize-1] = image[currentX+1][currentY];
    }
    if (currentY != 0 && image[currentX][currentY-1].value == 1){
        listSize++;
        image[currentX][currentY-1].object = objectNumber;
        image[currentX][currentY-1].value = 0;
        list[listSize-1] = image[currentX][currentY-1];
    }
}
