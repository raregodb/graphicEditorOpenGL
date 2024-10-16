#define GL_SILENCE_DEPRECATION

#include "iostream"
#include <GL/glew.h>
#include <QApplication>
#include <QPushButton>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QMouseEvent>
#include <glfw3.h>
#include "QMainWindow"
#include "QtCore"
#include "QtWidgets"
#include "vector"
#include <cmath>




struct Point {
    float x, y;
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
        glClearColor(0.7, 0, 0.9, 1);
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
        if (scissorState) {
            glEnable(GL_SCISSOR_TEST);
            glScissor(scissorX, scissorY, scissorW, scissorH);
        }

        if (blendState) {
            glEnable(GL_BLEND);
            glBlendFunc(sFactorType, dFactorType);
        }

        glBegin(figureType);

        for (auto & figurePoint : figurePoints) {
            glColor3f(figurePoint.r, figurePoint.g, figurePoint.b);
            glVertex2f(figurePoint.x, figurePoint.y);
        }
        glEnd();

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

        if (!transparencyState)
            glDisable(GL_ALPHA_TEST);
        if (!scissorState)
            glDisable(GL_SCISSOR_TEST);
        if (!blendState)
            glDisable(GL_BLEND);

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
        //QOpenGLWidget::mousePressEvent(event);
        glPoint point(normalizedMousePosX, normalizedMousePosY,
                      color.redF(), color.greenF(), color.blueF());
        if (choosingPlaceForFractal) {
            fractalXY.x = point.x;
            fractalXY.y = point.y;
        }
        else {
            figurePoints.push_back(point);
        }

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

    void changeChoosingFractalPositionState(bool state) {
        if (state)
            choosingPlaceForFractal = true;
        else
            choosingPlaceForFractal = false;
    }
    ;

private:
    QPoint mousePosition;

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

};


class ColorButton : public QPushButton {
public:
    ColorButton(QWidget* parent, MyGLWidget* glWidget) : QPushButton(parent), glWidget(glWidget) {
        colorDialog = new QColorDialog(Qt::white, this);
        connect(colorDialog, &QColorDialog::colorSelected, this, &ColorButton::colorSelectedSlot);

        //connect(colorDialog, &QColorDialog::colorSelected, this, &ColorButton::colorSelectedSlot);
        connect(this, &ColorButton::clicked, [this] {
            colorDialog->exec();

        });
    }



public slots:

    void colorSelectedSlot(const QColor &color) {
        glWidget->changeColor(color);
    };

private:
    QColorDialog* colorDialog;
    MyGLWidget* glWidget;
};




class InterfaceWidget : public QWidget {
public:
    InterfaceWidget(MyGLWidget* glWidget, QWidget* parent = nullptr) : QWidget(parent) {
        this->setMinimumSize(100, 700);

        this->glWidget = glWidget;

        layout = new QVBoxLayout(this);
        layout->setContentsMargins(10, 10, 10, 10);

        primitiveLayout = new QVBoxLayout();

        primitiveBox = new QGroupBox(this);
        primitiveBox->setTitle("НАСТРОЙКИ ПРИМИТИВА");
        primitiveBox->setContentsMargins(10, 15, 10, 15);

        labelBox = new QLabel(this);
        labelBox->setText("Выбор примитива");

        comboBox = new QComboBox(this);
        comboBox->addItem("GL_POINTS");
        comboBox->addItem("GL_LINES");
        comboBox->addItem("GL_LINE_STRIP");
        comboBox->addItem("GL_LINE_LOOP");
        comboBox->addItem("GL_TRIANGLES");
        comboBox->addItem("GL_TRIANGLE_STRIP");
        comboBox->addItem("GL_TRIANGLE_FAN");
        comboBox->addItem("GL_QUADS");
        comboBox->addItem("GL_QUAD_STRIP");
        comboBox->addItem("GL_POLYGON");

        labelLineWidth = new QLabel(this);
        labelLineWidth->setText("Толщина линий");

        sliderLineWidth = new QSlider(this);
        sliderLineWidth->setOrientation(Qt::Horizontal);
        sliderLineWidth->setRange(1, 10);
        sliderLineWidth->setValue(5);

        labelPointSize = new QLabel(this);
        labelPointSize->setText("Толщина точек");

        sliderPointSize = new QSlider(this);
        sliderPointSize->setOrientation(Qt::Horizontal);
        sliderPointSize->setRange(1, 10);
        sliderPointSize->setValue(10);


        colorButton = new ColorButton(this, glWidget);
        colorButton->setText("Выбрать цвет");

        primitiveLayout->addWidget(labelBox);
        primitiveLayout->addWidget(comboBox);
        primitiveLayout->addWidget(labelLineWidth);
        primitiveLayout->addWidget(sliderLineWidth);
        primitiveLayout->addWidget(labelPointSize);
        primitiveLayout->addWidget(sliderPointSize);
        primitiveLayout->addWidget(colorButton);

        primitiveBox->setLayout(primitiveLayout);

        layout->addWidget(primitiveBox);

        layout->addSpacing(20);

        transparencyBox = new QGroupBox(this);
        transparencyBox->setTitle("НАСТРОЙКИ ПРОЗРАЧНОСТИ");

        transparencyLayout = new QVBoxLayout();

        transparencyCheckBox = new QCheckBox(this);
        transparencyCheckBox->setText("Тест прозрачности");

        transparecyComboBox = new QComboBox(this);
        transparecyComboBox->addItem("GL_NEVER");
        transparecyComboBox->addItem("GL_LESS");
        transparecyComboBox->addItem("GL_EQUAL");
        transparecyComboBox->addItem("GL_LEQUAL");
        transparecyComboBox->addItem("GL_GREATER");
        transparecyComboBox->addItem("GL_NOTEQUAL");
        transparecyComboBox->addItem("GL_GEQUAL");
        transparecyComboBox->addItem("GL_ALWAYS");

        sliderTransparency = new QSlider(this);
        sliderTransparency->setOrientation(Qt::Horizontal);
        sliderTransparency->setRange(0, 10);
        sliderTransparency->setSingleStep(1);
        sliderTransparency->setValue(0);

        transparencyLayout->addWidget(transparencyCheckBox);
        transparencyLayout->addWidget(transparecyComboBox);
        transparencyLayout->addWidget(sliderTransparency);

        transparencyBox->setLayout(transparencyLayout);
        layout->addWidget(transparencyBox);

        layout->addSpacing(20);

        scissorBox = new QGroupBox(this);
        scissorBox->setTitle("НАСТРОЙКИ ОТСЕЧЕНИЯ");

        scissorLayout = new QVBoxLayout();

        scissorCheckBox = new QCheckBox(this);
        scissorCheckBox->setText("Тест отсечения");

        labelScissorX = new QLabel(this);
        labelScissorX->setText("Отсечение по X:");

        sliderScissorX = new QSlider(this);
        sliderScissorX->setOrientation(Qt::Horizontal);
        sliderScissorX->setRange(0, glWidget->width() * 2);
        sliderScissorX->setSingleStep(1);
        sliderScissorX->setValue(0);

        labelScissorY = new QLabel(this);
        labelScissorY->setText("Отсечение по Y:");

        sliderScissorY = new QSlider(this);
        sliderScissorY->setOrientation(Qt::Horizontal);
        sliderScissorY->setRange(0, glWidget->height() * 2);
        sliderScissorY->setSingleStep(1);
        sliderScissorY->setValue(0);

        labelScissorW = new QLabel(this);
        labelScissorW->setText("Ширина отсечения:");

        sliderScissorW = new QSlider(this);
        sliderScissorW->setOrientation(Qt::Horizontal);
        sliderScissorW->setRange(0, glWidget->width() * 2);
        sliderScissorW->setSingleStep(1);
        sliderScissorW->setValue(0);

        labelScissorH = new QLabel(this);
        labelScissorH ->setText("Высота отсечения:");

        sliderScissorH = new QSlider(this);
        sliderScissorH->setOrientation(Qt::Horizontal);
        sliderScissorH->setRange(0, glWidget->height()* 2);
        sliderScissorH->setSingleStep(1);
        sliderScissorH->setValue(0);

        scissorLayout->addWidget(scissorCheckBox);
        scissorLayout->addWidget(labelScissorX);
        scissorLayout->addWidget(sliderScissorX);
        scissorLayout->addWidget(labelScissorY);
        scissorLayout->addWidget(sliderScissorY);
        scissorLayout->addWidget(labelScissorW);
        scissorLayout->addWidget(sliderScissorW);
        scissorLayout->addWidget(labelScissorH);
        scissorLayout->addWidget(sliderScissorH);

        scissorBox->setLayout(scissorLayout);
        layout->addWidget(scissorBox);

        layout->addSpacing(20);

        blendBox = new QGroupBox(this);
        blendBox->setTitle("НАСТРОЙКИ СМЕШЕВАНИЯ ЦВЕТОВ");

        blendLayout = new QVBoxLayout();

        blendCheckBox = new QCheckBox(this);
        blendCheckBox->setText("Тест смешевания цветов");


        sfactorLabel = new QLabel(this);
        sfactorLabel->setText("sfactor:");

        sfactorComboBox = new QComboBox(this);
        sfactorComboBox->addItem("GL_ZERO");
        sfactorComboBox->addItem("GL_ONE");
        sfactorComboBox->addItem("GL_DST_COLOR");
        sfactorComboBox->addItem("GL_ONE_MINUS_DST_COLOR");
        sfactorComboBox->addItem("GL_SRC_ALPHA");
        sfactorComboBox->addItem("GL_ONE_MINUS_SRC_ALPHA");
        sfactorComboBox->addItem("GL_DST_ALPHA");
        sfactorComboBox->addItem("GL_ONE_MINUS_DST_ALPHA");
        sfactorComboBox->addItem("GL_SRC_ALPHA_SATURATE");

        dfactorLabel = new QLabel(this);
        dfactorLabel->setText("dfactor:");

        dfactorComboBox = new QComboBox(this);
        dfactorComboBox->addItem("GL_ZERO");
        dfactorComboBox->addItem("GL_ONE");
        dfactorComboBox->addItem("GL_SRC_COLOR");
        dfactorComboBox->addItem("GL_ONE_MINUS_SRC_COLOR");
        dfactorComboBox->addItem("GL_SRC_ALPHA");
        dfactorComboBox->addItem("GL_ONE_MINUS_SRC_ALPHA");
        dfactorComboBox->addItem("GL_DST_ALPHA");
        dfactorComboBox->addItem("GL_ONE_MINUS_DST_ALPHA");

        blendLayout->addWidget(blendCheckBox);
        blendLayout->addWidget(sfactorLabel);
        blendLayout->addWidget(sfactorComboBox);
        blendLayout->addWidget(dfactorLabel);
        blendLayout->addWidget(dfactorComboBox);

        blendBox->setLayout(blendLayout);
        layout->addWidget(blendBox);

        layout->addSpacing(20);

        fractalBox = new QGroupBox(this);
        fractalBox->setTitle("НАСТРОЙКИ ФРАКТАЛА");

        fractalLayout = new QVBoxLayout();

        placingFractalCheckBox = new QCheckBox(this);
        placingFractalCheckBox->setText("Установить расположение фрактала");

        fractalComboBox = new QComboBox(this);
        fractalComboBox->addItem("Геометрические фракталы");
        fractalComboBox->addItem("Фракталы по L-системе");

        fractalGenerations = new QSlider(this);
        fractalGenerations->setOrientation(Qt::Horizontal);
        fractalGenerations->setRange(0, 15);
        fractalGenerations->setSingleStep(1);

        fractalLengthSizeSpinBox = new QDoubleSpinBox(this);
        fractalLengthSizeSpinBox->setRange(0, 1.0);
        fractalLengthSizeSpinBox->setSingleStep(0.1f);
        fractalLengthSizeSpinBox->setValue(0.3f);

        QLabel* fractalLenLabel = new QLabel(this);
        fractalLenLabel->setText("Размер фрактала");

        fractalLayout->addWidget(placingFractalCheckBox);
        fractalLayout->addWidget(fractalComboBox);
        fractalLayout->addWidget(fractalLenLabel);
        fractalLayout->addWidget(fractalGenerations);
        fractalLayout->addWidget(fractalLengthSizeSpinBox);


        fractalBox->setLayout(fractalLayout);
        layout->addWidget(fractalBox);


        connectWidgets();
    }


    void connectWidgets() {
        connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                glWidget, &MyGLWidget::changeFigureTypes);

        connect(sliderLineWidth, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeLineWidth);

        connect(sliderPointSize, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changePointSize);

        connect(transparencyCheckBox, &QCheckBox::checkStateChanged,
                glWidget, &MyGLWidget::changeTransparencyState);

        connect(transparecyComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                glWidget, &MyGLWidget::changeTransparencyType);

        connect(sliderTransparency, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeAlfpha);

        connect(scissorCheckBox, &QCheckBox::checkStateChanged,
                glWidget, &MyGLWidget::changeScissorState);

        connect(sliderScissorX, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeScissorX);

        connect(sliderScissorY, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeScissorY);

        connect(sliderScissorW, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeScissorW);

        connect(sliderScissorH, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeScissorH);

        connect(blendCheckBox, &QCheckBox::checkStateChanged,
                glWidget, &MyGLWidget::changeBlendState);

        connect(sfactorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                glWidget, &MyGLWidget::changeSfactorType);

        connect(dfactorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                glWidget, &MyGLWidget::changeDfactorType);

        connect(fractalComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                glWidget, &MyGLWidget::changeFractalType);

        connect(fractalGenerations, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeFractalGenAmount);
        connect(fractalLengthSizeSpinBox, (&QDoubleSpinBox::valueChanged),
                glWidget, &MyGLWidget::changeFractalLength);
        connect(placingFractalCheckBox, (&QCheckBox::checkStateChanged),
                glWidget, &MyGLWidget::changeChoosingFractalPositionState);
    }

private:
    QVBoxLayout* layout;

    QVBoxLayout* primitiveLayout;
    QLabel* labelBox;
    QComboBox* comboBox;
    QLabel* labelLineWidth;
    QSlider *sliderLineWidth;
    QLabel* labelPointSize;
    QSlider *sliderPointSize;
    ColorButton* colorButton;
    QCheckBox* transparencyCheckBox;
    QComboBox* transparecyComboBox;
    QSlider* sliderTransparency;
    QCheckBox* scissorCheckBox;
    QLabel* labelScissorX;
    QLabel* labelScissorY;
    QLabel* labelScissorW;
    QLabel* labelScissorH;
    QSlider* sliderScissorX;
    QSlider* sliderScissorY;
    QSlider* sliderScissorW;
    QSlider* sliderScissorH;
    QCheckBox* blendCheckBox;
    QLabel* sfactorLabel;
    QLabel* dfactorLabel;
    QComboBox* sfactorComboBox;
    QComboBox* dfactorComboBox;
    QGroupBox* primitiveBox;
    QGroupBox* transparencyBox;
    QVBoxLayout* transparencyLayout;
    QGroupBox* scissorBox;
    QVBoxLayout* scissorLayout;
    QGroupBox* blendBox;
    QVBoxLayout* blendLayout;
    QGroupBox* fractalBox;
    QComboBox* fractalComboBox;
    QSlider* fractalGenerations;
    QVBoxLayout* fractalLayout;
    QDoubleSpinBox* fractalLengthSizeSpinBox;
    QCheckBox* placingFractalCheckBox;

    MyGLWidget* glWidget;
};





class MyMainWindow : public QWidget {
public:
    MyMainWindow() : QWidget() {
        this->setWindowTitle("DemenevKO");
        this->setMinimumSize(925, 725);


        mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(10, 10, 10, 10);

        myGlWidget = new MyGLWidget(this);

        interfaceWidget = new InterfaceWidget(myGlWidget, this);
        scrollInterface = new QScrollArea(this);
        scrollInterface->setWidget(interfaceWidget);

        mainLayout->addWidget(myGlWidget, 20);
        mainLayout->addWidget(scrollInterface, 20);

        this->setLayout(mainLayout);


    }

private:
    QWidget* interface;
    MyGLWidget* myGlWidget;
    InterfaceWidget* interfaceWidget;
    QHBoxLayout* mainLayout;
    QScrollArea* scrollInterface;
};




int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MyMainWindow myMainWindow;

    myMainWindow.show();
    return QApplication::exec();
}
