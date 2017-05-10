#include<sys/socket.h>
#include<sys/types.h>
#include <stdio.h>
#include <math.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>
#include "Computations.h"

/*This file is used for computations, separate processes will
*run these functions at the same time for efficiency and communicate using
*pipes*/

/*Calculates correlation
*Helper functions finalCalculation and getMean
*/
float getCorrelation(char **data, int dataSize){
  int i = 0;
  float C1Data[dataSize];
  float C2Data[dataSize];
  while(i < dataSize){
    int index = 0;
    int iterator = 0;
    while(iterator < 128){
      if(data[i][iterator] == ' '){
          index = iterator;
          iterator = 129;
      }
      iterator++;
    }
    char c1[index];
    char c2[(128 -index)];
    char *a = &data[i][index + 1];
    strncpy(c1, data[i], index);
    strncpy(c2, a, (128 - index));
    C1Data[i] = (float)atof(c1);
    C2Data[i] = (float)atof(c2);
    i++;
  }
  float mean1 = getMean(C1Data, dataSize);
  float mean2 = getMean(C2Data, dataSize);
  printf("mean column1: %f, mean column2: %f\n", mean1, mean2);
  int counter = 0;
  while(counter < dataSize){
    C1Data[counter] = C1Data[counter] - mean1;
    C2Data[counter] = C2Data[counter] - mean2;
    counter++;
  }
  float ABData[dataSize];
  float ASqdData[dataSize];
  float BSqdData[dataSize];
  counter = 0;
  while(counter < dataSize){
    ABData[counter] = C1Data[counter] * C2Data[counter];
    ASqdData[counter] = C1Data[counter] * C1Data[counter];
    BSqdData[counter] = C2Data[counter] * C2Data[counter];
    counter++;
  }
  float correlation = finalCalculation(ABData, ASqdData, BSqdData, dataSize);
  return correlation;
}

//Helper function for getCorrelation
float finalCalculation(float *AB, float *AA, float *BB, int dataSize){
  float sumAB = 0.0;
  float sumAA= 0.0;
  float sumBB = 0.0;
  int counter = 0;
  while(counter < dataSize){
    sumAB += AB[counter];
    sumAA += AA[counter];
    sumBB += BB[counter];
    counter++;
  }
  printf("sum AxB: %f sum AxA: %f sum BxB: %f\n", sumAB, sumAA, sumBB);
  return (sumAB / sqrt((double)(sumAA * sumBB)));
}

//Helper function for getCorrelation
float getMean(float *IntList, int dataSize){
  int counter = 0;
  float sum = 0.0;
  while(counter < dataSize){
    sum += IntList[counter];
    counter++;
  }
  return sum/(float)dataSize;
}


//Calculates the largest spread between two entries on all rows
float largestSpread(char **data, int dataSize){
  int i = 0;
  float C1Data[dataSize];
  float C2Data[dataSize];
  while(i < dataSize){
    int index = 0;
    int iterator = 0;
    while(iterator < 128){
      if(data[i][iterator] == ' '){
          index = iterator;
          iterator = 129;
      }
      iterator++;
    }
    char c1[index];
    char c2[(128 -index)];
    char *a = &data[i][index + 1];
    strncpy(c1, data[i], index);
    strncpy(c2, a, (128 - index));
    C1Data[i] = (float)atof(c1);
    C2Data[i] = (float)atof(c2);
    i++;
  }
  int counter = 0;
  float bigSpread = 0.0;
  while(counter < dataSize){
    if(abs(C1Data[counter] - C2Data[counter])>bigSpread){
      bigSpread = abs(C1Data[counter] - C2Data[counter]);
    }
    counter++;
  }
  return bigSpread;
}
