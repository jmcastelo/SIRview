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
    // Models

    models.push_back(new ModelFramework("SIR", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0"}, {0.0}, {20.0}, {2.5}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));
    models.push_back(new ModelFramework("SIRS", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));
    models.push_back(new ModelFramework("SIRA", {"S", "I", "R", "A"}, {"Susceptible", "Infected", "Recovered", "Asymptomatic"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0, 0.0}));
    models.push_back(new ModelFramework("SIR + Vital dynamics", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));
    models.push_back(new ModelFramework("SIRS + Vital dynamics", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1", "P3"}, {0.0, 0.0, 0.0}, {20.0, 5.0, 5.0}, {2.5, 0.1, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));

    // Model selection controls

    QLabel *modelLabel = new QLabel("Model");

    modelComboBox = new QComboBox;

    for (unsigned long i = 0; i < models.size(); i++)
    {
        modelComboBox->addItem(models[i]->name);
    }

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

    timeStartLineEdit = new QLineEdit;

    timeStartDoubleValidator = new CustomValidator(models[0]->timeMin, models[0]->timeMax, 10, timeStartLineEdit);
    timeStartDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    timeStartDoubleValidator->setLocale(QLocale::English);

    timeStartLineEdit->setValidator(timeStartDoubleValidator);

    timeStartSlider = new QSlider(Qt::Horizontal);
    timeStartSlider->setFixedWidth(250);
    timeStartSlider->setRange(0, 10000);
    timeStartSlider->setValue(0);

    timeEndLineEdit = new QLineEdit;

    timeEndDoubleValidator = new CustomValidator(models[0]->timeMin, models[0]->timeMax, 10, timeEndLineEdit);
    timeEndDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    timeEndDoubleValidator->setLocale(QLocale::English);

    timeEndLineEdit->setValidator(timeEndDoubleValidator);

    timeEndSlider = new QSlider(Qt::Horizontal);
    timeEndSlider->setFixedWidth(250);
    timeEndSlider->setRange(0, 10000);
    timeEndSlider->setValue(10000);

    QLabel *initialConditionsLabel = new QLabel("Initial conditions");

    initialConditionsVBoxLayout = new QVBoxLayout;

    //constructInitialConditionsControls(0);

    QLabel *parameterLabel = new QLabel("Parameters");

    parameterVBoxLayout = new QVBoxLayout;

    //constructParameterControls(0);

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
    mainControlsVBoxLayout->addWidget(initialConditionsLabel);
    mainControlsVBoxLayout->addLayout(initialConditionsVBoxLayout);
    mainControlsVBoxLayout->addWidget(parameterLabel);
    mainControlsVBoxLayout->addLayout(parameterVBoxLayout);

    // Graphs

    graphsTabWidget = new QTabWidget;
    graphsTabWidget->setTabPosition(QTabWidget::North);

    // Main grid layout

    QGridLayout *mainGridLayout = new QGridLayout;
    mainGridLayout->addLayout(mainControlsVBoxLayout, 0, 0, Qt::AlignTop);
    mainGridLayout->addWidget(graphsTabWidget, 0, 1);
    mainGridLayout->setColumnStretch(0, 0);
    mainGridLayout->setColumnStretch(1, 1);

    // Signals + Slots

    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ constructParameterControls(modelIndex); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ constructInitialConditionsControls(modelIndex); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ setPlotTabs(modelIndex); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) updateSectionControls(); updateInitialConditionsControls(); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ updateSectionComboBox(modelIndex); });
    connect(sectionComboBox, QOverload<int>::of(&QComboBox::activated), [=](int sectionIndex){ if (sectionIndex >= 0) selectSection(sectionIndex); });
    connect(addSectionPushButton, &QPushButton::clicked, this, &Widget::addSection);
    connect(removeSectionPushButton, &QPushButton::clicked, this, &Widget::removeSection);
    connect(timeStartLineEdit, &QLineEdit::returnPressed, this, &Widget::onTimeStartLineEditReturnPressed);
    connect(timeEndLineEdit, &QLineEdit::returnPressed, this, &Widget::onTimeEndLineEditReturnPressed);
    connect(timeStartSlider, &QSlider::sliderMoved, this, &Widget::onTimeStartSliderValueChanged);
    connect(timeEndSlider, &QSlider::sliderMoved, this, &Widget::onTimeEndSliderValueChanged);

    // Sections setup

    constructInitialConditionsControls(0);
    constructParameterControls(0);
    addInitialSections();

    // Set plot tabs

    setPlotTabs(0);

    // Main widget

    setLayout(mainGridLayout);

    setWindowTitle("SIRview");

    resize(1200, 800);
}

Widget::~Widget()
{
}

void Widget::deleteParameterControls()
{
    for (unsigned long i = 0; i < parameterLineEdit.size(); i++)
    {
        parameterLineEdit[i]->disconnect();
        parameterSlider[i]->disconnect();
    }

    parameterLineEdit.clear();
    parameterSlider.clear();

    QLayoutItem *child;
    while((child = parameterVBoxLayout->takeAt(0)) != 0)
    {
        if (QWidget *pQWidget = child->widget())
        {
            delete pQWidget;
        }
        else if (QLayout *pQLayout = child->layout())
        {
            while(!pQLayout->isEmpty())
            {
                QWidget *pQWidget = pQLayout->takeAt(0)->widget();

                if (QLabel *pQLabel = qobject_cast<QLabel*>(pQWidget))
                {
                    pQLayout->removeWidget(pQLabel);
                    pQLabel->hide();
                }
                else
                {
                    delete pQWidget;
                }
            }
            delete pQLayout;
        }
    }
}

void Widget::constructParameterControls(int modelIndex)
{
    deleteParameterControls();

    for (int i = 0; i < models[modelIndex]->numParameters; i++)
    {
        QLineEdit *lineEdit = new QLineEdit;

        CustomValidator *validator = new CustomValidator(models[modelIndex]->parameterMin[i], models[modelIndex]->parameterMax[i], 10, lineEdit);
        validator->setNotation(QDoubleValidator::StandardNotation);
        validator->setLocale(QLocale::English);

        lineEdit->setValidator(validator);

        models[modelIndex]->parameterNames[i];

        QHBoxLayout *hBoxLayout = new QHBoxLayout;

        hBoxLayout->addWidget(models[modelIndex]->parameterNames[i]);
        hBoxLayout->addWidget(lineEdit);

        QSlider *slider = new QSlider(Qt::Horizontal);
        slider->setFixedWidth(250);
        slider->setRange(0, 10000);
        slider->setValue(0);

        parameterVBoxLayout->addLayout(hBoxLayout);
        parameterVBoxLayout->addWidget(slider);

        models[modelIndex]->parameterNames[i]->show();

        parameterLineEdit.push_back(lineEdit);
        parameterSlider.push_back(slider);

        connect(lineEdit, &QLineEdit::returnPressed, [=]{ onParameterLineEditReturnPressed(i); });
        connect(slider, &QSlider::sliderMoved, [=](int value){ onParameterSliderValueChanged(i, value); });
    }
}

void Widget::deleteInitialConditionsControls()
{
    for (unsigned long i = 0; i < initialConditionsLineEdit.size(); i++)
    {
        initialConditionsLineEdit[i]->disconnect();
    }

    initialConditionsLineEdit.clear();

    QLayoutItem *child;
    while((child = initialConditionsVBoxLayout->takeAt(0)) != 0)
    {
        if (QWidget *pQWidget = child->widget())
        {
            delete pQWidget;
        }
        else if (QLayout *pQLayout = child->layout())
        {
            while(!pQLayout->isEmpty())
            {
                QWidget *pQWidget = pQLayout->takeAt(0)->widget();
                QLabel *pQLabel = qobject_cast<QLabel*>(pQWidget);
                if (pQLabel && (pQLabel != sumInitialConditionsLabel))
                {
                    pQLayout->removeWidget(pQLabel);
                    pQLabel->hide();
                }
                else
                {
                    delete pQWidget;
                }
            }
            delete pQLayout;
        }
    }
}

void Widget::constructInitialConditionsControls(int modelIndex)
{
    deleteInitialConditionsControls();

    for (int i = 0; i < models[modelIndex]->dimension; i++)
    {
        QLineEdit *lineEdit = new QLineEdit;

        CustomValidator *validator = new CustomValidator(0.0, 1.0, 10, lineEdit);
        //validator->setNotation(QDoubleValidator::StandardNotation);
        validator->setNotation(QDoubleValidator::ScientificNotation);
        validator->setLocale(QLocale::English);

        lineEdit->setValidator(validator);

        QHBoxLayout *hBoxLayout = new QHBoxLayout;

        hBoxLayout->addWidget(models[modelIndex]->variableShortNames[i]);
        hBoxLayout->addWidget(lineEdit);

        initialConditionsVBoxLayout->addLayout(hBoxLayout);

        models[modelIndex]->variableShortNames[i]->show();

        initialConditionsLineEdit.push_back(lineEdit);

        connect(lineEdit, &QLineEdit::returnPressed, [=]{ onInitialConditionsLineEditReturnPressed(i); });
    }

    sumInitialConditionsLabel = new QLabel("Sum");

    initialConditionsVBoxLayout->addWidget(sumInitialConditionsLabel);
}

void Widget::setPlotTabs(int modelIndex)
{
    graphsTabWidget->clear();

    graphsTabWidget->addTab(models[modelIndex]->plotsGridWidget, "All");
    graphsTabWidget->addTab(models[modelIndex]->allVariablesPlot, "Joint");

    for (int i = 0; i < models[modelIndex]->dimension; i++)
    {
        graphsTabWidget->addTab(models[modelIndex]->plots[i], models[modelIndex]->variableLongNames[i]->text());
    }
}

void Widget::setTimeStartMinMax(int sectionIndex)
{
    int modelIndex = modelComboBox->currentIndex();

    models[modelIndex]->setTimeStartMinMax(sectionIndex);

    timeStartDoubleValidator->setBottom(models[modelIndex]->sections[sectionIndex].timeStartMin);
    timeStartDoubleValidator->setTop(models[modelIndex]->sections[sectionIndex].timeStartMax);
}

void Widget::setTimeEndMinMax(int sectionIndex)
{
    int modelIndex = modelComboBox->currentIndex();

    models[modelIndex]->setTimeEndMinMax(sectionIndex);

    timeEndDoubleValidator->setBottom(models[modelIndex]->sections[sectionIndex].timeEndMin);
    timeEndDoubleValidator->setTop(models[modelIndex]->sections[sectionIndex].timeEndMax);
}

void Widget::updateSectionComboBox(int modelIndex)
{
    sectionComboBox->clear();

    for (unsigned long i = 0; i < models[modelIndex]->sections.size(); i++)
    {
        sectionComboBox->addItem(QString("Section %1").arg(i + 1));
    }

    sectionComboBox->setCurrentIndex(models[modelIndex]->currentSectionIndex);

    selectSection(models[modelIndex]->currentSectionIndex);
}

void Widget::addInitialSections()
{
    for (unsigned long modelIndex = 0; modelIndex < models.size(); modelIndex++)
    {
        models[modelIndex]->addSection();
        models[modelIndex]->currentSectionIndex = 0;
    }

    sectionComboBox->addItem("Section 1");
    sectionComboBox->setCurrentIndex(0);

    selectSection(0);

    for (unsigned long modelIndex = 0; modelIndex < models.size(); modelIndex++)
    {
        integrate(modelIndex, false);
    }
}

void Widget::selectSection(int sectionIndex)
{
    int modelIndex = modelComboBox->currentIndex();

    models[modelIndex]->setTimeStartMinMax(sectionIndex);
    models[modelIndex]->setTimeEndMinMax(sectionIndex);

    if (sectionIndex == 0)
    {
        removeSectionPushButton->setEnabled(false);

        for (int i = 0; i < models[modelIndex]->dimension; i++)
        {
            initialConditionsLineEdit[i]->setEnabled(true);
        }
    }
    else
    {
        removeSectionPushButton->setEnabled(true);

        for (int i = 0; i < models[modelIndex]->dimension; i++)
        {
            initialConditionsLineEdit[i]->setEnabled(false);
        }
    }

    models[modelIndex]->currentSectionIndex = sectionIndex;

    updateSectionControls();
    updateInitialConditionsControls();
}

void Widget::addSection()
{
    int modelIndex = modelComboBox->currentIndex();

    models[modelIndex]->addSection();

    int sectionIndex = models[modelIndex]->currentSectionIndex;

    sectionComboBox->addItem(QString("Section %1").arg(sectionIndex + 1));
    sectionComboBox->setCurrentIndex(sectionIndex);

    selectSection(sectionIndex);
}

void Widget::removeSection()
{
    int modelIndex = modelComboBox->currentIndex();
    int sectionIndex = sectionComboBox->currentIndex();

    int jmax = models[modelIndex]->sections.size() - 1;

    for (int j = jmax; j >= sectionIndex; j--)
    {
        sectionComboBox->removeItem(j);
    }

    models[modelIndex]->removeSection(sectionIndex);

    sectionIndex = models[modelIndex]->currentSectionIndex;

    selectSection(sectionIndex);

    if (sectionIndex == 0)
    {
        integrate(modelIndex, false);
    }
    else
    {
        integrate(modelIndex, true);
    }
}

void Widget::updateSectionControls()
{
    int modelIndex = modelComboBox->currentIndex();
    int sectionIndex = models[modelIndex]->currentSectionIndex;

    timeStartLineEdit->setText(QString("%1").arg(models[modelIndex]->sections[sectionIndex].timeStart));
    timeEndLineEdit->setText(QString("%1").arg(models[modelIndex]->sections[sectionIndex].timeEnd));

    for (int i = 0; i < models[modelIndex]->numParameters; i++)
    {
        parameterLineEdit[i]->setText(QString("%1").arg(models[modelIndex]->sections[sectionIndex].parameters[i]));
    }

    int timeStartSliderPosition = models[modelIndex]->sections[sectionIndex].getIndexTimeStart(timeStartSlider->maximum());
    timeStartSlider->setSliderPosition(timeStartSliderPosition);

    int timeEndSliderPosition = models[modelIndex]->sections[sectionIndex].getIndexTimeEnd(timeEndSlider->maximum());
    timeEndSlider->setSliderPosition(timeEndSliderPosition);

    for (int i = 0; i < models[modelIndex]->numParameters; i++)
    {
         int parameterSliderPosition = models[modelIndex]->sections[sectionIndex].getIndexParameter(i, parameterSlider[i]->maximum());
         parameterSlider[i]->setSliderPosition(parameterSliderPosition);
    }
}

void Widget::onTimeStartLineEditReturnPressed()
{
    int modelIndex = modelComboBox->currentIndex();
    int sectionIndex = sectionComboBox->currentIndex();

    models[modelIndex]->sections[sectionIndex].timeStart = timeStartLineEdit->text().toDouble();

    int sliderPosition = models[modelIndex]->sections[sectionIndex].getIndexTimeStart(timeStartSlider->maximum());
    timeStartSlider->setSliderPosition(sliderPosition);

    integrate(modelIndex, true);
}

void Widget::onTimeEndLineEditReturnPressed()
{
    int modelIndex = modelComboBox->currentIndex();
    int sectionIndex = sectionComboBox->currentIndex();

    models[modelIndex]->sections[sectionIndex].timeEnd = timeEndLineEdit->text().toDouble();

    int sliderPosition = models[modelIndex]->sections[sectionIndex].getIndexTimeEnd(timeEndSlider->maximum());
    timeEndSlider->setSliderPosition(sliderPosition);

    integrate(modelIndex, false);
}

void Widget::onTimeStartSliderValueChanged(int value)
{
    int modelIndex = modelComboBox->currentIndex();
    int sectionIndex = sectionComboBox->currentIndex();

    models[modelIndex]->sections[sectionIndex].setTimeStart(value, timeStartSlider->maximum());

    models[modelIndex]->setTimeEndMinMax(sectionIndex);

    timeStartLineEdit->setText(QString("%1").arg(models[modelIndex]->sections[sectionIndex].timeStart));

    integrate(modelIndex, true);
}

void Widget::onTimeEndSliderValueChanged(int value)
{
    int modelIndex = modelComboBox->currentIndex();
    int sectionIndex = sectionComboBox->currentIndex();

    models[modelIndex]->sections[sectionIndex].setTimeEnd(value, timeEndSlider->maximum());

    models[modelIndex]->setTimeStartMinMax(sectionIndex);

    timeEndLineEdit->setText(QString("%1").arg(models[modelIndex]->sections[sectionIndex].timeEnd));

    integrate(modelIndex, false);
}

void Widget::onParameterLineEditReturnPressed(int index)
{
    int modelIndex = modelComboBox->currentIndex();
    int sectionIndex = sectionComboBox->currentIndex();

    models[modelIndex]->sections[sectionIndex].parameters[index] = parameterLineEdit[index]->text().toDouble();

    int sliderPosition = models[modelIndex]->sections[sectionIndex].getIndexParameter(index, parameterSlider[index]->maximum());
    parameterSlider[index]->setSliderPosition(sliderPosition);

    integrate(modelIndex, false);
}

void Widget::onParameterSliderValueChanged(int index, int value)
{
    int modelIndex = modelComboBox->currentIndex();
    int sectionIndex = sectionComboBox->currentIndex();

    models[modelIndex]->sections[sectionIndex].setParameter(index, value, parameterSlider[index]->maximum());

    parameterLineEdit[index]->setText(QString("%1").arg(models[modelIndex]->sections[sectionIndex].parameters[index]));

    integrate(modelIndex, false);
}

void Widget::onInitialConditionsLineEditReturnPressed(int index)
{
    int modelIndex = modelComboBox->currentIndex();

    models[modelIndex]->sections[0].x0[index] = initialConditionsLineEdit[index]->text().toDouble();

    updateSumInitialConditionsLabel();

    integrate(modelIndex, false);
}

void Widget::updateInitialConditionsControls()
{
    int modelIndex = modelComboBox->currentIndex();
    int sectionIndex = models[modelIndex]->currentSectionIndex;

    for (int i = 0; i < models[modelIndex]->dimension; i++)
    {
        initialConditionsLineEdit[i]->setText(QString("%1").arg(models[modelIndex]->sections[sectionIndex].x0[i]));
    }

    updateSumInitialConditionsLabel();
}

void Widget::updateSumInitialConditionsLabel()
{
    int modelIndex = modelComboBox->currentIndex();
    int sectionIndex = models[modelIndex]->currentSectionIndex;

    QString text;
    double sum = 0.0;

    for (int i = 0; i < models[modelIndex]->dimension; i++)
    {
        text.append(models[modelIndex]->variableShortNames[i]->text());

        if (i < models[modelIndex]->dimension - 1)
        {
            text.append(" + ");
        }
        else
        {
            text.append(" = ");
        }

        sum += models[modelIndex]->sections[sectionIndex].x0[i];
    }

    text.append(QString("%1").arg(sum));

    sumInitialConditionsLabel->setText(text);
    sumInitialConditionsLabel->repaint();
}

void Widget::integrate(int modelIndex, bool interpolation)
{
    using namespace boost::numeric::odeint;

    typedef runge_kutta_dopri5<state_type> error_stepper_type;

    int sectionIndex = sectionComboBox->currentIndex();

    /*for (int i = sectionIndex; i < static_cast<int>(models[modelIndex]->sections.size()); i++)
    {
        if (interpolation && i > 0)
        {
            models[modelIndex]->sections[i].interpolateX0(models[modelIndex]->sections[i - 1]);
        }

        interpolation = true;

        models[modelIndex]->sections[i].x = models[modelIndex]->sections[i].x0;
        models[modelIndex]->sections[i].steps.clear();
        models[modelIndex]->sections[i].times.clear();

        if (modelIndex == 0) // SIR model
        {
            SIR sir(models[modelIndex]->sections[i].parameters[0]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sir, models[modelIndex]->sections[i].x, models[modelIndex]->sections[i].timeStart, models[modelIndex]->sections[i].timeEnd, 0.01, push_back_state_and_time(models[modelIndex]->sections[i].steps, models[modelIndex]->sections[i].times));
        }
        else if (modelIndex == 1) // SIRS model
        {
            SIRS sirs(models[modelIndex]->sections[i].parameters[0], models[modelIndex]->sections[i].parameters[1]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirs, models[modelIndex]->sections[i].x, models[modelIndex]->sections[i].timeStart, models[modelIndex]->sections[i].timeEnd, 0.01, push_back_state_and_time(models[modelIndex]->sections[i].steps, models[modelIndex]->sections[i].times));
        }
        else if (modelIndex == 2) // SIRA model
        {
            SIRA sira(models[modelIndex]->sections[i].parameters[0], models[modelIndex]->sections[i].parameters[1]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sira, models[modelIndex]->sections[i].x, models[modelIndex]->sections[i].timeStart, models[modelIndex]->sections[i].timeEnd, 0.01, push_back_state_and_time(models[modelIndex]->sections[i].steps, models[modelIndex]->sections[i].times));
        }
        else if (modelIndex == 3) // SIR + Vital dynamics model
        {
            SIRVitalDynamics sirVitalDynamics(models[modelIndex]->sections[i].parameters[0], models[modelIndex]->sections[i].parameters[1]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirVitalDynamics, models[modelIndex]->sections[i].x, models[modelIndex]->sections[i].timeStart, models[modelIndex]->sections[i].timeEnd, 0.01, push_back_state_and_time(models[modelIndex]->sections[i].steps, models[modelIndex]->sections[i].times));
        }
        else if (modelIndex == 4) // SIRS + Vital dynamics model
        {
            SIRSVitalDynamics sirsVitalDynamics(models[modelIndex]->sections[i].parameters[0], models[modelIndex]->sections[i].parameters[1], models[modelIndex]->sections[i].parameters[2]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirsVitalDynamics, models[modelIndex]->sections[i].x, models[modelIndex]->sections[i].timeStart, models[modelIndex]->sections[i].timeEnd, 0.01, push_back_state_and_time(models[modelIndex]->sections[i].steps, models[modelIndex]->sections[i].times));
        }
    }*/
    for (int i = sectionIndex; i < static_cast<int>(models[modelIndex]->sections.size()); i++)
    {
        Section *section = &models[modelIndex]->sections[i];

        if (interpolation && i > 0)
        {
            section->interpolateX0(models[modelIndex]->sections[i - 1]);
        }

        interpolation = true;

        section->x = section->x0;
        section->steps.clear();
        section->times.clear();

        if (modelIndex == 0) // SIR model
        {
            SIR sir(section->parameters[0]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sir, section->x, section->timeStart, section->timeEnd, 0.01, push_back_state_and_time(section->steps, section->times));
        }
        else if (modelIndex == 1) // SIRS model
        {
            SIRS sirs(section->parameters[0], section->parameters[1]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirs, section->x, section->timeStart, section->timeEnd, 0.01, push_back_state_and_time(section->steps, section->times));
        }
        else if (modelIndex == 2) // SIRA model
        {
            SIRA sira(section->parameters[0], section->parameters[1]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sira, section->x, section->timeStart, section->timeEnd, 0.01, push_back_state_and_time(section->steps, section->times));
        }
        else if (modelIndex == 3) // SIR + Vital dynamics model
        {
            SIRVitalDynamics sirVitalDynamics(section->parameters[0], section->parameters[1]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirVitalDynamics, section->x, section->timeStart, section->timeEnd, 0.01, push_back_state_and_time(section->steps, section->times));
        }
        else if (modelIndex == 4) // SIRS + Vital dynamics model
        {
            SIRSVitalDynamics sirsVitalDynamics(section->parameters[0], section->parameters[1], section->parameters[2]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirsVitalDynamics, section->x, section->timeStart, section->timeEnd, 0.01, push_back_state_and_time(section->steps, section->times));
        }
    }

    updateInitialConditionsControls();

    models[modelIndex]->setPlotsData();
}
