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

#include "phasespacewidget.h"

PhaseSpaceWidget::PhaseSpaceWidget(QWidget *parent) : QWidget(parent)
{
    // Models

    models.push_back(new PhaseSpaceModel(0, "SIR", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0"}, {0.0}, {20.0}, {2.5}));
    models.push_back(new PhaseSpaceModel(1, "SIRS", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}));
    models.push_back(new PhaseSpaceModel(2, "SIR + Vital dynamics", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}));
    models.push_back(new PhaseSpaceModel(3, "SIRS + Vital dynamics", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1", "P3"}, {0.0, 0.0, 0.0}, {20.0, 5.0, 5.0}, {2.5, 0.1, 0.1}));

    currentModel = models[0];

    // Model selection controls

    QLabel *modelLabel = new QLabel("Model");

    modelComboBox = new QComboBox;

    for (size_t i = 0; i < models.size(); i++)
    {
        modelComboBox->addItem(models[i]->name);
    }

    // Axes controls

    QLabel *xAxisLabel = new QLabel("X-Axis");

    xAxisComboBox = new QComboBox;

    QLabel *yAxisLabel = new QLabel("Y-Axis");

    yAxisComboBox = new QComboBox;

    // Initial conditions grid dimension

    QLabel *icGridDimensionLabel = new QLabel("ICs grid dimension");

    icGridDimensionLineEdit = new QLineEdit;

    QIntValidator *icGridDimensionValidator = new QIntValidator(1, 1000, icGridDimensionLineEdit);

    icGridDimensionLineEdit->setValidator(icGridDimensionValidator);

    // Time max controls

    QLabel *timeEndLabel = new QLabel("Time end (t/Tr)");

    timeEndLineEdit = new QLineEdit;

    QDoubleValidator *timeEndValidator = new QDoubleValidator(0.0, 100000.0, 10, timeEndLineEdit);

    timeEndLineEdit->setValidator(timeEndValidator);

    // Parameters controls

    QLabel *parameterLabel = new QLabel("Parameters");

    parameterVBoxLayout = new QVBoxLayout;

    // Main controls vertical layout

    QVBoxLayout *mainControlsVBoxLayout = new QVBoxLayout;
    mainControlsVBoxLayout->addWidget(modelLabel);
    mainControlsVBoxLayout->addWidget(modelComboBox);
    mainControlsVBoxLayout->addWidget(xAxisLabel);
    mainControlsVBoxLayout->addWidget(xAxisComboBox);
    mainControlsVBoxLayout->addWidget(yAxisLabel);
    mainControlsVBoxLayout->addWidget(yAxisComboBox);
    mainControlsVBoxLayout->addWidget(icGridDimensionLabel);
    mainControlsVBoxLayout->addWidget(icGridDimensionLineEdit);
    mainControlsVBoxLayout->addWidget(timeEndLabel);
    mainControlsVBoxLayout->addWidget(timeEndLineEdit);
    mainControlsVBoxLayout->addWidget(parameterLabel);
    mainControlsVBoxLayout->addLayout(parameterVBoxLayout);

    // Plots stacked layout

    plotsStackedLayout = new QStackedLayout;

    for (size_t i = 0; i < models.size(); i++)
    {
        plotsStackedLayout->addWidget(models[i]->plot);
    }

    // Main grid layout

    QGridLayout *mainGridLayout = new QGridLayout;
    mainGridLayout->addLayout(mainControlsVBoxLayout, 0, 0, Qt::AlignTop);
    mainGridLayout->addLayout(plotsStackedLayout, 0, 1);
    mainGridLayout->setColumnStretch(0, 0);
    mainGridLayout->setColumnStretch(1, 1);

    // Set main layout

    setLayout(mainGridLayout);

    // Signals + Slots

    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ currentModel = models[modelIndex]; });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), plotsStackedLayout, &QStackedLayout::setCurrentIndex);
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) initAxesComboBoxes(); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) updateControls(); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) constructParameterControls(); });
    connect(xAxisComboBox, QOverload<int>::of(&QComboBox::activated), [=](int variableIndex){ if (variableIndex >= 0) updateAxisComboBox(yAxisComboBox, variableIndex); });
    connect(yAxisComboBox, QOverload<int>::of(&QComboBox::activated), [=](int variableIndex){ if (variableIndex >= 0) updateAxisComboBox(xAxisComboBox, variableIndex); });
    connect(xAxisComboBox, QOverload<int>::of(&QComboBox::activated), [=](int variableIndex){ if (variableIndex >= 0) currentModel->setXAxis(variableIndex); });
    connect(yAxisComboBox, QOverload<int>::of(&QComboBox::activated), [=](int variableIndex){ if (variableIndex >= 0) currentModel->setYAxis(variableIndex); });
    connect(icGridDimensionLineEdit, &QLineEdit::returnPressed, [this](){ currentModel->updateInitialConditions(icGridDimensionLineEdit->text().toInt()); });
    connect(timeEndLineEdit, &QLineEdit::returnPressed, [this](){ currentModel->updateTimeEnd(timeEndLineEdit->text().toDouble()); });

    // Init

    modelComboBox->setCurrentIndex(0);
    initAxesComboBoxes();
    updateControls();
    constructParameterControls();
}

PhaseSpaceWidget::~PhaseSpaceWidget()
{
    for (size_t i = 0; i < models.size(); i++)
    {
        delete models[i];
        models[i]= nullptr;
    }

    models.clear();
}

void PhaseSpaceWidget::initAxesComboBoxes()
{
    xAxisComboBox->clear();
    yAxisComboBox->clear();

    for (int i = 0; i < currentModel->dimension; i++)
    {
        xAxisComboBox->addItem(currentModel->variableLongNames[i]->text());
        yAxisComboBox->addItem(currentModel->variableLongNames[i]->text());
    }

    xAxisComboBox->setCurrentIndex(currentModel->xAxis);
    yAxisComboBox->setCurrentIndex(currentModel->yAxis);

    updateAxisComboBox(yAxisComboBox, 0);
    updateAxisComboBox(xAxisComboBox, 1);
}

void PhaseSpaceWidget::updateAxisComboBox(QComboBox *axisComboBox, int variableIndex)
{
    QStandardItemModel *itemModel = qobject_cast<QStandardItemModel*>(axisComboBox->model());

    for (int i = 0; i < currentModel->dimension; i++)
    {
        QStandardItem *item = itemModel->item(i);

        if (i == variableIndex)
        {
            // Disable on the other combo box the item selected on this combo box
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        }
        else
        {
            // Enable the rest
            item->setFlags(item->flags() | Qt::ItemIsEnabled);
        }
    }
}

void PhaseSpaceWidget::updateControls()
{
    icGridDimensionLineEdit->setText(QString::number(currentModel->icGridDimension));
    timeEndLineEdit->setText(QString::number(currentModel->timeEnd));
}

void PhaseSpaceWidget::deleteParameterControls()
{
    for (size_t i = 0; i < parameterLineEdit.size(); i++)
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

void PhaseSpaceWidget::constructParameterControls()
{
    deleteParameterControls();

    for (int i = 0; i < currentModel->numParameters; i++)
    {
        QLineEdit *lineEdit = new QLineEdit;

        CustomValidator *validator = new CustomValidator(currentModel->parameterMin[i], currentModel->parameterMax[i], 10, lineEdit);
        validator->setNotation(QDoubleValidator::StandardNotation);
        validator->setLocale(QLocale::English);

        lineEdit->setValidator(validator);

        lineEdit->setText(QString("%1").arg(currentModel->parameter[i]));

        QHBoxLayout *hBoxLayout = new QHBoxLayout;

        hBoxLayout->addWidget(currentModel->parameterNames[i]);
        hBoxLayout->addWidget(lineEdit);

        QSlider *slider = new QSlider(Qt::Horizontal);
        slider->setFixedWidth(250);
        slider->setRange(0, 1000);

        int sliderPosition = currentModel->getIndexParameter(i, slider->maximum());
        slider->setValue(sliderPosition);

        parameterVBoxLayout->addLayout(hBoxLayout);
        parameterVBoxLayout->addWidget(slider);

        currentModel->parameterNames[i]->show();

        parameterLineEdit.push_back(lineEdit);
        parameterSlider.push_back(slider);

        connect(lineEdit, &QLineEdit::returnPressed, [=]{ onParameterLineEditReturnPressed(i); });
        connect(slider, &QSlider::valueChanged, [=](int value){ onParameterSliderValueChanged(i, value); });
    }
}


void PhaseSpaceWidget::onParameterLineEditReturnPressed(int index)
{
    currentModel->updateParameter(index, parameterLineEdit[index]->text().toDouble());

    int sliderPosition = currentModel->getIndexParameter(index, parameterSlider[index]->maximum());
    parameterSlider[index]->setSliderPosition(sliderPosition);
}

void PhaseSpaceWidget::onParameterSliderValueChanged(int index, int value)
{
    currentModel->setParameter(index, value, parameterSlider[index]->maximum());

    parameterLineEdit[index]->setText(QString::number(currentModel->parameter[index]));
}
