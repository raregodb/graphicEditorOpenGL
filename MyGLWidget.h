//
// Created by Кирилл Деменев on 16.10.2024.
//

#ifndef GRAPHICREDACTOR_MYGLWIDGET_H
#define GRAPHICREDACTOR_MYGLWIDGET_H

#include <fstream>
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

    void initializeShaders() {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Linking
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logLength;
            glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
            char* log = new char[logLength];
            glGetProgramInfoLog(shaderProgram, logLength, &logLength, log);
            std::cerr << "Program link error: " << log << std::endl;
            delete[] log;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void settingUpVertices() {
        // Создание VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Создание VBO для позиций
        glGenBuffers(1, &VBO[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        // Создание VBO для нормалей
        glGenBuffers(1, &VBO[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);

        // Отменяем привязку VAO
        glBindVertexArray(0);

    }


    void initializeGL() override
    {
        glClearColor(0.7, 0.5, 0.9, 1);
        timer.start();

        initializeShaders();
        settingUpVertices();
        QMatrix4x4 model, view, projection;

        // Позиция камеры
        GLfloat eyeX = 0.0f, eyeY = 5.0f, eyeZ = 10.0f;  // Камера на высоте Y = 5
        // Точка, куда смотрит камера
        GLfloat centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
        // Смещаем центр вниз (камера будет смотреть ниже)
        centerY -= 2.0f;  // Сдвигаем точку взгляда вниз по оси Y
        // Направление "вверх"
        GLfloat upX = 0.0f, upY = 1.0f, upZ = 0.0f;
        // Создаём матрицу вида
        view.lookAt(
                QVector3D(eyeX, eyeY, eyeZ),        // Позиция камеры
                QVector3D(centerX, centerY, centerZ), // Точка, куда смотрим
                QVector3D(upX, upY, upZ)           // Направление "вверх"
        );
        model.translate(0.0f, 0.0f, -2.0f);
        projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model.data());
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection.data());
        QMatrix4x4 normalMatrix = model.inverted().transposed();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "normalMatrix"), 1, GL_FALSE, normalMatrix.constData());

        // Передача других униформ (позиции света, цвета и т.д.)
        glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"),  0.3f, 0.5f, 0.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 0.0f, 5.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.0f, 0.0f);
        glUseProgram(shaderProgram);
        // Позиции
        GLuint positionAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(positionAttrib);
        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

        // Нормали
        GLuint normalAttrib = glGetAttribLocation(shaderProgram, "normal");
        glEnableVertexAttribArray(normalAttrib);
        glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        GLfloat lightPosition[] = { 0.0f, 0.0f, 0.2, 1.0f };
        GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat lightDiffuse[] = { 0, 0, 0, 0 };
        GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat baseSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);
        glMaterialfv(GL_FRONT, GL_SPECULAR, baseSpecular);


    }

    void resizeGL(int w, int h) override
    {
        glViewport(0, 0, this->width(), this->height());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    }

    void paintGL() override
    {
        glUseProgram(shaderProgram);
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << std::endl;
        }


        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glRotatef(0.1f, 0.0f, 1.0f, 0.0f);

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

        //drawCurve();



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

        float currentTime = timer.elapsed() / 1000.0f; // Время в секундах
        float lightIntensity = (sin(currentTime) + 1.0f) / 2.0f;
        //glUniform1f(glGetUniformLocation(shaderProgram, "lightIntensity"), 1.0f);

        GLfloat baseDiffuse[] = { this->color.redF(), this->color.greenF(), this->color.blueF(), this->color.alphaF() };
        GLfloat baseAmbient[] = { 0, 0, 0 };

        GLfloat dynamicDiffuse[] = {
                baseDiffuse[0] * lightIntensity,
                baseDiffuse[1] * lightIntensity,
                baseDiffuse[2] * lightIntensity,
                1.0f
        };

        glLightfv(GL_LIGHT0, GL_DIFFUSE, dynamicDiffuse);
        glLightfv(GL_LIGHT0, GL_AMBIENT, baseAmbient);

        glUseProgram(0);

        glBegin(GL_POINTS);
        glColor3f(this->color.redF()/255, this->color.greenF()/255, this->color.blueF()/255);
        glVertex3f(0.0f, 0, -0.2);
        glEnd();

        glUseProgram(shaderProgram);

        drawSurfaceVAO();


        glFlush();

        update();
    }

    void drawSurfaceVAO() {
        if (controlPoints.empty()) {
            return;
        }

        positions.clear();
        normals.clear();

        const int numLayers = 2;      // Количество слоев по оси Z
        const float zStep = 0.5f;     // Шаг по оси Z
        const float dx = 0.01f;        // Шаг по оси X для интерполяции

        std::vector<std::vector<Point>> layers(numLayers);

        // Генерация точек поверхности
        for (int layer = 0; layer < numLayers; ++layer) {
            float z = layer * zStep; // Определяем уровень Z для текущего слоя

            for (float x = controlPoints.front().x; x <= controlPoints.back().x; x += dx) {
                float y = lagrangeInterpolation(x, controlPoints);
                layers[layer].emplace_back(Point{x, y, z}); // Сохраняем точку в слое
            }
        }

        // Создание треугольников между слоями
        for (int layer = 0; layer < numLayers - 1; ++layer) {
            for (size_t i = 0; i < layers[layer].size() - 1; ++i) {
                const auto& p1 = layers[layer][i];
                const auto& p2 = layers[layer][i + 1];
                const auto& p3 = layers[layer + 1][i];
                const auto& p4 = layers[layer + 1][i + 1];

                // Добавляем два треугольника для квадрата
                addTriangle(p1, p2, p3);
                addTriangle(p3, p2, p4);
            }
        }

        // Обновляем буферы с новыми данными
        glBindVertexArray(VAO);

        // Обновляем VBO для позиций
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);

        // Обновляем VBO для нормалей
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(0);


        // Отрисовка поверхности
        GLint uniformCount;
        glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, positions.size() / 3);
        glBindVertexArray(0);
        glUseProgram(0);

        // Рисуем контрольные точки
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        for (const auto& point : controlPoints) {
            glVertex3f(point.x, point.y, 0.0f);
        }
        glEnd();
    }

    void addTriangle(const Point& p1, const Point& p2, const Point& p3) {
        // Позиции вершин
        positions.push_back(p1.x); positions.push_back(p1.y); positions.push_back(p1.z);
        positions.push_back(p2.x); positions.push_back(p2.y); positions.push_back(p2.z);
        positions.push_back(p3.x); positions.push_back(p3.y); positions.push_back(p3.z);

        // Нормали (заранее фиксированные вдоль оси Z)
        normals.push_back(0.0f); normals.push_back(0.0f); normals.push_back(1.0f);
        normals.push_back(0.0f); normals.push_back(0.0f); normals.push_back(1.0f);
        normals.push_back(0.0f); normals.push_back(0.0f); normals.push_back(1.0f);
    }

    void drawSurface() {
        if (controlPoints.empty()) {
            return;
        }


        const int numLayers = 2; // Количество слоев по оси Z
        const float zStep = 0.5f; // Шаг по оси Z

        std::vector<std::vector<Point>> layers(numLayers);

        // Генерируем точки для каждого слоя
        for (int layer = 0; layer < numLayers; ++layer) {
            float z = layer * zStep; // Определяем уровень Z для текущего слоя

            for (float x = controlPoints.front().x; x <= controlPoints.back().x; x += 0.01f) {
                float y = lagrangeInterpolation(x, controlPoints);
                layers[layer].emplace_back(Point{x, y, z}); // Сохраняем точку в слое

                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);

                normals.push_back(0.0f);
                normals.push_back(0.0f);
                normals.push_back(1.0f);
            }
        }

        glBindVertexArray(VAO);


        // Рисуем полигоны между слоями
        for (int layer = 0; layer < numLayers - 1; ++layer) {
            glBegin(GL_QUAD_STRIP); // Используем GL_QUAD_STRIP для рисования квадратных полос
            for (size_t i = 0; i < layers[layer].size(); ++i) {
                // Рисуем два треугольника для каждого квадрата
                glColor3f(0, 1, 0.2); // Цвет поверхности
                glVertex3f(layers[layer][i].x, layers[layer][i].y, layers[layer][layer].z); // Нижняя точка
                glVertex3f(layers[layer + 1][i].x, layers[layer + 1][i].y, layers[layer + 1][layer + 1].z); // Верхняя точка
            }
            glEnd();
        }

        // Рисуем контрольные точки
        glColor3f(1, 1, 0);
        glBegin(GL_POINTS);
        for (const auto& point : controlPoints) {
            glVertex3f(point.x, point.y, 0.0f);
        }
        glEnd();

//        for (const auto& point : controlPoints) {
//            drawCircle(point.x, point.y, 0.05f, 30); // Рисуем круг с радиусом 0.05 и 30 сегментами
//        }
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
                    turtle.angle += turn;
                    break;
                case '-':
                    turtle.angle -= turn;
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
        for (char c : input) {
            if (c == 'F') {
                output += newF;
            } else {
                output += c;
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
    GLuint shaderProgram;
    GLuint VBO[2], VAO;
    std::vector<float> positions;
    std::vector<float> normals;

    QElapsedTimer timer;
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
