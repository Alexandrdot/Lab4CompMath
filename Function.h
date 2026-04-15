#include <iostream>

using namespace std;

class Function{
public:
    double NewtonFunc(float x, vector<vector<float>> diff, vector<vector<float>> Dots);
    double LagrangeFunc(float x, vector<float> C, vector<vector<float>> Dots);
    double SmoothingFunc(float x, vector<float> koeff);
    double CubicSplineFunc(float x, vector<vector<float>> matrix, vector<vector<float>> Dots, int NumberCubicSpline);
    double FourDegreeFunc(float x, vector<float> koeff);
};
