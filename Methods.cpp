#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>
#include "Methods.h"
#include "Charts.h"

using namespace std;
//вызываю методы, провожу расчет, вывожу графики через Charts

vector<float> Methods::MyDataForFourthDegree(){
    vector<float> koeff(5);
    for (int i = 0; i < 5; ++i) {
        cout << "Введите коэффициент a" << i << ": ";
        while (!(cin >> koeff[i])) {
            cout << "Ошибка ввода. Пожалуйста, введите число: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return koeff;
}

vector<float> Methods::MyDataForFirstDerivative(){
    vector <float> data(2);
    printf("Общий вид f(x) = V * x ^ 2 \n");
    
    cout << "V: ";
    while (!(cin >> data[0])) {
        cout << "Ошибка ввода. Пожалуйста, введите число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "x: ";
    while (!(cin >> data[1])) {
        cout << "Ошибка ввода. Пожалуйста, введите число: ";
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return data;
}

void Methods::NewtonInterpolationPolynomial(vector<vector<float>> Dots, int derivative = 0){
    long N = Dots.size(); //количество точек
    Charts chart;
    vector<vector<float>> diff(N, vector<float>(N, 0.0f)); //разделенные разности n-го порядка
    
    //(0) = f(xi)
    for (int i = 0; i < N; ++i) {
            diff[0][i] = Dots[i][1];
    }
    //остальные (по формулам разделенных разностей, шаг разный)
    for (int i = 1; i < N; ++i) {
        for (int j = 0; j < N - i; ++j) {
            diff[i][j] = (diff[i - 1][j + 1] - diff[i - 1][j]) / (Dots[j + i][0] - Dots[j][0]);
        }
    }
    //теперь есть все коеффиценты
    //далее вызываем "chart" => "сборщик" => view
    if (derivative == 0)
        chart.PrintNewtonPolynom(diff, Dots);
    
    else if (derivative == 1)
        chart.PrintFirstDerivativeFunc(diff, Dots);
    
    else
        chart.PrintSecondDerivativeFunc(diff, Dots);
}

void Methods::LagrangeInterpolationPolynomial(vector<vector<float>> Dots){
    long N = Dots.size(); //количество точек
    vector<float> C(N); //коэф (знаменатель) + умножил на yi для простоты
    Charts chart;
    
    for (int i = 0; i < N; i++){
        C[i] = Dots[i][1];
        for (int j = 0; j < N; j++){
            if (i != j){
                C[i] *= 1 / (Dots[i][0] - Dots[j][0]);
            }
        }
    }
    //собрали коэффициенты
    chart.PrintLagrangePolynom(C, Dots);
}

vector<vector<float>> Methods::BuildMatrixKoeff(vector<vector<float>> Koeff, int a, vector<vector<float>> Dots){
    //мнк, вычисление матрицы коэффицентов полинома a, b, c, ...
    for (int i = 0; i < Koeff.size(); i++){
        for (int j = 0; j < Koeff.size() + 1; j++){
            double summ_degree = 0;
            for (int k = 0; k < Dots.size(); k++){ //a
                if (j != Koeff.size())
                    summ_degree += pow(Dots[k][0], a - i - j);
                else
                    summ_degree += pow(Dots[k][0], a - i - j + 1) * Dots[k][1];
            }
            Koeff[i][j] = summ_degree;
        }
    }
    return Koeff;
}

void Methods::SmoothingPolynomials(vector<vector<float>> Dots, int a){
    vector<vector<float>> koeff(a+1, vector<float>(a+2));
    Methods methods;
    Charts chart;
    koeff = methods.BuildMatrixKoeff(koeff, a * 2, Dots);
    vector<float> x = GaussMethod(koeff);
    
    if (x.size() != 1){
        if (a == 1)
            chart.PrintFirstSmoothingPolynomials(x);
        else if (a == 2)
            chart.PrintSecondSmoothingPolynomials(x);
        else
            chart.PrintThirdSmoothingPolynomials(x);
    }
    else {
        cout << "Нулевой диагональный элемент, решение не возможно!" << endl;
        return;
    }
}

vector<float> Methods::GaussMethod(vector<vector<float>> matrix){
    //схема единственного деления
    long row = matrix.size(); //кол-во строк
    long column = row + 1; //кол-во столбцов
    vector<float> x(row);
    for (int n = 0; n < column - 2; n++) // N-2 - так как расширенная матрица
    {
        float max_element = -1000;
        int number_row = 0;
        vector<float> copy_row(column);
        for (int k = n; k < row; k++) //поиск макс элемента в столбце на каждом шаге в зависимости от номера столбца
        {
            if (abs(matrix[k][n]) > max_element)
            {
                max_element = abs(matrix[k][n]);
                number_row = k;
                for (int j = 0; j < column; j++)
                {
                    copy_row[j] = matrix[k][j];
                }
            }
        }

        for (int j = 0; j < column; j++)
        {
            matrix[number_row][j] = matrix[n][j];
            matrix[n][j] = copy_row[j];
        }

        if (matrix[n][n] == 0)
        {
            return vector<float>(0);
        }
        for (int i = n + 1; i < row; i++) //строки
        {
            //сейчас я на второй строке
            float koeff = matrix[i][n] / matrix[n][n];
            for (int j = 0; j < column; j++) //элементы
            {
                matrix[i][j] = matrix[i][j] - koeff * matrix[n][j];
            }
        }
    }
    for (int i = int(row) - 1; i >= 0; i--) // i - строки (с конца)
    {
       float sum = 0;
       for (int j = i + 1; j < row; j++)  // j - столбцы (учитываем уже найденные переменные)
       {
           sum += matrix[i][j] * x[j];
       }
       x[i] = (matrix[i] [column - 1] - sum) / matrix[i][i];
    }
    return x;
}

vector<float> Methods::RunningAlgorithm(vector<vector<float>> matrix) {
    long row = matrix.size(); //кол-во строк
    int column = int(row) + 1; //кол-во столбцов
    vector<vector<float>> koeff (row, vector<float> (2, 0.0f));
   
    float A, B, e;
    //прямой ход
    for(int i = 0; i < row; i++)
    {
        //сейчас я на нулевой строке
        if (i == 0)
        {
            A = -matrix[i][i + 1] / matrix[i][i];
            B = matrix[i][column - 1] / matrix[i][i];
        }
        else if (i == row - 1)
        {
            A = 0;
            B = (matrix[i][column - 1] - matrix[i][i - 1] * koeff[i - 1][1]) /
                (matrix[i][i - 1] * koeff[i - 1][0] + matrix[i][i]);
        }
        else
        {
            e = matrix[i][i - 1] * koeff[i - 1][0] + matrix[i][i]; //+
            A = -matrix[i][i + 1] / e; //+
            B = (matrix[i][column - 1] - matrix[i][i - 1] * koeff[i - 1][1]) / e;
        }
        koeff[i][0] = A;
        koeff[i][1] = B;
    }

    //обратный ход
    vector<float> x (row);

    x[column - 2] = koeff[column - 2][1];
    for (int i = column - 3; i >= 0; i--)
    {
        x[i] = koeff[i][0] * x[i + 1] + koeff[i][1];
    }

    return x;
}

void Methods::FourDegreePolynomials(){
    Charts chart;
    vector<float> _koeff = MyDataForFourthDegree();
    chart.PrintFourDegreePolynom(_koeff);
}

vector<vector<float>> Methods::SearchKoeffPolinom(vector<vector<float>> matrix, vector<vector<float>> Dots){
    Methods method;
    long N = Dots.size();
    // для C нужно собрать вначале трехдиагоналку потом решить ее.
    vector<vector<float>> three_diagonal_matrix (N-2, vector<float> (N-1));
    //действия
    for (int i = 0; i < N - 2; ++i) {
        float h_i = Dots[i + 1][0] - Dots[i][0];
        float h_i_plus = Dots[i + 2][0] - Dots[i + 1][0];

        if (i > 0) {
            three_diagonal_matrix[i][i - 1] = h_i;
        }

        three_diagonal_matrix[i][i] = 2 * (h_i + h_i_plus);

        if (i < N - 3) {
            three_diagonal_matrix[i][i + 1] = h_i_plus;
        }
        three_diagonal_matrix[i][N-2] = 3 * (((Dots[i + 2][1] - Dots[i + 1][1]) / h_i_plus) - ((Dots[i + 1][1] - Dots[i][1]) / h_i));

    }
    
    vector<float> c =  method.RunningAlgorithm(three_diagonal_matrix);
    
    matrix[0][2] = 0;    // c[0] = 0;
    matrix[N-2][2] = 0; // c[N-2] = 0;    <--границы
    
    for (int i = 1;i < N-1; i++){
        matrix[i][2] = c[i-1];
    }
    
    for(int i = 0; i < matrix.size(); i++){
        float h = Dots[i+1][0] - Dots[i][0];
        if (i == matrix.size() - 1){
            matrix[i][1] = (Dots[i+1][1] - Dots[i][1]) / h - (0 + 2 * matrix[i][2]) * h / 3; //b
            matrix[i][3] = (0 - matrix[i][2]) / (3 * h); //d
        }
        else{
            matrix[i][1] = (Dots[i+1][1] - Dots[i][1]) / h - (matrix[i+1][2] + 2 * matrix[i][2]) * h / 3; //b
            matrix[i][3] = (matrix[i+1][2] - matrix[i][2]) / (3 * h); //d
        }
    }
    return matrix;
}

void Methods::CubicSplineInterpolation(vector<vector<float>> Dots, int derivative){
    long N = Dots.size();
    Methods methods;
    Charts chart;
    vector<vector<float>> matrix (N-1, vector<float>(4));
    for (int i = 0; i < matrix.size(); i++){
        matrix[i][0] = Dots[i][1];
    }
    //собрали коэфф а;
    matrix = methods.SearchKoeffPolinom(matrix, Dots);
    //теперь имеем все коэффициенты
    if (derivative == 0)
        chart.PrintCubicSplinePolinoms(matrix, Dots);
    
    else if (derivative == 1)
        chart.PrintFirstDerivativeFunc(matrix, Dots);
    
    else
        chart.PrintSecondDerivativeFunc(matrix, Dots);
}

vector<vector<float>> Methods::DerivativeFunc(vector<vector<float>> Dots){
    vector<vector<float>> derivative_values (Dots.size()-2, vector<float>(2));
    for (int i = 1; i < Dots.size() - 1; i++){
        float h = Dots[i+1][0] - Dots[i][0];
        derivative_values[i-1][0] = Dots[i][0];
        derivative_values[i-1][1] = (Dots[i+1][1] - Dots[i-1][1]) / (2 * h);
    }
    return derivative_values;
}

void Methods::FirstDerivative(){
    vector<float> data = MyDataForFirstDerivative();
    //data = [v], [x]
    float step = 0.1;
    vector<vector<float>> func (3, vector<float>(2));
    //func = [ [x1 y1], [x2 y2], [x3 y3] ]
    func[0][0] = data[1] - step; 
    func[1][0] = data[1];
    func[2][0] = data[1] + step;
    
    func[0][1] = data[0] * pow((data[1] - step), 2);
    func[1][1] = data[0] * pow(data[1], 2);
    func[2][1] = data[0] * pow((data[1] + step), 2);
    float first_derivative = (DerivativeFunc(func))[0][1];
    
    cout << "Первая производная равна: "<< first_derivative << endl;
}
