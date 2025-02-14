//
// Created by Кирилл Деменев on 16.10.2024.
//

#include "iostream"

//#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <QApplication>
#include <QPushButton>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QMouseEvent>
//#include "GLFW/glfw3.h"
//#include <glfw3.h>
#include "QMainWindow"
#include "QtCore"
#include "QtWidgets"
#include "vector"
#include <cmath>

#include "shaders/shadersSource.h"

#ifndef GRAPHICEDITOR_STRUCTS_H
#define GRAPHICEDITOR_STRUCTS_H

struct Point {
    float x, y, z;
};

struct Turtle {
    float x, y;
    float angle;

    Turtle(Point fractalStart, float startAngle)
            : x(fractalStart.x), y(fractalStart.y), angle(startAngle) {}
};

class glPoint {
public:
    glPoint(GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B) {
        this->x = x;
        this->y = y;
        this->r = R;
        this->g = G;
        this->b = B;
    }

    GLfloat x;
    GLfloat y;
    GLfloat r;
    GLfloat g;
    GLfloat b;

};


enum clickType {
    figureDrawing,
    fractalDrawing,
    splineDrawing
};

#endif //GRAPHICEDITOR_STRUCTS_H
