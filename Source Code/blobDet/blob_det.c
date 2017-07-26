////////////////////////////////////////////////////////////////////////////////
//  File:           blob_det.cpp
//  Description:    Detects 10 blobs in a 64x64 matrix
//  By:             MTM
////////////////////////////////////////////////////////////////////////////////

#include <ac_fixed.h>
#include "blob_det.h"
#include <iostream>
#define N 4096
#define M 64
#define T 63

void blob_check(ac_int<1, false> value[M][M], int x, int y,int& listSize, ac_int<6, false> xList[N], ac_int<6, false> yList[N])
{
    if(x !=0 && value[x -1][y] == 1){ 
        xList[listSize] = x;
        yList[listSize] = y;
        listSize++;
        value[x -1][y] = 0;
    }
    if(y !=T && value[x][y +1] == 1){
        xList[listSize] = x; 
        yList[listSize] = y+1;
        listSize++;
        value[x][y +1] = 0;
    }
    if(x !=T && value[x +1][y] == 1){
        xList[listSize] = x +1;
        yList[listSize] = y; 
        listSize++;
        value[x +1][y] = 0;
    }
    if(x !=0 && value[x][y-1] == 1){
        xList[listSize] = x; 
        yList[listSize] = y-1;
        listSize++;
        value[x][y-1] = 0;
    }
}

#pragma hls_design top

void blob_det(ac_int<1,false>& vin, ac_int<1,false>& enable_copy, ac_int<1,false>& startFrame,static ac_int<14,false> *area0,static ac_int<14,false> *area1,static ac_int<14,false> *area2,static ac_int<14,false> *area3,static ac_int<14,false> *area4,static ac_int<14,false> *area5,static ac_int<14,false> *area6,static ac_int<14,false> *area7,static ac_int<14,false> *area8,static ac_int<14,false> *area9)
{

    ac_int<1, false> value[M][M]; 
    static ac_int<14,false> sums[10];
    ac_int<6, false> xList[N], yList[N];
    ac_int<14, false> total = 0;
    int listSize = 0,counter = 0,sumsIndex = 0, i = 0, j =0, x = 0;
    static ac_int<1,false> g = 0, reset = 0;

#if 1
	if(startFrame){
     
	FRAME:	for(int p = 0; p < NUM_PIXELS; p++) {		 
		    if(enable_copy){
				value[j][i] = vin;
				total++;
				if(i==T){
					i = 0;
					j++;
				}
				else
					i++;	    
			} 
	    }


	RESET: for(int z = 0; z < 10 && total == 4096; z++){
			sums[z] = 0;
			if(z == 9){
			     reset = 1;
			     total = 0;
			     }
			else
			    reset = 0;
		}

    
	BLOB_DET:	for(x = 0; x<M && reset; x++){
			for(int y = 0; y<M; y++){
				if(value[x][y] == 1){
				    value[x][y] = 0;
					sums[sumsIndex] = 1;
					blob_check(value, x, y, listSize, xList, yList);
					sumsIndex++;
				}
				while(counter < listSize){
				    blob_check(value,xList[counter], yList[counter],listSize,xList,yList);
				    counter++;
				    sums[sumsIndex-1]++;
				}
			}
		    if(x == M-1){
		        g = 1;
		        reset = 0;
		        }
		    else
		        g = 0;
		} 

		if (g){   
		    *area0 = sums[0];
		    *area1 = sums[1];
		    *area2 = sums[2];
		    *area3 = sums[3];
		    *area4 = sums[4];
		    *area5 = sums[5];
		    *area6 = sums[6];
		    *area7 = sums[7];
		    *area8 = sums[8];
		    *area9 = sums[9];
		}

    }
}

     