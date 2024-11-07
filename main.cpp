#define GL_SILENCE_DEPRECATION

#include "MyGLWidget.h"


class ColorButton : public QPushButton {
public:
    ColorButton(QWidget* parent, MyGLWidget* glWidget) : QPushButton(parent), glWidget(glWidget) {
        colorDialog = new QColorDialog(Qt::white, this);
        connect(colorDialog, &QColorDialog::colorSelected, this, &ColorButton::colorSelectedSlot);
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

        drawingSettingsLayout = new QVBoxLayout();
        drawingSettingsBox = new QGroupBox(this);
        drawingSettingsBox->setTitle("ВЫБРАТЬ РЕЖИМ");
        drawingSettingsBox->setContentsMargins(10, 15, 10, 10);
        drawingTypeComboBox = new QComboBox(this);
        drawingTypeComboBox->addItem("ОТРИСОВКА ПРИМИТИВА");
        drawingTypeComboBox->addItem("ПЕРЕМЕСТИТЬ ФРАКТАЛ");
        drawingTypeComboBox->addItem("ОТРИСОВКА СПЛАЙНА");
        drawingSettingsLayout->addWidget(drawingTypeComboBox);
        drawingSettingsBox->setLayout(drawingSettingsLayout);

        layout->addWidget(drawingSettingsBox);

        layout->addSpacing(20);

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
        transparencyBox->setCheckable(true);
        transparencyBox->setChecked(false);

        transparencyLayout = new QVBoxLayout();

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

        transparencyLayout->addWidget(transparecyComboBox);
        transparencyLayout->addWidget(sliderTransparency);

        transparencyBox->setLayout(transparencyLayout);
        layout->addWidget(transparencyBox);

        layout->addSpacing(20);

        scissorBox = new QGroupBox(this);
        scissorBox->setTitle("НАСТРОЙКИ ОТСЕЧЕНИЯ");
        scissorBox->setCheckable(true);
        scissorBox->setChecked(false);

        scissorLayout = new QVBoxLayout();

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
        blendBox->setCheckable(true);
        blendBox->setChecked(false);

        blendLayout = new QVBoxLayout();

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

        blendLayout->addWidget(sfactorLabel);
        blendLayout->addWidget(sfactorComboBox);
        blendLayout->addWidget(dfactorLabel);
        blendLayout->addWidget(dfactorComboBox);

        blendBox->setLayout(blendLayout);
        layout->addWidget(blendBox);

        layout->addSpacing(20);

        fractalBox = new QGroupBox(this);
        fractalBox->setTitle("НАСТРОЙКИ ФРАКТАЛА");
        fractalBox->setCheckable(true);
        fractalBox->setChecked(false);

        fractalLayout = new QVBoxLayout();


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

        connect(transparencyBox, &QGroupBox::clicked,
                glWidget, &MyGLWidget::changeTransparencyState);

        connect(transparecyComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                glWidget, &MyGLWidget::changeTransparencyType);

        connect(sliderTransparency, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeAlfpha);

        connect(scissorBox, &QGroupBox::clicked,
                glWidget, &MyGLWidget::changeScissorState);

        connect(sliderScissorX, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeScissorX);

        connect(sliderScissorY, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeScissorY);

        connect(sliderScissorW, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeScissorW);

        connect(sliderScissorH, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeScissorH);

        connect(blendBox, &QGroupBox::clicked,
                glWidget, &MyGLWidget::changeBlendState);

        connect(sfactorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                glWidget, &MyGLWidget::changeSfactorType);

        connect(dfactorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                glWidget, &MyGLWidget::changeDfactorType);
        connect(fractalBox, &QGroupBox::clicked,
                glWidget, &MyGLWidget::showFractal);

        connect(fractalComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                glWidget, &MyGLWidget::changeFractalType);

        connect(fractalGenerations, QOverload<int>::of(&QSlider::valueChanged),
                glWidget, &MyGLWidget::changeFractalGenAmount);
        connect(fractalLengthSizeSpinBox, (&QDoubleSpinBox::valueChanged),
                glWidget, &MyGLWidget::changeFractalLength);
        connect(drawingTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                glWidget, &MyGLWidget::changeDrawingType);
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

    QGroupBox* drawingSettingsBox;
    QVBoxLayout* drawingSettingsLayout;
    QComboBox* drawingTypeComboBox;

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
