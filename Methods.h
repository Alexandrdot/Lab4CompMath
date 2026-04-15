using namespace std;

class Methods{
public:
    void NewtonInterpolationPolynomial(vector<vector<float>> Dots, int derivative);
    void LagrangeInterpolationPolynomial(vector<vector<float>> Dots);
    void SmoothingPolynomials(vector<vector<float>> Dots, int a);
    void CubicSplineInterpolation(vector<vector<float>> Dots, int derivative);
    void FirstDerivative();
    vector<float> MyDataForFirstDerivative();
    vector<vector<float>> BuildMatrixKoeff(vector<vector<float>> Koeff, int a, vector<vector<float>> Dots);
    vector<vector<float>> SearchKoeffPolinom(vector<vector<float>> matrix, vector<vector<float>> Dots);
    void FourDegreePolynomials();
    vector<vector<float>> DerivativeFunc(vector<vector<float>> Dots);
    vector<float> GaussMethod(vector<vector<float>> Koeff);
    vector<float> MyDataForFourthDegree();
    vector<float> RunningAlgorithm(vector<vector<float>> matrix);
};
