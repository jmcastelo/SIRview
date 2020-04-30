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

    // Model selection controls

    QLabel *modelLabel = new QLabel("Model");

    modelComboBox = new QComboBox;
    modelComboBox->addItem("SIR");
    modelComboBox->addItem("SIRS");
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

    constructParameterControls(0);

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

    // Main grid layout

    QGridLayout *mainGridLayout = new QGridLayout;
    mainGridLayout->addLayout(mainControlsVBoxLayout, 0, 0, Qt::AlignLeft | Qt::AlignTop);
    mainGridLayout->addWidget(graphsTabWidget, 0 , 1);

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

        parameterVBoxLayout->addWidget(R0Label);
        parameterVBoxLayout->addWidget(lineEdit);
        parameterVBoxLayout->addWidget(slider);

        parameterDoubleValidator.push_back(validator);
        parameterLineEdit.push_back(lineEdit);
        parameterSlider.push_back(slider);

        connect(lineEdit, &QLineEdit::returnPressed, [=]{ onParameterLineEditReturnPressed(0); });
        connect(slider, &QSlider::sliderMoved, [=](int value){ onParameterSliderValueChanged(0, value); });
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
            parametersMax[0] = 50.0;

            double t0 = 0.0;
            double t1 = 10.0;

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
}

void Widget::removeSection()
{
    int index = sectionComboBox->currentIndex();

    for (int i = static_cast<int>(sections.size()) - 1; i >= index; i--)
    {
        sectionComboBox->removeItem(i);
    }

    sections.erase(sections.begin() + index, sections.begin() + sections.size());
}

void Widget::onTimeStartLineEditReturnPressed()
{
    int sectionIndex = sectionComboBox->currentIndex();
    sections[sectionIndex].timeStart = timeStartLineEdit->text().toDouble();

    int sliderPosition = sections[sectionIndex].getIndexTimeStart(timeStartSlider->maximum());
    timeStartSlider->setSliderPosition(sliderPosition);
}

void Widget::onTimeEndLineEditReturnPressed()
{
    int sectionIndex = sectionComboBox->currentIndex();
    sections[sectionIndex].timeEnd = timeEndLineEdit->text().toDouble();

    int sliderPosition = sections[sectionIndex].getIndexTimeEnd(timeEndSlider->maximum());
    timeEndSlider->setSliderPosition(sliderPosition);
}

void Widget::onTimeStartSliderValueChanged(int value)
{
    int sectionIndex = sectionComboBox->currentIndex();

    sections[sectionIndex].setTimeStart(value, timeStartSlider->maximum());

    setTimeEndMinMax(sectionIndex);

    timeStartLineEdit->setText(QString("%1").arg(sections[sectionIndex].timeStart));
}

void Widget::onTimeEndSliderValueChanged(int value)
{
    int sectionIndex = sectionComboBox->currentIndex();

    sections[sectionIndex].setTimeEnd(value, timeEndSlider->maximum());

    setTimeStartMinMax(sectionIndex);

    timeEndLineEdit->setText(QString("%1").arg(sections[sectionIndex].timeEnd));
}

void Widget::onParameterLineEditReturnPressed(int index)
{
    int sectionIndex = sectionComboBox->currentIndex();
    sections[sectionIndex].parameters[index] = parameterLineEdit[index]->text().toDouble();

    int sliderPosition = sections[sectionIndex].getIndexParameter(index, parameterSlider[index]->maximum());
    parameterSlider[index]->setSliderPosition(sliderPosition);
}

void Widget::onParameterSliderValueChanged(int index, int value)
{
    int sectionIndex = sectionComboBox->currentIndex();

    sections[sectionIndex].setParameter(index, value, parameterSlider[index]->maximum());

    parameterLineEdit[index]->setText(QString("%1").arg(sections[sectionIndex].parameters[index]));
}
