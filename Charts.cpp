#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include "Charts.h"
#include "Function.h"
#include "Methods.h"
#include <vector>
#include <iostream>

using namespace std;

Charts infochart;

// Функция для вывода текста на экран
void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void DrawDots() {
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (const auto &dot : infochart._dots) {
        glVertex2f(dot[0], dot[1]);
    }
    glEnd();
}

void DrawLegend() {
    float start_x = -infochart.axis_length + 0.5;
    float start_y = infochart.axis_length - 1.0;
    float line_spacing = 0.5;

    if (infochart.n_flag) {
        glColor3f(1, 1, 0);
        renderBitmapString(start_x, start_y, GLUT_BITMAP_HELVETICA_12, "Newton Polynomial");
        start_y -= line_spacing;
    }
    if (infochart.l_flag) {
        glColor3f(0, 1, 1);
        renderBitmapString(start_x, start_y, GLUT_BITMAP_HELVETICA_12, "Lagrange Polynomial");
        start_y -= line_spacing;
    }
    if (infochart.f_flag) {
        glColor3f(1, 0, 1);
        renderBitmapString(start_x, start_y, GLUT_BITMAP_HELVETICA_12, "4th Degree Polynomial");
        start_y -= line_spacing;
    }
    if (infochart.mnk1_flag) {
        glColor3f(0, 1, 0);
        renderBitmapString(start_x, start_y, GLUT_BITMAP_HELVETICA_12, "MNK 1st Degree");
        start_y -= line_spacing;
    }
    if (infochart.mnk2_flag) {
        glColor3f(0, 0, 1);
        renderBitmapString(start_x, start_y, GLUT_BITMAP_HELVETICA_12, "MNK 2nd Degree");
        start_y -= line_spacing;
    }
    if (infochart.mnk3_flag) {
        glColor3f(1, 0.5, 0);
        renderBitmapString(start_x, start_y, GLUT_BITMAP_HELVETICA_12, "MNK 3rd Degree");
        start_y -= line_spacing;
    }
    
    if (infochart.c_flag) {
        glColor3f(0.25, 0.75, 0.1);
        renderBitmapString(start_x, start_y, GLUT_BITMAP_HELVETICA_12, "Cubic Spline");
        start_y -= line_spacing;
    }
    if (infochart.d1_flag) {
        glColor3f(1.0, 0.41, 0.71);
        renderBitmapString(start_x, start_y, GLUT_BITMAP_HELVETICA_12, "First Derivative");
        start_y -= line_spacing;
    }
    if (infochart.d2_flag) {
        glColor3f(0.5, 0.0, 0.5);
        renderBitmapString(start_x, start_y, GLUT_BITMAP_HELVETICA_12, "Second Derivative");
        start_y -= line_spacing;
    }
}

void Axis() {
    float axis_color[] = {0.5, 0.5, 0.5};
    glColor3fv(axis_color);

    // Ось X
    glBegin(GL_LINES);
    glVertex2f(-infochart.axis_length, 0.0);
    glVertex2f(infochart.axis_length, 0.0);
    glEnd();

    // Ось Y
    glBegin(GL_LINES);
    glVertex2f(0.0, infochart.axis_length_y_neg);
    glVertex2f(0.0, infochart.axis_length);
    glEnd();

    // Подписи осей
    glColor3f(0.5, 0.5, 0.5);
    renderBitmapString(infochart.axis_length - 0.5, -0.3, GLUT_BITMAP_HELVETICA_12, "X");
    renderBitmapString(0.3, infochart.axis_length - 0.5, GLUT_BITMAP_HELVETICA_12, "Y");

    float major_tick_length = 0.1;  // Длина основных делений
    float minor_tick_length = 0.05; // Длина маленьких делений

    // Деления и подписи на оси X
        for (float i = -infochart.axis_length; i <= infochart.axis_length; i += infochart.tick_spacing) {
            if (i != 0.0) {
                // Основные деления
                glBegin(GL_LINES);
                glVertex2f(i, -major_tick_length);
                glVertex2f(i, major_tick_length);
                glEnd();
                // Подписи (центрируем)
                char num_str[10];
                sprintf(num_str, "%.0f", i);
                float text_offset = (i < 0) ? -0.25 : -0.15; // Смещение влево для отрицательных
                float label_y_position = -0.5; // Например, -0.5. <-- для опускания подписей 1 2 3... i

                renderBitmapString(i + text_offset, label_y_position, GLUT_BITMAP_8_BY_13, num_str);
            }
            // Маленькие деления
            for (float j = i + 0.1; j < i + 1.0 && j <= infochart.axis_length; j += 0.1) {
                if (j != 0.0) {
                    glBegin(GL_LINES);
                    glVertex2f(j, -minor_tick_length);
                    glVertex2f(j, minor_tick_length);
                    glEnd();
                }
            }
        }

    // Деления и подписи на оси Y
    for (float i = infochart.axis_length_y_neg; i <= infochart.axis_length; i += infochart.tick_spacing) {
        if (i != 0.0) {
            // Основные деления
            glBegin(GL_LINES);
            glVertex2f(-major_tick_length, i);
            glVertex2f(major_tick_length, i);
            glEnd();

            // Подписи
            char num_str[10];
            sprintf(num_str, "%.0f", i);
            renderBitmapString(0.2, i - 0.1, GLUT_BITMAP_8_BY_13, num_str);
        }

        // Маленькие деления
        for (float j = i + 0.1; j < i + 1.0 && j <= infochart.axis_length; j += 0.1) {
            if (j != 0.0) {
                glBegin(GL_LINES);
                glVertex2f(-minor_tick_length, j);
                glVertex2f(minor_tick_length, j);
                glEnd();
            }
        }
    }
}

void display() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    Function func;
    glColor3f(1.0, 1.0, 1.0);
    if (infochart.n_flag){
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (float x = infochart.x_start; x <= infochart.x_end; x += 0.01) {
            float y = func.NewtonFunc(x, infochart.n_diff, infochart._dots);
            glVertex2f(x, y);
        }
        glEnd();
        infochart.dots_flag = true;
    }

    if (infochart.l_flag){
        glColor3f(0, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (float x = infochart.x_start; x <= infochart.x_end; x += 0.01) {
            float y = func.LagrangeFunc(x, infochart._c, infochart._dots);
            glVertex2f(x, y);
        }
        glEnd();
        infochart.dots_flag = true;
    }

    if (infochart.f_flag){
        glColor3f(1, 0, 1);
        glBegin(GL_LINE_STRIP);
        for (float x = infochart.x_start; x <= infochart.x_end; x += 0.01) {
            float y = func.FourDegreeFunc(x, infochart._koeff_fourth_degree);
            glVertex2f(x, y);
        }
        glEnd();
        infochart.dots_flag = true;
    }

    if (infochart.mnk1_flag){
        glColor3f(0, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (float x = infochart.x_start; x <= infochart.x_end; x += 0.01) {
            float y = func.SmoothingFunc(x, infochart._koeff_mnk1);
            glVertex2f(x, y);
        }
        glEnd();
        infochart.dots_flag = true;
    }

    if (infochart.mnk2_flag){
        glColor3f(0, 0, 1);
        glBegin(GL_LINE_STRIP);
        for (float x = infochart.x_start; x <= infochart.x_end; x += 0.01) {
            float y = func.SmoothingFunc(x, infochart._koeff_mnk2);
            glVertex2f(x, y);
        }
        glEnd();
        infochart.dots_flag = true;
    }

    if (infochart.mnk3_flag){
        glColor3f(1, 0.5, 0.5);
        glBegin(GL_LINE_STRIP);
        for (float x = infochart.x_start; x <= infochart.x_end; x += 0.01) {
            float y = func.SmoothingFunc(x, infochart._koeff_mnk3);
            glVertex2f(x, y);
        }
        glEnd();
        infochart.dots_flag = true;
    }
    
    if (infochart.c_flag){
        for (int i = 0; i < infochart.matrix_koeff_spline.size(); i++){
            glColor3f(0.25, 0.75, 0.1);
            glBegin(GL_LINE_STRIP);
            for (float x = infochart._dots[i][0]; x <= infochart._dots[i+1][0]; x += 0.01) {
                float y = func.CubicSplineFunc(x, infochart.matrix_koeff_spline, infochart._dots, i);
                glVertex2f(x, y);
            }
            glEnd();
        }
        infochart.dots_flag = true;
    }
    
    if (infochart.d1_flag){
        Methods methods;

        for (int i = 0; i < infochart.matrix_koeff_spline.size(); i++){
            int j = 0;
            vector<vector<float>> dots_polynom((infochart._dots[i+1][0] - infochart._dots[i][0]) / 0.01 + 4, vector<float> (2));
            vector<vector<float>> dots_derivative((infochart._dots[i+1][0] - infochart._dots[i][0]) / 0.01 + 4, vector<float> (2));
           
            for (float x = infochart._dots[i][0] - 0.02; x <= infochart._dots[i+1][0] + 0.02; x += 0.01) {
                float y = func.CubicSplineFunc(x, infochart.matrix_koeff_spline, infochart._dots, i);
                if (j < dots_polynom.size()){
                    dots_polynom[j][0] = x;
                    dots_polynom[j][1] = y;
                }
                j++;
            }
            dots_derivative = methods.DerivativeFunc(dots_polynom);
            glColor3f(1.0, 0.41, 0.71);
            glBegin(GL_LINE_STRIP);
            for (int t = 0; t < dots_derivative.size(); t++) {
                glVertex2f(dots_derivative[t][0], dots_derivative[t][1]);
            }
            glEnd();
            dots_polynom.clear();
            dots_derivative.clear();
        }
        
    }
    
    if (infochart.d2_flag){
        Methods methods;

        for (int i = 0; i < infochart.matrix_koeff_spline.size(); i++){
            int j = 0;
            vector<vector<float>> dots_polynom((infochart._dots[i+1][0] - infochart._dots[i][0]) / 0.01 + 4, vector<float> (2));
            vector<vector<float>> dots_derivative((infochart._dots[i+1][0] - infochart._dots[i][0]) / 0.01 + 4, vector<float> (2));
           
            for (float x = infochart._dots[i][0] - 0.02; x <= infochart._dots[i+1][0] + 0.02; x += 0.01) {
                float y = func.CubicSplineFunc(x, infochart.matrix_koeff_spline, infochart._dots, i);
                if (j <= dots_polynom.size()-1){
                    dots_polynom[j][0] = x;
                    dots_polynom[j][1] = y;
                }
                j++;
            }
            j = 0;
            dots_derivative =  methods.DerivativeFunc(methods.DerivativeFunc(dots_polynom));
            glColor3f(0.5, 0.0, 0.5);
            glBegin(GL_LINE_STRIP);
            for (int t = 0; t < dots_derivative.size(); t++) {
                glVertex2f(dots_derivative[t][0], dots_derivative[t][1]);
            }
            glEnd();
            dots_polynom.clear();
            dots_derivative.clear();
        }
    }
    if (infochart.dots_flag)
        DrawDots();
    DrawLegend();
    Axis();
    glFlush();
}

void Charts::MyGlutDisplay(){
    glutDisplayFunc(display);
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Поддержание пропорциональности осей
    float aspect = (float)width / (float)height;
    if (aspect >= 1.0) {
        glOrtho(-infochart.axis_length * aspect, infochart.axis_length * aspect, infochart.axis_length_y_neg, infochart.axis_length, -1.0, 1.0);
    } else {
        glOrtho(-infochart.axis_length, infochart.axis_length, infochart.axis_length_y_neg / aspect, infochart.axis_length / aspect, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
}

void Charts::MyGlutReshape(){
    glutReshapeFunc(reshape);
}

void UpdateFlags(){
    glutPostRedisplay();
}
void Charts::PrintNewtonPolynom(vector<vector<float>> diff, vector<vector<float>> Dots){
    infochart.n_flag = true;
    infochart.n_diff = diff;
    infochart._dots = Dots;
    UpdateFlags();
}

void Charts::PrintLagrangePolynom(vector<float> C, vector<vector<float>> Dots){
    infochart.l_flag = true;
    infochart._dots = Dots;
    infochart._c = C;
    UpdateFlags();
}

void Charts::PrintFirstSmoothingPolynomials(vector<float> koeff){
    infochart.mnk1_flag = true;
    infochart._koeff_mnk1 = koeff;
    UpdateFlags();
}
void Charts::PrintSecondSmoothingPolynomials(vector<float> koeff){
    infochart.mnk2_flag = true;
    infochart._koeff_mnk2 = koeff;
    UpdateFlags();
}
void Charts::PrintThirdSmoothingPolynomials(vector<float> koeff){
    infochart.mnk3_flag = true;
    infochart._koeff_mnk3 = koeff;
    UpdateFlags();
}

void Charts::PrintFourDegreePolynom(vector<float> koeff){
    infochart.f_flag = true;
    infochart._koeff_fourth_degree = koeff;
    UpdateFlags();
}

void Charts::PrintCubicSplinePolinoms(vector<vector<float>> matrix, vector<vector<float>> Dots){
    infochart.c_flag = true;
    infochart.matrix_koeff_spline = matrix;
    infochart._dots = Dots;
    UpdateFlags();
}

void Charts::PrintFirstDerivativeFunc(vector<vector<float>> matrix, vector<vector<float>> Dots){
    infochart.d1_flag = true;
    infochart.matrix_koeff_spline = matrix;
    infochart._dots = Dots;
    UpdateFlags();
}

void Charts::PrintSecondDerivativeFunc(vector<vector<float>> matrix, vector<vector<float>> Dots){
    infochart.d2_flag = true;
    infochart.matrix_koeff_spline = matrix;
    infochart._dots = Dots;
    UpdateFlags();
}

void Charts::Clear(){
    infochart.d1_flag = false;
    infochart.d2_flag = false;
    infochart.n_flag = false;
    infochart.c_flag = false;
    infochart.l_flag = false;
    infochart.f_flag = false;
    infochart.dots_flag = false;
    infochart.mnk1_flag = false;
    infochart.mnk2_flag = false;
    infochart.mnk3_flag = false;
    UpdateFlags();
}
