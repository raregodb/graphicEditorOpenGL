//
// Created by Кирилл Деменев on 16.10.2024.
//

#ifndef GRAPHICREDACTOR_MYGLWIDGET_H
#define GRAPHICREDACTOR_MYGLWIDGET_H

#include "structs.h"

class MyGLWidget : public QOpenGLWidget
{
public:
    MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
        this->setMinimumSize(700, 700);
        this->setFixedSize(this->size());

        this->windowWidth = this->width();
        this->windowHeight = this->height();
    }

protected:
    void initializeGL() override
    {
        glClearColor(0.7, 0.5, 0.9, 1);
    }

    void resizeGL(int w, int h) override
    {
        glViewport(0, 0, this->width(), this->height());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    }

    void paintGL() override
    {
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(lineWidth);
        glPointSize(pointSize);

        if (transparencyState) {
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(transparencyType, alphaTransparency);
        }
        else {
            glDisable(GL_ALPHA_TEST);
        }

        if (scissorState) {
            glEnable(GL_SCISSOR_TEST);
            glScissor(scissorX, scissorY, scissorW, scissorH);
        }
        else {
            glDisable(GL_SCISSOR_TEST);
        }

        if (blendState) {
            glEnable(GL_BLEND);
            glBlendFunc(sFactorType, dFactorType);
        }
        else {
            glDisable(GL_BLEND);
        }

        glBegin(figureType);

        for (auto & figurePoint : figurePoints) {
            glColor3f(figurePoint.r, figurePoint.g, figurePoint.b);
            glVertex2f(figurePoint.x, figurePoint.y);
        }
        glEnd();

        drawCurve();

        if (isFractalSeen) {
            if (fractalType == 0) {
                // Определяем начальный треугольник
                generateFractal(fractalXY, fractalGen, fractalLength, M_PI / 8);
            }
            else if (fractalType == 1) {
                if (fractalGen > 6)
                    fractalGen = 6;
                Turtle turtle(fractalXY, M_PI / 2); // Start at the origin facing up
                std::string commands = axiom;

                // Generate the fractal by applying rules multiple times
                for (int i = 0; i < fractalGen; ++i) { // Change the number of iterations as needed
                    //std::cout<<commands<<std::endl;
                    commands = applyRules(commands);
                }

                //std::cout<<commands<<std::endl;
                drawBranch(turtle, commands); // Adjust length as needed
            }
        }

        glFlush();

        update();
    }

    void mousePressEvent(QMouseEvent* event) override{
        mousePosition = event->pos();
        normalizedMousePosX = (2.0f * mousePosition.x() / this->width()) - 1.0f; // Преобразуем в диапазон [-1, 1]
        normalizedMousePosY = 1.0f - (2.0f * mousePosition.y() / this->height()); // Преобразуем в диапазон [-1, 1]
        std::cout<<mousePosition.x()<<"; "<<mousePosition.y()<<"\n";
        std::cout<<normalizedMousePosX<<"; "<<normalizedMousePosY<<"\n";
        std::cout<<color.redF()<< " " << color.greenF() << " " << color.blueF() << std::endl;

        glPoint point(normalizedMousePosX, normalizedMousePosY,
                      color.redF(), color.greenF(), color.blueF());

        // Проверка, выбрана ли контрольная точка
        for (int i = 0; i < controlPoints.size(); i++) {
            if (abs(normalizedMousePosX - controlPoints[i].x) < POINT_RADIUS && abs(normalizedMousePosY - controlPoints[i].y) < POINT_RADIUS) {
                selectedPoint = i;
                break;
            }
        }

        switch (clickTypeDrawing) {
            case figureDrawing:
                figurePoints.push_back(point);
                break;
            case fractalDrawing:
                fractalXY.x = point.x;
                fractalXY.y = point.y;
                break;
            case splineDrawing:
                if (selectedPoint == -1)
                    controlPoints.push_back(point);
                break;
        }

    }

    void mouseMoveEvent(QMouseEvent* event) override {
        if (selectedPoint != -1) {
            normalizedMousePosX = (2.0f * event->pos().x() / this->width()) - 1.0f; // Преобразуем в диапазон [-1, 1]
            normalizedMousePosY = 1.0f - (2.0f * event->pos().y() / this->height()); // Преобразуем в диапазон [-1, 1]

            // Обновляем положение выбранной контрольной точки
            controlPoints[selectedPoint].x = normalizedMousePosX;
            controlPoints[selectedPoint].y = normalizedMousePosY;

            // Перерисовываем экран
            drawCurve();
            update();
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        selectedPoint = -1;
    }

    // Функция для рисования треугольника
    Point drawBrenchTriangle(Point a, float len, float angle) {
        Point c = {a.x + ((2*len) * cosf(angle)), a.y + (len * sinf(angle))};

        glBegin(GL_LINE_LOOP);
        glColor3f(0.207, 0.133, 0.074);
        glVertex2f(a.x, a.y);
        glColor3f(0.6, 0.47, 0.31);
        glVertex2f((a.x + c.x)/2, ((a.y + c.y)/2) - len/10);
        glColor3f(0.498, 0.274, 0.10);
        glVertex2f(c.x, c.y);
        glEnd();


        return c;
    }


    void drawLeafTriangle(float centerX, float centerY, float size, float angle) {
        // Вычисляем координаты вершин треугольника
        float halfSize = size / 2.0f;

        // Вершины треугольника
        Point vertices[3];
        vertices[0] = {centerX + halfSize * cosf(angle), centerY + halfSize * sinf(angle)};
        vertices[1] = {centerX + halfSize * cosf(angle + 2 * M_PI / 3), centerY + halfSize * sinf(angle + 2 * M_PI / 3)};
        vertices[2] = {centerX + halfSize * cosf(angle + 4 * M_PI / 3), centerY + halfSize * sinf(angle + 4 * M_PI / 3)};

        // Рисуем первый треугольник
        glBegin(GL_LINE_LOOP);
        glColor3f(0.13, 0.5, 0.13);
        glVertex2f(vertices[0].x, vertices[0].y);
        glColor3f(0, 0.501, 0);
        glVertex2f(vertices[1].x, vertices[1].y);
        glColor3f(0, 0.392, 0);
        glVertex2f(vertices[2].x, vertices[2].y);
        glEnd();

        // Рисуем второй треугольник, смещенный на размер
        float offsetX = halfSize * cosf(angle + M_PI / 3); // Смещение по X
        float offsetY = halfSize * sinf(angle + M_PI / 3); // Смещение по Y

        vertices[0] = {centerX + offsetX + halfSize * cosf(angle), centerY + offsetY + halfSize * sinf(angle)};
        vertices[1] = {centerX + offsetX + halfSize * cosf(angle + 2 * M_PI / 3), centerY + offsetY + halfSize * sinf(angle + 2 * M_PI / 3)};
        vertices[2] = {centerX + offsetX + halfSize * cosf(angle + 4 * M_PI / 3), centerY + offsetY + halfSize * sinf(angle + 4 * M_PI / 3)};

        // Рисуем второй треугольник
        glBegin(GL_LINE_LOOP);
        glColor3f(0.13, 0.5, 0.13);
        glVertex2f(vertices[0].x, vertices[0].y);
        glColor3f(0, 0.501, 0);
        glVertex2f(vertices[1].x, vertices[1].y);
        glColor3f(0, 0.392, 0);
        glVertex2f(vertices[2].x, vertices[2].y);
        glEnd();
    }


    // Рекурсивная функция для генерации фрактала
    void generateFractal(Point startPoint, int depth, float len, float angle) {
        if (depth == 0) {
            // Рисуем треугольник
            drawLeafTriangle(startPoint.x, startPoint.y, len, angle);
        } else {
            // Находим вершину текущего треугольника
            Point tip = drawBrenchTriangle(startPoint, len, angle); // Вершина треугольника


            // Рекурсивно генерируем два меньших треугольника
            generateFractal(tip, depth - 1, len/2, angle + leftTurn);
            generateFractal(tip, depth - 1, len/2, angle - rightTurn);

            // На последней итерации добавляем "листву"
            if (depth == 1) {
                // Рисуем большие треугольники на концах
                drawLeafTriangle(tip.x,  tip.y, len*2, angle); // Лист
            }
        }
    }

    void drawBranch(Turtle& turtle, const std::string& commands) {
        std::stack<Turtle> stack;
        float len = fractalLength;

        for (char command : commands) {
            switch (command) {
                case 'F':
                    glBegin(GL_LINES);
                    glVertex2f(turtle.x, turtle.y);
                    turtle.x += len * cos(turtle.angle);
                    turtle.y += len * sin(turtle.angle);
                    glVertex2f(turtle.x, turtle.y);
                    glEnd();

                    break;
                case '+':
                    //turtle.angle += turn;
                    turtle.angle += M_PI / 4; // Поворот вправо на 45 градусов
                    break;
                case '-':
                    //turtle.angle -= turn;
                    turtle.angle -= M_PI / 4; // Поворот влево на 45 градусов
                    break;
                case '[':
                    stack.push(turtle);
                    break;
                case ']':
                    if (!stack.empty()) {
                        turtle = stack.top();
                        stack.pop();
                    }
                    break;
                default:
                    break;
            }
        }
    }

    std::string applyRules(const std::string& input) {
        std::string output;
//        for (char c : input) {
//            if (c == 'F') {
//                output += newF;
//            } else {
//                output += c;
//            }
//        }
        for (char c : input) {
            if (c == 'F') {
                output += "FF"; // Применяем правило F -> FF
            } else if (c == 'X') {
                output += "F-X+X+F+F-X"; // Применяем правило X -> F-X+X+F+F-X
            } else {
                output += c; // Оставляем другие символы без изменений
            }
        }

        return output;
    }

    float lagrangeInterpolation(float x, const std::vector<glPoint>& points) {
        float result = 0.0f;
        int n = points.size();

        for (int i = 0; i < n; i++) {
            float term = points[i].y;
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    term *= (x - points[j].x) / (points[i].x - points[j].x);
                }
            }
            result += term;
        }
        return result;
    }

    Point computeTangent(const std::vector<glPoint>& controlPoints, int i) {
        Point tangent = {0.0f, 0.0f};

        if (i > 0 && i < controlPoints.size() - 1) {
            // Используем разности для вычисления касательной
            tangent.x = (controlPoints[i + 1].x - controlPoints[i - 1].x) / 2.0f;
            tangent.y = (controlPoints[i + 1].y - controlPoints[i - 1].y) / 2.0f;
        } else if (i == 0 && controlPoints.size() > 1) {
            // Для первой точки
            tangent.x = controlPoints[1].x - controlPoints[0].x;
            tangent.y = controlPoints[1].y - controlPoints[0].y;
        } else if (i == controlPoints.size() - 1 && controlPoints.size() > 1) {
            // Для последней точки
            tangent.x = controlPoints[controlPoints.size() - 1].x - controlPoints[controlPoints.size() - 2].x;
            tangent.y = controlPoints[controlPoints.size() - 1].y - controlPoints[controlPoints.size() - 2].y;
        }
        return tangent;
    }

    void drawCircle(float centerX, float centerY, float radius, int numSegments) {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < numSegments; i++) {
            float angle = 2.0f * M_PI * float(i) / float(numSegments); // Вычисляем угол
            float x = radius * cosf(angle); // Координата X
            float y = radius * sinf(angle); // Координата Y
            glVertex2f(centerX + x, centerY + y); // Рисуем точку на окружности
        }
        glEnd();
    }

    void drawCurve() {
        if (controlPoints.empty())
            return;
        glBegin(GL_LINE_STRIP);
        for (float x = controlPoints.front().x; x <= controlPoints.back().x; x += 0.01f) {
            float y = lagrangeInterpolation(x, controlPoints);
            glColor3f(0, 1, 0.2);
            glVertex2f(x, y);
        }
        glEnd();

        glColor3f(1, 0, 0);
        glBegin(GL_POINTS);
        for (const auto& point : controlPoints) {
            glVertex2f(point.x, point.y);
        }
        glEnd();

        for (const auto& point : controlPoints) {
            drawCircle(point.x, point.y, 0.05f, 30); // Рисуем круг с радиусом 0.05 и 30 сегментами
        }

        for (int i = 0; i < controlPoints.size(); i++) {
            if (i > 0 && i < controlPoints.size() - 1) {
                // Вычисляем касательную
                Point tangent = computeTangent(controlPoints, i);

                // Рисуем касательную
                glBegin(GL_LINES);
                glVertex2f(controlPoints[i].x, controlPoints[i].y);
                glVertex2f(controlPoints[i].x + tangent.x * 0.6f, controlPoints[i].y + tangent.y * 0.6f); // Увеличиваем длину касательной
                glEnd();
            }
        }
    }


public slots:
    void changeFigureTypes(int index) {
        switch (index) {
            case 0: figureType = GL_POINTS; break;
            case 1: figureType = GL_LINES; break;
            case 2: figureType = GL_LINE_STRIP; break;
            case 3: figureType = GL_LINE_LOOP; break;
            case 4: figureType = GL_TRIANGLES; break;
            case 5: figureType = GL_TRIANGLE_STRIP; break;
            case 6: figureType = GL_TRIANGLE_FAN; break;
            case 7: figureType = GL_QUADS; break;
            case 8: figureType = GL_QUAD_STRIP; break;
            case 9: figureType = GL_POLYGON; break;
            default: figureType = GL_LINE_LOOP; break;
        }
        update();
    }

    void changeTransparencyType(int index) {
        switch (index) {
            case 0: transparencyType = GL_NEVER; break;
            case 1: transparencyType = GL_LESS; break;
            case 2: transparencyType = GL_EQUAL; break;
            case 3: transparencyType = GL_LEQUAL; break;
            case 4: transparencyType = GL_GREATER; break;
            case 5: transparencyType = GL_NOTEQUAL; break;
            case 6: transparencyType = GL_GEQUAL; break;
            case 7: transparencyType = GL_ALWAYS; break;
            default: transparencyType = GL_NEVER; break;
        }
        update();
    }


    void changeLineWidth(int newLineWidth) {
        this->lineWidth = newLineWidth;
        update();
    }

    void changePointSize(int newPointSize) {
        this->pointSize = newPointSize;
        update();
    }

    void changeColor(QColor newColor) {
        this->color = newColor;
        update();
    }

    void changeAlfpha(int newAlpha) {
        this->alphaTransparency = newAlpha/10;
        update();
    }

    void changeTransparencyState(bool check) {
        if (check)
            transparencyState = true;
        else
            transparencyState = false;
        update();
    }

    void changeScissorState(bool check) {
        if (check)
            scissorState = true;
        else
            scissorState = false;
        update();
    }

    void changeScissorX(int newX) {
        this->scissorX = newX;
        update();
    }

    void changeScissorY(int newY) {
        this->scissorY = newY;
        update();
    }

    void changeScissorW(int newW) {
        this->scissorW = newW;
        update();
    }

    void changeScissorH(int newH) {
        this->scissorH = newH;
        update();
    }

    void changeBlendState(bool check) {
        if (check)
            blendState = true;
        else
            blendState = false;
        update();
    }

    void changeSfactorType(int index) {
        switch (index) {
            case 0: sFactorType = GL_ZERO; break;
            case 1: sFactorType = GL_ONE; break;
            case 2: sFactorType = GL_DST_COLOR; break;
            case 3: sFactorType = GL_ONE_MINUS_DST_COLOR; break;
            case 4: sFactorType = GL_SRC_ALPHA; break;
            case 5: sFactorType = GL_ONE_MINUS_SRC_ALPHA; break;
            case 6: sFactorType = GL_DST_ALPHA; break;
            case 7: sFactorType = GL_ONE_MINUS_DST_ALPHA; break;
            case 8: sFactorType = GL_SRC_ALPHA_SATURATE; break;
            default: sFactorType = GL_ZERO; break;
        }
        update();
    }
    void changeDfactorType(int index) {
        switch (index) {
            case 0: dFactorType = GL_ZERO; break;
            case 1: dFactorType = GL_ONE; break;
            case 2: dFactorType = GL_SRC_COLOR; break;
            case 3: dFactorType = GL_ONE_MINUS_SRC_COLOR; break;
            case 4: dFactorType = GL_SRC_ALPHA; break;
            case 5: dFactorType = GL_ONE_MINUS_SRC_ALPHA; break;
            case 6: dFactorType = GL_DST_ALPHA; break;
            case 7: dFactorType = GL_ONE_MINUS_DST_ALPHA; break;
            default: dFactorType = GL_ZERO; break;
        }
        update();
    }

    void showFractal(bool state) {
        if (state)
            isFractalSeen = true;
        else
            isFractalSeen = false;
    }

    void changeFractalType(int index) {
        switch (index) {
            case 0: fractalType = 0; break;
            case 1: fractalType = 1; break;
            default: fractalType = 0; break;
        }
        update();
    }

    void changeFractalGenAmount(int value) {
        this->fractalGen = value;
        update();
    }

    void changeFractalLength(float value) {
        this->fractalLength = value;
        update();
    }

    void changeDrawingType(int index) {
        switch (index) {
            case 0: clickTypeDrawing = figureDrawing; break;
            case 1: clickTypeDrawing = fractalDrawing; break;
            case 2: clickTypeDrawing = splineDrawing; break;
        }
        update();
    }
    ;

private:
    QPoint mousePosition;
    clickType clickTypeDrawing = figureDrawing;
    int selectedPoint = -1; // Индекс выбранной контрольной точки
    const float POINT_RADIUS = 0.05f; // Радиус для определения, когда точка выбрана

    GLfloat normalizedMousePosX;
    GLfloat normalizedMousePosY;

    int windowWidth;
    int windowHeight;

    GLfloat lineWidth = 5;
    GLfloat pointSize = 10;

    std::vector<glPoint> figurePoints;

    GLenum figureType = GL_POINTS;

    GLenum transparencyType = GL_ALWAYS;
    bool transparencyState = false;
    float alphaTransparency = 0;

    bool scissorState = false;
    GLint scissorX = 0;
    GLint scissorY = 0;
    GLint scissorW = 0;
    GLint scissorH = 0;

    bool blendState = false;
    GLenum sFactorType = GL_ZERO;
    GLenum dFactorType = GL_ZERO;


    QColor color;

    Point fractalXY;
    int fractalType = 0;
    std::string axiom = "F";
    std::string newF = "-F+F+[+F-F-]-[-F+F+F]";
    float turn = M_PI / 8; // pi / 8
    float fractalLength = 0.3;
    int fractalGen = 0;
    float leftTurn = M_PI / 6;
    float rightTurn = M_PI / 6;
    bool choosingPlaceForFractal = false;
    bool isFractalSeen = false;


    std::vector<glPoint> controlPoints;

};


#endif //GRAPHICREDACTOR_MYGLWIDGET_H
