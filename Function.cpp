#include "Function.h"
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

double Function::NewtonFunc(float x, vector<vector<float>> diff, vector<vector<float>> Dots){
    double y = 0;
    
    for (int i = 0; i < diff.size(); i++){
        double k = diff[i][0];
        for (int j = 0; j < i; j++){
            k *= (x - Dots[j][0]);
        }
        y += k;
    }
    return y;
};

double Function::LagrangeFunc(float x, vector<float> C, vector<vector<float>> Dots){
    double y = 0;
    
    for (int i = 0; i < C.size(); i++){
        double k = C[i];
        for (int j = 0; j < C.size(); j++){
            if (i != j){
                k *= (x - Dots[j][0]);
            }
        }
        y += k;
    }
    return y;
}

double Function::SmoothingFunc(float x, vector<float> koeff){
    double y = 0;
    
    for (int i = 0; i < koeff.size();i++){
        y += koeff[i] * pow(x, koeff.size() - 1 - i);
    }
    return y;
}

double Function::FourDegreeFunc(float x, vector<float> koeff){
    double y = 0;
    
    for (int i = 0; i < koeff.size();i++){
        y += koeff[i] * pow(x, i);
    }
    return y;
}

double Function::CubicSplineFunc(float x, vector<vector<float>> matrix, vector<vector<float>> Dots, int NumberCubicSpline){
    double y = 0;
    for (int i = 0; i < 4;i++){
        y += matrix[NumberCubicSpline][i] * pow(x-Dots[NumberCubicSpline][0], i);
    }
    return y;
}
