#include "ac_int.h"
#define minB 100
#define maxB 220
#define minC 350
#define maxC 600
#define minD 230
#define maxD 340

#pragma hls_design top
void areaTotal(ac_int<14, false> area0, ac_int<14, false> area1,ac_int<14, false> area2,ac_int<14, false> area3,ac_int<14, false> area4,ac_int<14, false> area5,ac_int<14, false> area6,ac_int<14, false> area7,ac_int<14, false> area8,ac_int<14, false> area9,static ac_int<14, false> *value_out)
{
    ac_int<14, false> tot_value = 0;
	int i = 0;

	if(minB<area0 && area0<maxB){
		tot_value += 5;
		i++;
	}
	else if(minC<area0 && area0<maxC){
		tot_value += 50;
		i++;
	}
	else if(minD<area0 && area0<maxD){
		tot_value += 100;
		i++;
	}
	else
		i++;
	
	/////////////////////////////////////////////////////
	

	if(minB<area1 && area1<maxB){
		tot_value += 5;
		i++;
	}
	else if(minC<area1 && area1<maxC){
		tot_value += 50;
		i++;
	}
	else if(minD<area1 && area1<maxD){
		tot_value += 100;
		i++;
	}
	else
		i++;
	
	/////////////////////////////////////////////////////
	
    if(minB<area2 && area2<maxB){
		tot_value += 5;
		i++;
	}
	else if(minC<area2 && area2<maxC){
		tot_value += 50;
		i++;
	}
	else if(minD<area2 && area2<maxD){
		tot_value += 100;
		i++;
	}
	else
		i++;
	
	/////////////////////////////////////////////////////
	
	if(minB<area3 && area3<maxB){
		tot_value += 5;
		i++;
	}
	else if(minC<area3 && area3<maxC){
		tot_value += 50;
		i++;
	}
	else if(minD<area3 && area3<maxD){
		tot_value += 100;
		i++;
	}
	else
		i++;
	
	/////////////////////////////////////////////////////
	
	if(minB<area4 && area4<maxB){
		tot_value += 5;
		i++;
	}
	else if(minC<area4 && area4<maxC){
		tot_value += 50;
		i++;
	}
	else if(minD<area4 && area4<maxD){
		tot_value += 100;
		i++;
	}
	else
		i++;
	
	/////////////////////////////////////////////////////
	
	if(minB<area5 && area5<maxB){
		tot_value += 5;
		i++;
	}
	else if(minC<area5 && area5<maxC){
		tot_value += 50;
		i++;
	}
	else if(minD<area5 && area5<maxD){
		tot_value += 100;
		i++;
	}
	else
		i++;
	
	/////////////////////////////////////////////////////
	
	if(minB<area6 && area6<maxB){
		tot_value += 5;
		i++;
	}
	else if(minC<area6 && area6<maxC){
		tot_value += 50;
		i++;
	}
	else if(minD<area6 && area6<maxD){
		tot_value += 100;
		i++;
	}
	else
		i++;
	
	/////////////////////////////////////////////////////
	
	if(minB<area7 && area7<maxB){
		tot_value += 5;
		i++;
	}
	else if(minC<area7 && area7<maxC){
		tot_value += 50;
		i++;
	}
	else if(minD<area7 && area7<maxD){
		tot_value += 100;
		i++;
	}
	else
		i++;
	
	/////////////////////////////////////////////////////
	
	if(minB<area8 && area8<maxB){
		tot_value += 5;
		i++;
	}
	else if(minC<area8 && area8<maxC){
		tot_value += 50;
		i++;
	}
	else if(minD<area8 && area8<maxD){
		tot_value += 100;
		i++;
	}
	else
		i++;
	
	/////////////////////////////////////////////////////
	
	if(minB<area9 && area9<maxB){
		tot_value += 5;
		i++;
	}
	else if(minC<area9 && area9<maxC){
		tot_value += 50;
		i++;
	}
	else if(minD<area9 && area9<maxD){
		tot_value += 100;
		i++;
	}
	else
		i++;
	
	/////////////////////////////////////////////////////

	if(i == 10){
		*value_out = tot_value;
		i = 0;
	}
}

