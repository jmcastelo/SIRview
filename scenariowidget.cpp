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

#include "scenariowidget.h"

ScenarioWidget::ScenarioWidget(QWidget *parent): QWidget(parent)
{
    // Models

    models.push_back(new ScenarioGenericModel(0, "SIR", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0"}, {0.0}, {20.0}, {2.5}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));
    models.push_back(new ScenarioGenericModel(1, "SIRS", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));
    models.push_back(new ScenarioGenericModel(2, "SEIR", {"S", "E", "I", "R"}, {"Susceptible", "Exposed", "Infected", "Recovered"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}, {1.0 - 1.0e-7, 0.0, 1.0e-7, 0.0}));
    models.push_back(new ScenarioGenericModel(3, "SEIRS", {"S", "E", "I", "R"}, {"Susceptible", "Exposed", "Infected", "Recovered"}, {"P0", "P1", "P2"}, {0.0, 0.0, 0.0}, {20.0, 5.0, 5.0}, {2.5, 0.1, 0.1}, {1.0 - 1.0e-7, 0.0, 1.0e-7, 0.0}));
    models.push_back(new ScenarioGenericModel(4, "SIRA", {"S", "I", "R", "A"}, {"Susceptible", "Infected", "Recovered", "Asymptomatic"}, {"P0", "P1", "P2"}, {0.0, 0.0, 0.0}, {20.0, 5.0, 5.0}, {2.5, 0.1, 0.1}, {1.0 - 2.0e-7, 1.0e-7, 0.0, 1.0e-7}));
    models.push_back(new ScenarioGenericModel(5, "SIR + Vital dynamics", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));
    models.push_back(new ScenarioGenericModel(6, "SIRS + Vital dynamics", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1", "P2"}, {0.0, 0.0, 0.0}, {20.0, 5.0, 5.0}, {2.5, 0.1, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));
    models.push_back(new ScenarioGenericModel(7, "SEIR + Vital dynamics", {"S", "E", "I", "R"}, {"Susceptible", "Exposed", "Infected", "Recovered"}, {"P0", "P1", "P2"}, {0.0, 0.0, 0.0}, {20.0, 5.0, 5.0}, {2.5, 0.1, 0.1}, {1.0 - 1.0e-7, 0.0, 1.0e-7, 0.0}));
    models.push_back(new ScenarioGenericModel(8, "SEIRS + Vital dynamics", {"S", "E", "I", "R"}, {"Susceptible", "Exposed", "Infected", "Recovered"}, {"P0", "P1", "P2", "P3"}, {0.0, 0.0, 0.0, 0.0}, {20.0, 5.0, 5.0, 5.0}, {2.5, 0.1, 0.1, 0.1}, {1.0 - 1.0e-7, 0.0, 1.0e-7, 0.0}));

    currentModel = models[0];

    // Model selection controls

    QLabel *modelLabel = new QLabel("Model");

    modelComboBox = new QComboBox;

    for (size_t i = 0; i < models.size(); i++)
    {
        modelComboBox->addItem(models[i]->name);
    }

    modelComboBox->setCurrentIndex(0);

    // Init snapshots vector

    std::list<Snapshot*> snapshotList;

    snapshots.reserve(5);

    for (size_t i = 0; i < models.size(); i++)
    {
        snapshots.push_back(snapshotList);
    }

    // Snapshot management controls

    QLabel *snapshotLabel = new QLabel("Snapshots");

    takeSnapshotPushButton = new QPushButton("Take");
    removeSnapshotPushButton = new QPushButton("Remove");
    removeSnapshotPushButton->setEnabled(false);

    QHBoxLayout *takeRemovePushButtonsHBoxLayout = new QHBoxLayout;
    takeRemovePushButtonsHBoxLayout->addWidget(takeSnapshotPushButton);
    takeRemovePushButtonsHBoxLayout->addWidget(removeSnapshotPushButton);

    snapshotComboBox = new QComboBox;

    // Scenario management controls

    QLabel *scenarioLabel = new QLabel("Scenarios");

    addScenarioPushButton = new QPushButton("Add");
    removeScenarioPushButton = new QPushButton("Remove");
    removeScenarioPushButton->setEnabled(false);

    QHBoxLayout *addRemovePushButtonsHBoxLayout = new QHBoxLayout;
    addRemovePushButtonsHBoxLayout->addWidget(addScenarioPushButton);
    addRemovePushButtonsHBoxLayout->addWidget(removeScenarioPushButton);

    scenarioComboBox = new QComboBox;

    QLabel *timeLabel = new QLabel("Time range");

    shiftTimeRangesCheckbox = new QCheckBox("Shift time ranges");
    shiftTimeRangesCheckbox->setChecked(false);

    timeStartLineEdit = new QLineEdit;

    //timeStartDoubleValidator = new CustomValidator(models[0]->timeMin, models[0]->timeMax, 10, timeStartLineEdit);
    timeStartDoubleValidator = new QDoubleValidator(0.0, 1.0, 10, timeStartLineEdit);
    timeStartDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    timeStartDoubleValidator->setLocale(QLocale::English);

    timeStartLineEdit->setValidator(timeStartDoubleValidator);

    timeStartSlider = new QSlider(Qt::Horizontal);
    timeStartSlider->setFixedWidth(250);
    timeStartSlider->setRange(0, 1000);

    timeEndLineEdit = new QLineEdit;

    //timeEndDoubleValidator = new CustomValidator(models[0]->timeMin, models[0]->timeMax, 10, timeEndLineEdit);
    timeEndDoubleValidator = new QDoubleValidator(0.0, 1.0, 10, timeEndLineEdit);
    timeEndDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    timeEndDoubleValidator->setLocale(QLocale::English);

    timeEndLineEdit->setValidator(timeEndDoubleValidator);

    timeEndSlider = new QSlider(Qt::Horizontal);
    timeEndSlider->setFixedWidth(250);
    timeEndSlider->setRange(0, 1000);

    QLabel *initialConditionsLabel = new QLabel("Initial conditions");

    initialConditionsVBoxLayout = new QVBoxLayout;

    QLabel *parameterLabel = new QLabel("Parameters");

    parameterVBoxLayout = new QVBoxLayout;

    // Main controls vertical layout

    QVBoxLayout *mainControlsVBoxLayout = new QVBoxLayout;
    mainControlsVBoxLayout->addWidget(modelLabel);
    mainControlsVBoxLayout->addWidget(modelComboBox);
    mainControlsVBoxLayout->addWidget(snapshotLabel);
    mainControlsVBoxLayout->addLayout(takeRemovePushButtonsHBoxLayout);
    mainControlsVBoxLayout->addWidget(snapshotComboBox);
    mainControlsVBoxLayout->addWidget(scenarioLabel);
    mainControlsVBoxLayout->addLayout(addRemovePushButtonsHBoxLayout);
    mainControlsVBoxLayout->addWidget(scenarioComboBox);
    mainControlsVBoxLayout->addWidget(timeLabel);
    mainControlsVBoxLayout->addWidget(shiftTimeRangesCheckbox);
    mainControlsVBoxLayout->addWidget(timeStartLineEdit);
    mainControlsVBoxLayout->addWidget(timeStartSlider);
    mainControlsVBoxLayout->addWidget(timeEndLineEdit);
    mainControlsVBoxLayout->addWidget(timeEndSlider);
    mainControlsVBoxLayout->addWidget(initialConditionsLabel);
    mainControlsVBoxLayout->addLayout(initialConditionsVBoxLayout);
    mainControlsVBoxLayout->addWidget(parameterLabel);
    mainControlsVBoxLayout->addLayout(parameterVBoxLayout);

    // Plots

    plotsTabWidget = new QTabWidget;
    plotsTabWidget->setTabPosition(QTabWidget::North);

    // Main grid layout

    QGridLayout *mainGridLayout = new QGridLayout;
    mainGridLayout->addLayout(mainControlsVBoxLayout, 0, 0, Qt::AlignTop);
    mainGridLayout->addWidget(plotsTabWidget, 0, 1);
    mainGridLayout->setColumnStretch(0, 0);
    mainGridLayout->setColumnStretch(1, 1);

    // Signals + Slots

    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ currentModel = models[modelIndex]; });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) constructInitialConditionsControls(); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) constructParameterControls(); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) setPlotTabs(); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) updateScenarioComboBox(); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) updateScenarioControls(); updateInitialConditionsControls(); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ updateSnapshotWidgets(modelIndex); });
    connect(takeSnapshotPushButton, &QPushButton::clicked, this, &ScenarioWidget::takeSnapshot);
    connect(removeSnapshotPushButton, &QPushButton::clicked, this, &ScenarioWidget::removeSnapshot);
    connect(snapshotComboBox, QOverload<int>::of(&QComboBox::activated), [=](int snapshotIndex){ selectSnapshot(snapshotIndex);});
    connect(scenarioComboBox, QOverload<int>::of(&QComboBox::activated), [=](int scenarioIndex){ selectScenario(scenarioIndex); });
    connect(addScenarioPushButton, &QPushButton::clicked, this, &ScenarioWidget::addScenario);
    connect(removeScenarioPushButton, &QPushButton::clicked, this, &ScenarioWidget::removeScenario);
    connect(shiftTimeRangesCheckbox, &QCheckBox::toggled, [this](bool shift){ updateTimeRangeMinMax(shift); updateTimeStartControls(); updateTimeEndControls(); updateValidators(); });
    connect(timeStartLineEdit, &QLineEdit::returnPressed, this, &ScenarioWidget::onTimeStartLineEditReturnPressed);
    connect(timeEndLineEdit, &QLineEdit::returnPressed, this, &ScenarioWidget::onTimeEndLineEditReturnPressed);
    connect(timeStartSlider, &QSlider::valueChanged, this, &ScenarioWidget::onTimeStartSliderValueChanged);
    connect(timeEndSlider, &QSlider::valueChanged, this, &ScenarioWidget::onTimeEndSliderValueChanged);

    // Scenarios setup

    constructInitialConditionsControls();
    constructParameterControls();
    addInitialScenarios();

    // Set plot tabs

    setPlotTabs();

    // Set main layout

    setLayout(mainGridLayout);
}

ScenarioWidget::~ScenarioWidget()
{
    for (size_t i = 0; i < models.size(); i++)
    {
        delete models[i];
        models[i]= nullptr;
    }

    models.clear();
}

void ScenarioWidget::deleteParameterControls()
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

void ScenarioWidget::constructParameterControls()
{
    deleteParameterControls();

    for (int i = 0; i < currentModel->numParameters; i++)
    {
        QLineEdit *lineEdit = new QLineEdit;

        CustomValidator *validator = new CustomValidator(currentModel->parameterMin[i], currentModel->parameterMax[i], 10, lineEdit);
        validator->setNotation(QDoubleValidator::StandardNotation);
        validator->setLocale(QLocale::English);

        lineEdit->setValidator(validator);

        QHBoxLayout *hBoxLayout = new QHBoxLayout;

        hBoxLayout->addWidget(currentModel->parameterNames[i]);
        hBoxLayout->addWidget(lineEdit);

        QSlider *slider = new QSlider(Qt::Horizontal);
        slider->setFixedWidth(250);
        slider->setRange(0, 10000);
        slider->setValue(0);

        parameterVBoxLayout->addLayout(hBoxLayout);
        parameterVBoxLayout->addWidget(slider);

        currentModel->parameterNames[i]->show();

        parameterLineEdit.push_back(lineEdit);
        parameterSlider.push_back(slider);

        connect(lineEdit, &QLineEdit::returnPressed, [=]{ onParameterLineEditReturnPressed(i); });
        connect(slider, &QSlider::valueChanged, [=](int value){ onParameterSliderValueChanged(i, value); });
    }
}

void ScenarioWidget::deleteInitialConditionsControls()
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

void ScenarioWidget::constructInitialConditionsControls()
{
    deleteInitialConditionsControls();

    for (int i = 0; i < currentModel->dimension; i++)
    {
        QLineEdit *lineEdit = new QLineEdit;

        CustomValidator *validator = new CustomValidator(0.0, 1.0, 10, lineEdit);
        //validator->setNotation(QDoubleValidator::StandardNotation);
        validator->setNotation(QDoubleValidator::ScientificNotation);
        validator->setLocale(QLocale::English);

        lineEdit->setValidator(validator);

        QHBoxLayout *hBoxLayout = new QHBoxLayout;

        hBoxLayout->addWidget(currentModel->variableShortNames[i]);
        hBoxLayout->addWidget(lineEdit);

        initialConditionsVBoxLayout->addLayout(hBoxLayout);

        currentModel->variableShortNames[i]->show();

        initialConditionsLineEdit.push_back(lineEdit);

        connect(lineEdit, &QLineEdit::returnPressed, [=]{ onInitialConditionsLineEditReturnPressed(i); });
    }

    sumInitialConditionsLabel = new QLabel("Sum");

    initialConditionsVBoxLayout->addWidget(sumInitialConditionsLabel);
}

void ScenarioWidget::setPlotTabs()
{
    plotsTabWidget->clear();

    plotsTabWidget->addTab(currentModel->plotsGridWidget, "All");
    plotsTabWidget->addTab(currentModel->allVariablesPlot, "Joint");

    for (int i = 0; i < currentModel->dimension; i++)
    {
        plotsTabWidget->addTab(currentModel->plots[i], currentModel->variableLongNames[i]->text());
    }
}

void ScenarioWidget::updateScenarioComboBox()
{
    scenarioComboBox->clear();

    for (size_t i = 0; i < currentModel->scenarios.size(); i++)
    {
        scenarioComboBox->addItem(QString("Scenario %1").arg(i + 1));
    }

    scenarioComboBox->setCurrentIndex(currentModel->currentScenarioIndex);

    selectScenario(currentModel->currentScenarioIndex);
}

void ScenarioWidget::addInitialScenarios()
{
    for (size_t modelIndex = 0; modelIndex < models.size(); modelIndex++)
    {
        models[modelIndex]->addScenario();
        models[modelIndex]->currentScenarioIndex = 0;
    }

    scenarioComboBox->addItem("Scenario 1");
    scenarioComboBox->setCurrentIndex(0);

    selectScenario(0);

    for (size_t modelIndex = 0; modelIndex < models.size(); modelIndex++)
    {
        ScenarioModel *model = models[modelIndex];
        integrate(model, false);
    }
}

void ScenarioWidget::selectScenario(int scenarioIndex)
{
    if (scenarioIndex == 0)
    {
        removeScenarioPushButton->setEnabled(false);

        for (int i = 0; i < currentModel->dimension; i++)
        {
            initialConditionsLineEdit[i]->setEnabled(true);
        }
    }
    else
    {
        removeScenarioPushButton->setEnabled(true);

        for (int i = 0; i < currentModel->dimension; i++)
        {
            initialConditionsLineEdit[i]->setEnabled(false);
        }
    }

    currentModel->currentScenarioIndex = scenarioIndex;

    updateScenarioControls();
    updateInitialConditionsControls();
}

void ScenarioWidget::addScenario()
{
    currentModel->addScenario();

    int scenarioIndex = currentModel->currentScenarioIndex;

    scenarioComboBox->addItem(QString("Scenario %1").arg(scenarioIndex + 1));
    scenarioComboBox->setCurrentIndex(scenarioIndex);

    selectScenario(scenarioIndex);
}

void ScenarioWidget::removeScenario()
{
    int scenarioIndex = scenarioComboBox->currentIndex();
    int jmax = currentModel->scenarios.size() - 1;

    for (int j = jmax; j >= scenarioIndex; j--)
    {
        scenarioComboBox->removeItem(j);
    }

    currentModel->removeScenario(scenarioIndex);

    scenarioIndex = currentModel->currentScenarioIndex;

    selectScenario(scenarioIndex);

    if (scenarioIndex == 0)
    {
        integrate(currentModel, false);
    }
    else
    {
        integrate(currentModel, true);
    }
}

void ScenarioWidget::updateScenarioControls()
{
    updateTimeStartControls();
    updateTimeEndControls();
    updateValidators();
    updateParameterControls();
}

void ScenarioWidget::updateTimeStartControls()
{
    int scenarioIndex = currentModel->currentScenarioIndex;

    timeStartLineEdit->setText(QString::number(currentModel->scenarios[scenarioIndex].timeStart));

    bool startSliderState = timeStartSlider->blockSignals(true);

    int timeStartSliderPosition = currentModel->scenarios[scenarioIndex].getIndexTimeStart(timeStartSlider->maximum());
    timeStartSlider->setValue(timeStartSliderPosition);

    timeStartSlider->blockSignals(startSliderState);
}

void ScenarioWidget::updateTimeEndControls()
{
    int scenarioIndex = currentModel->currentScenarioIndex;

    timeEndLineEdit->setText(QString::number(currentModel->scenarios[scenarioIndex].timeEnd));

    bool endSliderState = timeEndSlider->blockSignals(true);

    int timeEndSliderPosition = currentModel->scenarios[scenarioIndex].getIndexTimeEnd(timeEndSlider->maximum());
    timeEndSlider->setValue(timeEndSliderPosition);

    timeEndSlider->blockSignals(endSliderState);
}

void ScenarioWidget::updateValidators()
{
    int scenarioIndex = currentModel->currentScenarioIndex;

    timeStartDoubleValidator->setBottom(currentModel->scenarios[scenarioIndex].timeStartMin);
    timeStartDoubleValidator->setTop(currentModel->scenarios[scenarioIndex].timeStartMax);

    timeEndDoubleValidator->setBottom(currentModel->scenarios[scenarioIndex].timeEndMin);
    timeEndDoubleValidator->setTop(currentModel->scenarios[scenarioIndex].timeEndMax);
}

void ScenarioWidget::updateParameterControls()
{
    int scenarioIndex = currentModel->currentScenarioIndex;

    for (int i = 0; i < currentModel->numParameters; i++)
    {
        parameterLineEdit[i]->setText(QString::number(currentModel->scenarios[scenarioIndex].parameters[i]));
    }

    for (int i = 0; i < currentModel->numParameters; i++)
    {
         int parameterSliderPosition = currentModel->scenarios[scenarioIndex].getIndexParameter(i, parameterSlider[i]->maximum());
         parameterSlider[i]->setValue(parameterSliderPosition);
    }
}

void ScenarioWidget::updateTimeRangeMinMax(bool shift)
{
    if (!shift)
    {
        currentModel->updateTimeRangeMinMax();
    }
}

void ScenarioWidget::onTimeStartLineEditReturnPressed()
{
    int scenarioIndex = scenarioComboBox->currentIndex();

    if (shiftTimeRangesCheckbox->isChecked())
    {
        double timeStart0 = currentModel->scenarios[scenarioIndex].timeStart;
        double timeStart1 = timeStartLineEdit->text().toDouble();

        currentModel->scenarios[scenarioIndex].timeStart = timeStart1;
        currentModel->shiftTimeRanges(scenarioIndex, timeStart1 - timeStart0);

        updateTimeEndControls();
    }
    else
    {
        currentModel->scenarios[scenarioIndex].timeStart = timeStartLineEdit->text().toDouble();
        currentModel->onTimeStartChanged(scenarioIndex);
    }

    updateTimeStartControls();
    updateValidators();

    integrate(currentModel, true);
}

void ScenarioWidget::onTimeEndLineEditReturnPressed()
{
    int scenarioIndex = scenarioComboBox->currentIndex();

    currentModel->scenarios[scenarioIndex].timeEnd = timeEndLineEdit->text().toDouble();
    currentModel->onTimeEndChanged(scenarioIndex);

    updateTimeEndControls();
    updateValidators();

    integrate(currentModel, false);
}

void ScenarioWidget::onTimeStartSliderValueChanged(int value)
{
    int scenarioIndex = scenarioComboBox->currentIndex();

    if (shiftTimeRangesCheckbox->isChecked())
    {
        double timeStart0 = currentModel->scenarios[scenarioIndex].timeStart;
        currentModel->scenarios[scenarioIndex].setTimeStart(value, timeStartSlider->maximum());

        double timeStart1 = currentModel->scenarios[scenarioIndex].timeStart;
        currentModel->shiftTimeRanges(scenarioIndex, timeStart1 - timeStart0);

        updateTimeEndControls();
    }
    else
    {
        currentModel->scenarios[scenarioIndex].setTimeStart(value, timeStartSlider->maximum());
        currentModel->onTimeStartChanged(scenarioIndex);
    }

    timeStartLineEdit->setText(QString::number(currentModel->scenarios[scenarioIndex].timeStart));

    updateValidators();

    integrate(currentModel, true);
}

void ScenarioWidget::onTimeEndSliderValueChanged(int value)
{
    int scenarioIndex = scenarioComboBox->currentIndex();

    currentModel->scenarios[scenarioIndex].setTimeEnd(value, timeEndSlider->maximum());
    currentModel->onTimeEndChanged(scenarioIndex);

    timeEndLineEdit->setText(QString::number(currentModel->scenarios[scenarioIndex].timeEnd));

    if (value == timeEndSlider->maximum())
    {
        bool endSliderState = timeEndSlider->blockSignals(true);
        int sliderPosition = currentModel->scenarios[scenarioIndex].getIndexTimeEnd(timeEndSlider->maximum());
        timeEndSlider->setValue(sliderPosition);
        timeEndSlider->blockSignals(endSliderState);
    }

    bool startSliderState = timeStartSlider->blockSignals(true);
    int timeStartSliderPosition = currentModel->scenarios[scenarioIndex].getIndexTimeStart(timeStartSlider->maximum());
    timeStartSlider->setValue(timeStartSliderPosition);
    timeStartSlider->blockSignals(startSliderState);

    updateValidators();

    integrate(currentModel, false);
}

void ScenarioWidget::onParameterLineEditReturnPressed(int index)
{
    int scenarioIndex = scenarioComboBox->currentIndex();

    currentModel->scenarios[scenarioIndex].parameters[index] = parameterLineEdit[index]->text().toDouble();

    int sliderPosition = currentModel->scenarios[scenarioIndex].getIndexParameter(index, parameterSlider[index]->maximum());
    parameterSlider[index]->setSliderPosition(sliderPosition);

    integrate(currentModel, false);
}

void ScenarioWidget::onParameterSliderValueChanged(int index, int value)
{
    int scenarioIndex = scenarioComboBox->currentIndex();

    currentModel->scenarios[scenarioIndex].setParameter(index, value, parameterSlider[index]->maximum());

    parameterLineEdit[index]->setText(QString::number(currentModel->scenarios[scenarioIndex].parameters[index]));

    integrate(currentModel, false);
}

void ScenarioWidget::onInitialConditionsLineEditReturnPressed(int index)
{
    currentModel->scenarios[0].x0[index] = initialConditionsLineEdit[index]->text().toDouble();

    updateSumInitialConditionsLabel();

    integrate(currentModel, false);
}

void ScenarioWidget::updateInitialConditionsControls()
{
    int scenarioIndex = currentModel->currentScenarioIndex;

    for (int i = 0; i < currentModel->dimension; i++)
    {
        initialConditionsLineEdit[i]->setText(QString::number(currentModel->scenarios[scenarioIndex].x0[i]));
    }

    updateSumInitialConditionsLabel();
}

void ScenarioWidget::updateSumInitialConditionsLabel()
{
    int scenarioIndex = currentModel->currentScenarioIndex;

    QString text;
    double sum = 0.0;

    for (int i = 0; i < currentModel->dimension; i++)
    {
        text.append(currentModel->variableShortNames[i]->text());

        if (i < currentModel->dimension - 1)
        {
            text.append(" + ");
        }
        else
        {
            text.append(" = ");
        }

        sum += currentModel->scenarios[scenarioIndex].x0[i];
    }

    text.append(QString::number(sum));

    sumInitialConditionsLabel->setText(text);
    sumInitialConditionsLabel->repaint();
}

void ScenarioWidget::takeSnapshot()
{
    int modelIndex = modelComboBox->currentIndex();

    snapshots[modelIndex].push_back(new Snapshot(models[modelIndex]));

    int snapshotIndex = snapshots[modelIndex].size() - 1;

    snapshotComboBox->addItem(QString("Snapshot %1").arg(snapshotIndex + 1));
    snapshotComboBox->setCurrentIndex(snapshotIndex);

    updateSnapshotTab(snapshotIndex);

    removeSnapshotPushButton->setEnabled(true);

    currentModel->currentSnapshotIndex = snapshotIndex;
}

void ScenarioWidget::selectSnapshot(int snapshotIndex)
{
    updateSnapshotTab(snapshotIndex);

    int snapshotTabIndex = 2 + currentModel->dimension;

    plotsTabWidget->setCurrentIndex(snapshotTabIndex);

    currentModel->currentSnapshotIndex = snapshotIndex;
}

void ScenarioWidget::updateSnapshotTab(int snapshotIndex)
{
    int modelIndex = modelComboBox->currentIndex();
    int snapshotTabIndex = 2 + currentModel->dimension;
    int plotsTabWidgetIndex = plotsTabWidget->currentIndex();

    if (snapshots[modelIndex].size() > 1)
    {
        plotsTabWidget->removeTab(snapshotTabIndex);
    }

    auto it = std::next(snapshots[modelIndex].begin(), snapshotIndex);
    plotsTabWidget->addTab((*it)->plotsGridWidget, "Snapshot");

    plotsTabWidget->setCurrentIndex(plotsTabWidgetIndex);
}

void ScenarioWidget::removeSnapshot()
{
    int modelIndex = modelComboBox->currentIndex();
    int snapshotTabIndex = 2 + currentModel->dimension;
    int plotsTabWidgetIndex = plotsTabWidget->currentIndex();
    int snapshotIndex = snapshotComboBox->currentIndex();

    plotsTabWidget->removeTab(snapshotTabIndex);

    std::list<Snapshot*>::iterator it = snapshots[modelIndex].begin();
    std::advance(it, snapshotIndex);
    snapshots[modelIndex].erase(it);

    snapshotComboBox->removeItem(snapshotIndex);

    for (int i = 0; i < snapshotComboBox->count(); i++)
    {
        snapshotComboBox->setItemText(i, QString("Snapshot %1").arg(i + 1));
    }

    if (snapshots[modelIndex].size() > 0)
    {
        auto it = std::next(snapshots[modelIndex].begin(), snapshotComboBox->currentIndex());
        plotsTabWidget->addTab((*it)->plotsGridWidget, "Snapshot");

        plotsTabWidget->setCurrentIndex(plotsTabWidgetIndex);
    }
    else
    {
        removeSnapshotPushButton->setEnabled(false);

        if (plotsTabWidgetIndex == snapshotTabIndex)
        {
            plotsTabWidget->setCurrentIndex(0);
        }
        else
        {
            plotsTabWidget->setCurrentIndex(plotsTabWidgetIndex);
        }
    }

    currentModel->currentSnapshotIndex = snapshotIndex - 1;
}

void ScenarioWidget::updateSnapshotWidgets(int modelIndex)
{
    snapshotComboBox->clear();

    for (size_t i = 0; i < snapshots[modelIndex].size(); i++)
    {
        snapshotComboBox->addItem(QString("Snapshot %1").arg(i + 1));
    }

    if (snapshots[modelIndex].size() > 0)
    {
        int snapshotIndex = models[modelIndex]->currentSnapshotIndex;

        snapshotComboBox->setCurrentIndex(snapshotIndex);

        updateSnapshotTab(snapshotIndex);

        removeSnapshotPushButton->setEnabled(true);
    }
    else
    {
        int snapshotTabIndex = 2 + models[modelIndex]->dimension;

        QWidget *widget = plotsTabWidget->widget(snapshotTabIndex);

        if (widget != nullptr)
        {
            plotsTabWidget->removeTab(snapshotTabIndex);
        }

        removeSnapshotPushButton->setEnabled(false);
    }
}

void ScenarioWidget::integrate(ScenarioModel *model, bool interpolation)
{
    using namespace boost::numeric::odeint;

    typedef runge_kutta_dopri5<state_type> error_stepper_type;

    int scenarioIndex = scenarioComboBox->currentIndex();

    if (shiftTimeRangesCheckbox->isChecked())
    {
        scenarioIndex = 0;
    }

    for (size_t i = scenarioIndex; i < model->scenarios.size(); i++)
    {
        Scenario *scenario = &model->scenarios[i];

        if (interpolation && i > 0)
        {
            scenario->interpolateX0(model->scenarios[i - 1]);
        }

        interpolation = true;

        scenario->x = scenario->x0;
        scenario->steps.clear();
        scenario->times.clear();

        if (model->modelIndex == 0) // SIR model
        {
            SIR sir(scenario->parameters);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sir, scenario->x, scenario->timeStart, scenario->timeEnd, 0.01, push_back_state_and_time(scenario->steps, scenario->times));
        }
        else if (model->modelIndex == 1) // SIRS model
        {
            SIRS sirs(scenario->parameters);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirs, scenario->x, scenario->timeStart, scenario->timeEnd, 0.01, push_back_state_and_time(scenario->steps, scenario->times));
        }
        else if (model->modelIndex == 2) // SEIR
        {
            SEIR seir(scenario->parameters);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), seir, scenario->x, scenario->timeStart, scenario->timeEnd, 0.01, push_back_state_and_time(scenario->steps, scenario->times));
        }
        else if (model->modelIndex == 3) // SEIRS
        {
            SEIRS seirs(scenario->parameters);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), seirs, scenario->x, scenario->timeStart, scenario->timeEnd, 0.01, push_back_state_and_time(scenario->steps, scenario->times));
        }
        else if (model->modelIndex == 4) // SIRA model
        {
            SIRA sira(scenario->parameters);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sira, scenario->x, scenario->timeStart, scenario->timeEnd, 0.01, push_back_state_and_time(scenario->steps, scenario->times));
        }
        else if (model->modelIndex == 5) // SIR + Vital dynamics model
        {
            SIRVitalDynamics sirVitalDynamics(scenario->parameters);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirVitalDynamics, scenario->x, scenario->timeStart, scenario->timeEnd, 0.01, push_back_state_and_time(scenario->steps, scenario->times));
        }
        else if (model->modelIndex == 6) // SIRS + Vital dynamics model
        {
            SIRSVitalDynamics sirsVitalDynamics(scenario->parameters);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirsVitalDynamics, scenario->x, scenario->timeStart, scenario->timeEnd, 0.01, push_back_state_and_time(scenario->steps, scenario->times));
        }
        else if (model->modelIndex == 7) // SEIR + Vital dynamics model
        {
            SEIRVitalDynamics seirVitalDynamics(scenario->parameters);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), seirVitalDynamics, scenario->x, scenario->timeStart, scenario->timeEnd, 0.01, push_back_state_and_time(scenario->steps, scenario->times));
        }
        else if (model->modelIndex == 8) // SEIRS + Vital dynamics model
        {
            SEIRSVitalDynamics seirsVitalDynamics(scenario->parameters);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), seirsVitalDynamics, scenario->x, scenario->timeStart, scenario->timeEnd, 0.01, push_back_state_and_time(scenario->steps, scenario->times));
        }
    }

    updateInitialConditionsControls();

    model->setPlotsData();
}
