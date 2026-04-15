#include <iostream>

using namespace std;

class Charts{
public:
    bool c_flag = false;
    bool d1_flag = false;
    bool d2_flag = false;
    bool n_flag = false;
    bool l_flag = false;
    bool f_flag = false;
    bool mnk1_flag = false;
    bool mnk2_flag = false;
    bool mnk3_flag = false;
    bool dots_flag = false;

    int x_start = -10;
    int x_end = 10;

    float axis_length = 10;
    float axis_length_y_neg = -10.0;
    float tick_spacing = 1.0;

    vector<float> _c; //для Лагранжа
    vector<float> _koeff_fourth_degree; //для 4 степени
    vector<float> _koeff_mnk1; //1 степень
    vector<float> _koeff_mnk2; //2 степень
    vector<float> _koeff_mnk3; //3 степень

    vector<vector<float>> n_diff; //для Нютона
    vector<vector<float>> _dots; //массив точек
    
    vector<vector<float>> matrix_koeff_spline;
    
    void PrintNewtonPolynom(vector<vector<float>> diff, vector<vector<float>> Dots);
    void PrintLagrangePolynom(vector<float> C, vector<vector<float>> Dots);
    void PrintFourDegreePolynom(vector<float> koeff);
    void PrintFirstSmoothingPolynomials(vector<float> koeff);
    void PrintSecondSmoothingPolynomials(vector<float> koeff);
    void PrintThirdSmoothingPolynomials(vector<float> koeff);
    void PrintCubicSplinePolinoms(vector<vector<float>> matrix, vector<vector<float>> Dots);
    void PrintFirstDerivativeFunc(vector<vector<float>> diff, vector<vector<float>> Dots);
    void PrintSecondDerivativeFunc(vector<vector<float>> diff, vector<vector<float>> Dots);
    void MyGlutDisplay();
    void MyGlutReshape();
    void Clear();
};
