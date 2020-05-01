// Copyright 2020 José María Castelo Ares

// This file is part of SIRview.

// SIRview is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SIRview is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SIRview.  If not, see <https://www.gnu.org/licenses/>.

#include "widget.h"

Widget::Widget(QWidget *parent): QWidget(parent)
{
    // Init section variables

    timeMin = 0.0;
    timeMax = 1000.0;

    colors[0] = Qt::black;
    colors[1] = Qt::red;
    colors[2] = Qt::green;
    colors[3] = Qt::blue;
    colors[4] = Qt::darkRed;
    colors[5] = Qt::darkGreen;
    colors[6] = Qt::darkBlue;
    colors[7] = Qt::magenta;
    colors[8] = Qt::yellow;
    colors[9] = Qt::cyan;
    colors[10] = Qt::darkMagenta;
    colors[11] = Qt::darkYellow;
    colors[12] = Qt::darkCyan;
    colors[13] = Qt::darkGray;

    // Model selection controls

    QLabel *modelLabel = new QLabel("Model");

    modelComboBox = new QComboBox;
    modelComboBox->addItem("SIR");
    //modelComboBox->addItem("SIRS");
    modelComboBox->setCurrentIndex(0);

    // Section management controls

    QLabel *sectionLabel = new QLabel("Sections");

    addSectionPushButton = new QPushButton("Add");
    removeSectionPushButton = new QPushButton("Remove");
    removeSectionPushButton->setEnabled(false);

    QHBoxLayout *addRemovePushButtonsHBoxLayout = new QHBoxLayout;
    addRemovePushButtonsHBoxLayout->addWidget(addSectionPushButton);
    addRemovePushButtonsHBoxLayout->addWidget(removeSectionPushButton);

    sectionComboBox = new QComboBox;

    QLabel *timeLabel = new QLabel("Time range");

    timeStartDoubleValidator = new QDoubleValidator(timeMin, timeMax, 10);

    timeStartLineEdit = new QLineEdit;
    timeStartLineEdit->setFixedWidth(200);
    timeStartLineEdit->setValidator(timeStartDoubleValidator);

    timeStartSlider = new QSlider(Qt::Horizontal);
    timeStartSlider->setFixedWidth(200);
    timeStartSlider->setRange(0, 1000);
    timeStartSlider->setValue(0);

    timeEndDoubleValidator = new QDoubleValidator(timeMin, timeMax, 10);

    timeEndLineEdit = new QLineEdit;
    timeEndLineEdit->setFixedWidth(200);
    timeEndLineEdit->setValidator(timeEndDoubleValidator);

    timeEndSlider = new QSlider(Qt::Horizontal);
    timeEndSlider->setFixedWidth(200);
    timeEndSlider->setRange(0, 1000);
    timeEndSlider->setValue(1000);

    QLabel *parameterLabel = new QLabel("Parameters");

    parameterVBoxLayout = new QVBoxLayout;

    constructParameterControls(0); // SIR

    // Main controls vertical layout & widget

    mainControlsVBoxLayout = new QVBoxLayout;
    mainControlsVBoxLayout->addWidget(modelLabel);
    mainControlsVBoxLayout->addWidget(modelComboBox);
    mainControlsVBoxLayout->addWidget(sectionLabel);
    mainControlsVBoxLayout->addLayout(addRemovePushButtonsHBoxLayout);
    mainControlsVBoxLayout->addWidget(sectionComboBox);
    mainControlsVBoxLayout->addWidget(timeLabel);
    mainControlsVBoxLayout->addWidget(timeStartLineEdit);
    mainControlsVBoxLayout->addWidget(timeStartSlider);
    mainControlsVBoxLayout->addWidget(timeEndLineEdit);
    mainControlsVBoxLayout->addWidget(timeEndSlider);
    mainControlsVBoxLayout->addWidget(parameterLabel);
    mainControlsVBoxLayout->addLayout(parameterVBoxLayout);

    // Graphs

    graphsTabWidget = new QTabWidget;
    graphsTabWidget->setTabPosition(QTabWidget::North);

    constructPlots(0); // SIR

    // Main grid layout

    QGridLayout *mainGridLayout = new QGridLayout;
    mainGridLayout->addLayout(mainControlsVBoxLayout, 0, 0, Qt::AlignTop);
    mainGridLayout->addWidget(graphsTabWidget, 0, 1);
    mainGridLayout->setColumnStretch(0, 0);
    mainGridLayout->setColumnStretch(1, 1);

    // Sections setup

    addSection();
    setTimeStartMinMax(0);
    setTimeEndMinMax(0);

    // Main widget

    setLayout(mainGridLayout);

    setWindowTitle("SIRview");

    resize(1200, 800);

    // Signals + Slots

    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){ constructParameterControls(index); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){ constructPlots(index); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){ Q_UNUSED(index) deleteSections(); });
    connect(sectionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){ selectSection(index); });
    connect(addSectionPushButton, &QPushButton::clicked, this, &Widget::addSection);
    connect(removeSectionPushButton, &QPushButton::clicked, this, &Widget::removeSection);
    connect(timeStartLineEdit, &QLineEdit::returnPressed, this, &Widget::onTimeStartLineEditReturnPressed);
    connect(timeEndLineEdit, &QLineEdit::returnPressed, this, &Widget::onTimeEndLineEditReturnPressed);
    connect(timeStartSlider, &QSlider::sliderMoved, this, &Widget::onTimeStartSliderValueChanged);
    connect(timeEndSlider, &QSlider::sliderMoved, this, &Widget::onTimeEndSliderValueChanged);
}

Widget::~Widget()
{
}

void Widget::deleteParameterControls()
{
    for (unsigned long i = 0; i < parameterLineEdit.size(); i++)
    {
        parameterLineEdit[i]->disconnect();
    }

    parameterLineEdit.clear();
    parameterDoubleValidator.clear();
    parameterSlider.clear();

    while(!parameterVBoxLayout->isEmpty())
    {
        QWidget *w = parameterVBoxLayout->takeAt(0)->widget();
        delete w;
    }
}

void Widget::constructParameterControls(int index)
{
    deleteParameterControls();

    if (index == 0) // SIR model
    {
        QLabel *R0Label = new QLabel("R0");

        QDoubleValidator *validator = new QDoubleValidator(0.0, 50.0, 10);

        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setFixedWidth(200);
        lineEdit->setValidator(validator);

        QSlider *slider = new QSlider(Qt::Horizontal);
        slider->setFixedWidth(200);
        slider->setRange(0, 1000);
        slider->setValue(0);

        QHBoxLayout *hBoxLayout = new QHBoxLayout;

        hBoxLayout->addWidget(R0Label);
        hBoxLayout->addWidget(lineEdit);

        parameterVBoxLayout->addLayout(hBoxLayout);
        parameterVBoxLayout->addWidget(slider);

        parameterDoubleValidator.push_back(validator);
        parameterLineEdit.push_back(lineEdit);
        parameterSlider.push_back(slider);

        connect(lineEdit, &QLineEdit::returnPressed, [=]{ onParameterLineEditReturnPressed(0); });
        connect(slider, &QSlider::sliderMoved, [=](int value){ onParameterSliderValueChanged(0, value); });
    }
}

void Widget::deletePlots()
{
    graphsTabWidget->clear();
    plots.clear();
}

void Widget::constructPlots(int index)
{
    deletePlots();

    if (index == 0) // SIR model
    {
        // S plot

        plots.push_back(new QCustomPlot(this));

        plots[0]->xAxis->setLabel("t/Tr");
        plots[0]->yAxis->setLabel("S");

        // I plot

        plots.push_back(new QCustomPlot(this));

        plots[1]->xAxis->setLabel("t/Tr");
        plots[1]->yAxis->setLabel("I");

        // R plot

        plots.push_back(new QCustomPlot(this));

        plots[2]->xAxis->setLabel("t/Tr");
        plots[2]->yAxis->setLabel("R");

        for (int i = 0; i < 3; i++)
        {
            plots[i]->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);

            plots[i]->axisRect()->setupFullAxesBox(true);
            plots[i]->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
            plots[i]->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
        }

        // Add graph tabs

        graphsTabWidget->addTab(plots[0], "Susceptible");
        graphsTabWidget->addTab(plots[1], "Infectious");
        graphsTabWidget->addTab(plots[2], "Recovered");
    }
}

void Widget::setPlots()
{
    unsigned long jmax = sections.size() - 1;

    for (unsigned long j = 0; j < jmax; j++)
    {
        sections[j].setAbscissaOrdinate(sections[j + 1].timeStart);
    }

    sections[jmax].setAbscissaOrdinate();

    for (unsigned long i = 0; i < plots.size(); i++)
    {
        int numGraphs = plots[i]->graphCount();

        int k = 0;

        for (int j = 0; j < numGraphs - 2; j += 2)
        {
            plots[i]->graph(j)->setData(sections[k].abscissaLeft, sections[k].ordinateLeft[i], true);
            plots[i]->graph(j + 1)->setData(sections[k].abscissaRight, sections[k].ordinateRight[i], true);
            k++;
        }

        plots[i]->graph(numGraphs - 1)->setData(sections[k].abscissa, sections[k].ordinate[i], true);

        plots[i]->rescaleAxes();
        plots[i]->replot();
    }
}

void Widget::setTimeStartMinMax(int index)
{
    double t0 = timeMin;
    double t1 = sections[index].timeEnd;

    if (index - 1 > 0)
    {
        t0 = sections[index - 1].timeStart;
    }

    if (index + 1 < static_cast<int>(sections.size()))
    {
        t1 = sections[index + 1].timeStart;
    }

    sections[index].timeStartMin = t0;
    timeStartDoubleValidator->setBottom(t0);

    sections[index].timeStartMax = t1;
    timeStartDoubleValidator->setTop(t1);
}

void Widget::setTimeEndMinMax(int index)
{
    double t0 = sections[index].timeStart;
    double t1 = timeMax;

    if (index + 1 < static_cast<int>(sections.size()))
    {
        t0 = sections[index + 1].timeStart;
    }

    sections[index].timeEndMin = t0;
    timeEndDoubleValidator->setBottom(t0);

    sections[index].timeEndMax = t1;
    timeEndDoubleValidator->setTop(t1);
}

void Widget::deleteSections()
{
    sectionComboBox->clear();

    sections.clear();

    addSection();
    setTimeStartMinMax(0);
    setTimeEndMinMax(0);
}

void Widget::selectSection(int index)
{
    setTimeStartMinMax(index);
    setTimeEndMinMax(index);

    timeStartLineEdit->setText(QString("%1").arg(sections[index].timeStart));
    timeEndLineEdit->setText(QString("%1").arg(sections[index].timeEnd));

    for (unsigned long i = 0; i < parameterLineEdit.size(); i++)
    {
        parameterLineEdit[i]->setText(QString("%1").arg(sections[index].parameters[i]));
    }

    int timeStartSliderPosition = sections[index].getIndexTimeStart(timeStartSlider->maximum());
    timeStartSlider->setSliderPosition(timeStartSliderPosition);

    int timeEndSliderPosition = sections[index].getIndexTimeEnd(timeEndSlider->maximum());
    timeEndSlider->setSliderPosition(timeEndSliderPosition);

    for (unsigned long i = 0; i < parameterSlider.size(); i++)
    {
         int parameterSliderPosition = sections[index].getIndexParameter(i, parameterSlider[i]->maximum());
         parameterSlider[i]->setSliderPosition(parameterSliderPosition);
    }

    if (index == 0)
    {
        removeSectionPushButton->setEnabled(false);
    }
    else
    {
        removeSectionPushButton->setEnabled(true);
    }
}

void Widget::addSection()
{
    if (modelComboBox->currentIndex() == 0) // SIR model
    {
        if (sections.empty())
        {
            state_type x0(3);
            x0[1] = 1.0e-7;
            x0[2] = 0.0;
            x0[0] = 1.0 - x0[1] - x0[2];

            std::vector<double> parameters(1);
            parameters[0] = 2.5;

            std::vector<double> parametersMin(1);
            parametersMin[0] = 0.0;

            std::vector<double> parametersMax(1);
            parametersMax[0] = 20.0;

            double t0 = 0.0;
            double t1 = 50.0;

            Section section(x0, parameters, parametersMin, parametersMax, t0, t1);
            sections.push_back(section);
        }
        else
        {
            sections.push_back(sections.back());
        }
    }

    int index = sections.size() - 1;

    sectionComboBox->addItem(QString("Section %1").arg(index + 1));
    sectionComboBox->setCurrentIndex(index);

    timeStartLineEdit->setText(QString("%1").arg(sections.back().timeStart));
    timeEndLineEdit->setText(QString("%1").arg(sections.back().timeEnd));

    for (unsigned long i = 0; i < sections.back().parameters.size(); i++)
    {
        parameterLineEdit[i]->setText(QString("%1").arg(sections.back().parameters[i]));
    }

    if (index > 0)
    {
        for (unsigned long i = 0; i < plots.size(); i++)
        {
            int numGraphs = plots[i]->graphCount();

            plots[i]->removeGraph(numGraphs - 1);
        }

        for (unsigned long i = 0; i < plots.size(); i++)
        {
            plots[i]->addGraph();
            plots[i]->addGraph();

            int numGraphs = plots[i]->graphCount();

            QPen pen = QPen(colors[(index - 1) % 14]);
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(3);

            plots[i]->graph(numGraphs - 2)->setPen(pen);

            pen.setStyle(Qt::DashLine);
            pen.setWidth(1);

            plots[i]->graph(numGraphs - 1)->setPen(pen);
        }

        QPen pen = QPen(colors[index % 14]);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);

        for (unsigned long i = 0; i < plots.size(); i++)
        {
            plots[i]->addGraph();

            int numGraphs = plots[i]->graphCount();

            plots[i]->graph(numGraphs - 1)->setPen(pen);
        }

        setPlots();
    }
    else
    {
        QPen pen = QPen(colors[0]);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);

        for (unsigned long i = 0; i < plots.size(); i++)
        {
            plots[i]->addGraph();
            plots[i]->graph(0)->setPen(pen);
        }

        integrate(false);
    }
}

void Widget::removeSection()
{
    int index = sectionComboBox->currentIndex();
    int jmax = sections.size() - 1;

    sections.erase(sections.begin() + index, sections.begin() + sections.size());

    for (int j = jmax; j >= index; j--)
    {
        sectionComboBox->removeItem(j);
    }

    QPen pen = QPen(colors[(index - 1) % 14]);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);

    for (unsigned long i = 0; i < plots.size(); i++)
    {
        int numGraphs = plots[i]->graphCount();

        plots[i]->removeGraph(numGraphs - 1);

        for (int j = numGraphs - 2; j >= 2 * index - 1; j--)
        {
            plots[i]->removeGraph(j);
        }

        //plots[i]->addGraph();

        numGraphs = plots[i]->graphCount();

        plots[i]->graph(numGraphs - 1)->setPen(pen);

        plots[i]->rescaleAxes();
        plots[i]->replot();
    }

    if (index == 0)
    {
        integrate(false);
    }
    else
    {
        integrate(true);
    }
}

void Widget::onTimeStartLineEditReturnPressed()
{
    int sectionIndex = sectionComboBox->currentIndex();
    sections[sectionIndex].timeStart = timeStartLineEdit->text().toDouble();

    int sliderPosition = sections[sectionIndex].getIndexTimeStart(timeStartSlider->maximum());
    timeStartSlider->setSliderPosition(sliderPosition);

    integrate(true);
}

void Widget::onTimeEndLineEditReturnPressed()
{
    int sectionIndex = sectionComboBox->currentIndex();
    sections[sectionIndex].timeEnd = timeEndLineEdit->text().toDouble();

    int sliderPosition = sections[sectionIndex].getIndexTimeEnd(timeEndSlider->maximum());
    timeEndSlider->setSliderPosition(sliderPosition);

    integrate(false);
}

void Widget::onTimeStartSliderValueChanged(int value)
{
    int sectionIndex = sectionComboBox->currentIndex();

    sections[sectionIndex].setTimeStart(value, timeStartSlider->maximum());

    setTimeEndMinMax(sectionIndex);

    timeStartLineEdit->setText(QString("%1").arg(sections[sectionIndex].timeStart));

    integrate(true);
}

void Widget::onTimeEndSliderValueChanged(int value)
{
    int sectionIndex = sectionComboBox->currentIndex();

    sections[sectionIndex].setTimeEnd(value, timeEndSlider->maximum());

    setTimeStartMinMax(sectionIndex);

    timeEndLineEdit->setText(QString("%1").arg(sections[sectionIndex].timeEnd));

    integrate(false);
}

void Widget::onParameterLineEditReturnPressed(int index)
{
    int sectionIndex = sectionComboBox->currentIndex();
    sections[sectionIndex].parameters[index] = parameterLineEdit[index]->text().toDouble();

    int sliderPosition = sections[sectionIndex].getIndexParameter(index, parameterSlider[index]->maximum());
    parameterSlider[index]->setSliderPosition(sliderPosition);

    integrate(false);
}

void Widget::onParameterSliderValueChanged(int index, int value)
{
    int sectionIndex = sectionComboBox->currentIndex();

    sections[sectionIndex].setParameter(index, value, parameterSlider[index]->maximum());

    parameterLineEdit[index]->setText(QString("%1").arg(sections[sectionIndex].parameters[index]));

    integrate(false);
}

void Widget::integrate(bool interpolation)
{
    using namespace boost::numeric::odeint;

    typedef runge_kutta_dopri5<state_type> error_stepper_type;

    int sectionIndex = sectionComboBox->currentIndex();

    for (int i = sectionIndex; i < static_cast<int>(sections.size()); i++)
    {
        if (interpolation && i > 0)
        {
            sections[i].interpolateX0(sections[i - 1]);
        }

        interpolation = true;

        sections[i].x = sections[i].x0;
        sections[i].steps.clear();
        sections[i].times.clear();

        if (modelComboBox->currentIndex() == 0) // SIR model
        {
            SIR sir(sections[i].parameters[0]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sir, sections[i].x, sections[i].timeStart, sections[i].timeEnd, 0.01, push_back_state_and_time(sections[i].steps, sections[i].times));
        }
    }

    setPlots();
}
