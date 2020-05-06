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

    models.push_back(new ModelFramework(0, "SIR", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0"}, {0.0}, {20.0}, {2.5}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));
    models.push_back(new ModelFramework(1, "SIRS", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));
    models.push_back(new ModelFramework(2, "SIRA", {"S", "I", "R", "A"}, {"Susceptible", "Infected", "Recovered", "Asymptomatic"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0, 0.0}));
    models.push_back(new ModelFramework(3, "SIR + Vital dynamics", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1"}, {0.0, 0.0}, {20.0, 5.0}, {2.5, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));
    models.push_back(new ModelFramework(4, "SIRS + Vital dynamics", {"S", "I", "R"}, {"Susceptible", "Infected", "Recovered"}, {"P0", "P1", "P3"}, {0.0, 0.0, 0.0}, {20.0, 5.0, 5.0}, {2.5, 0.1, 0.1}, {1.0 - 1.0e-7, 1.0e-7, 0.0}));

    currentModel = models[0];

    // Init snapshots vector

    std::list<ModelFramework> snapshot;

    snapshots.reserve(5);

    for (unsigned long i = 0; i < models.size(); i++)
    {
        snapshots.push_back(snapshot);
    }

    // Model selection controls

    QLabel *modelLabel = new QLabel("Model");

    modelComboBox = new QComboBox;

    for (unsigned long i = 0; i < models.size(); i++)
    {
        modelComboBox->addItem(models[i]->name);
    }

    modelComboBox->setCurrentIndex(0);

    // Model management controls

    QLabel *snapshotLabel = new QLabel("Snapshots");

    takeSnapshotPushButton = new QPushButton("Take");
    removeSnapshotPushButton = new QPushButton("Remove");
    removeSnapshotPushButton->setEnabled(false);

    QHBoxLayout *takeRemovePushButtonsHBoxLayout = new QHBoxLayout;
    takeRemovePushButtonsHBoxLayout->addWidget(takeSnapshotPushButton);
    takeRemovePushButtonsHBoxLayout->addWidget(removeSnapshotPushButton);

    snapshotComboBox = new QComboBox;

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

    // Main controls vertical layout & widget

    mainControlsVBoxLayout = new QVBoxLayout;
    mainControlsVBoxLayout->addWidget(modelLabel);
    mainControlsVBoxLayout->addWidget(modelComboBox);
    mainControlsVBoxLayout->addWidget(snapshotLabel);
    mainControlsVBoxLayout->addLayout(takeRemovePushButtonsHBoxLayout);
    mainControlsVBoxLayout->addWidget(snapshotComboBox);
    mainControlsVBoxLayout->addWidget(sectionLabel);
    mainControlsVBoxLayout->addLayout(addRemovePushButtonsHBoxLayout);
    mainControlsVBoxLayout->addWidget(sectionComboBox);
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
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) updateSectionComboBox(); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ Q_UNUSED(modelIndex) updateSectionControls(); updateInitialConditionsControls(); });
    connect(modelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int modelIndex){ updateSnapshotWidgets(modelIndex); });
    connect(takeSnapshotPushButton, &QPushButton::clicked, this, &Widget::takeSnapshot);
    connect(removeSnapshotPushButton, &QPushButton::clicked, this, &Widget::removeSnapshot);
    connect(snapshotComboBox, QOverload<int>::of(&QComboBox::activated), [=](int snapshotIndex){ selectSnapshot(snapshotIndex);});
    connect(sectionComboBox, QOverload<int>::of(&QComboBox::activated), [=](int sectionIndex){ selectSection(sectionIndex); });
    connect(addSectionPushButton, &QPushButton::clicked, this, &Widget::addSection);
    connect(removeSectionPushButton, &QPushButton::clicked, this, &Widget::removeSection);
    connect(shiftTimeRangesCheckbox, &QCheckBox::toggled, [this](bool shift){ updateTimeRangeMinMax(shift); updateTimeStartControls(); updateTimeEndControls(); updateValidators(); });
    connect(timeStartLineEdit, &QLineEdit::returnPressed, this, &Widget::onTimeStartLineEditReturnPressed);
    connect(timeEndLineEdit, &QLineEdit::returnPressed, this, &Widget::onTimeEndLineEditReturnPressed);
    connect(timeStartSlider, &QSlider::valueChanged, this, &Widget::onTimeStartSliderValueChanged);
    connect(timeEndSlider, &QSlider::valueChanged, this, &Widget::onTimeEndSliderValueChanged);

    // Sections setup

    constructInitialConditionsControls();
    constructParameterControls();
    addInitialSections();

    // Set plot tabs

    setPlotTabs();

    // Main widget

    setLayout(mainGridLayout);

    setWindowTitle("SIRview");

    resize(1200, 800);
}

Widget::~Widget()
{
    for (unsigned long i = 0; i < models.size(); i++)
    {
        delete models[i];
        models[i]= nullptr;
    }

    models.clear();
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

void Widget::constructParameterControls()
{
    deleteParameterControls();

    for (int i = 0; i < currentModel->numParameters; i++)
    {
        QLineEdit *lineEdit = new QLineEdit;

        CustomValidator *validator = new CustomValidator(currentModel->parameterMin[i], currentModel->parameterMax[i], 10, lineEdit);
        validator->setNotation(QDoubleValidator::StandardNotation);
        validator->setLocale(QLocale::English);

        lineEdit->setValidator(validator);

        currentModel->parameterNames[i];

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

void Widget::constructInitialConditionsControls()
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

void Widget::setPlotTabs()
{
    plotsTabWidget->clear();

    plotsTabWidget->addTab(currentModel->plotsGridWidget, "All");
    plotsTabWidget->addTab(currentModel->allVariablesPlot, "Joint");

    for (int i = 0; i < currentModel->dimension; i++)
    {
        plotsTabWidget->addTab(currentModel->plots[i], currentModel->variableLongNames[i]->text());
    }
}

void Widget::updateSectionComboBox()
{
    sectionComboBox->clear();

    for (unsigned long i = 0; i < currentModel->sections.size(); i++)
    {
        sectionComboBox->addItem(QString("Section %1").arg(i + 1));
    }

    sectionComboBox->setCurrentIndex(currentModel->currentSectionIndex);

    selectSection(currentModel->currentSectionIndex);
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
        ModelFramework *model = models[modelIndex];
        integrate(model, false);
    }
}

void Widget::selectSection(int sectionIndex)
{
    if (sectionIndex == 0)
    {
        removeSectionPushButton->setEnabled(false);

        for (int i = 0; i < currentModel->dimension; i++)
        {
            initialConditionsLineEdit[i]->setEnabled(true);
        }
    }
    else
    {
        removeSectionPushButton->setEnabled(true);

        for (int i = 0; i < currentModel->dimension; i++)
        {
            initialConditionsLineEdit[i]->setEnabled(false);
        }
    }

    currentModel->currentSectionIndex = sectionIndex;

    updateSectionControls();
    updateInitialConditionsControls();
}

void Widget::addSection()
{
    currentModel->addSection();

    int sectionIndex = currentModel->currentSectionIndex;

    sectionComboBox->addItem(QString("Section %1").arg(sectionIndex + 1));
    sectionComboBox->setCurrentIndex(sectionIndex);

    selectSection(sectionIndex);
}

void Widget::removeSection()
{
    int sectionIndex = sectionComboBox->currentIndex();
    int jmax = currentModel->sections.size() - 1;

    for (int j = jmax; j >= sectionIndex; j--)
    {
        sectionComboBox->removeItem(j);
    }

    currentModel->removeSection(sectionIndex);

    sectionIndex = currentModel->currentSectionIndex;

    selectSection(sectionIndex);

    if (sectionIndex == 0)
    {
        integrate(currentModel, false);
    }
    else
    {
        integrate(currentModel, true);
    }
}

void Widget::updateSectionControls()
{
    updateTimeStartControls();
    updateTimeEndControls();
    updateValidators();
    updateParameterControls();
}

void Widget::updateTimeStartControls()
{
    int sectionIndex = currentModel->currentSectionIndex;

    timeStartLineEdit->setText(QString("%1").arg(currentModel->sections[sectionIndex].timeStart));

    bool startSliderState = timeStartSlider->blockSignals(true);

    int timeStartSliderPosition = currentModel->sections[sectionIndex].getIndexTimeStart(timeStartSlider->maximum());
    timeStartSlider->setValue(timeStartSliderPosition);

    timeStartSlider->blockSignals(startSliderState);
}

void Widget::updateTimeEndControls()
{
    int sectionIndex = currentModel->currentSectionIndex;

    timeEndLineEdit->setText(QString("%1").arg(currentModel->sections[sectionIndex].timeEnd));

    bool endSliderState = timeEndSlider->blockSignals(true);

    int timeEndSliderPosition = currentModel->sections[sectionIndex].getIndexTimeEnd(timeEndSlider->maximum());
    timeEndSlider->setValue(timeEndSliderPosition);

    timeEndSlider->blockSignals(endSliderState);
}

void Widget::updateValidators()
{
    int sectionIndex = currentModel->currentSectionIndex;

    timeStartDoubleValidator->setBottom(currentModel->sections[sectionIndex].timeStartMin);
    timeStartDoubleValidator->setTop(currentModel->sections[sectionIndex].timeStartMax);

    timeEndDoubleValidator->setBottom(currentModel->sections[sectionIndex].timeEndMin);
    timeEndDoubleValidator->setTop(currentModel->sections[sectionIndex].timeEndMax);
}

void Widget::updateParameterControls()
{
    int sectionIndex = currentModel->currentSectionIndex;

    for (int i = 0; i < currentModel->numParameters; i++)
    {
        parameterLineEdit[i]->setText(QString("%1").arg(currentModel->sections[sectionIndex].parameters[i]));
    }

    for (int i = 0; i < currentModel->numParameters; i++)
    {
         int parameterSliderPosition = currentModel->sections[sectionIndex].getIndexParameter(i, parameterSlider[i]->maximum());
         parameterSlider[i]->setValue(parameterSliderPosition);
    }
}

void Widget::updateTimeRangeMinMax(bool shift)
{
    if (!shift)
    {
        currentModel->updateTimeRangeMinMax();
    }
}

void Widget::onTimeStartLineEditReturnPressed()
{
    int sectionIndex = sectionComboBox->currentIndex();

    if (shiftTimeRangesCheckbox->isChecked())
    {
        double timeStart0 = currentModel->sections[sectionIndex].timeStart;
        double timeStart1 = timeStartLineEdit->text().toDouble();

        currentModel->sections[sectionIndex].timeStart = timeStart1;
        currentModel->shiftTimeRanges(sectionIndex, timeStart1 - timeStart0);

        updateTimeEndControls();
    }
    else
    {
        currentModel->sections[sectionIndex].timeStart = timeStartLineEdit->text().toDouble();
        currentModel->onTimeStartChanged(sectionIndex);
    }

    updateTimeStartControls();
    updateValidators();

    integrate(currentModel, true);
}

void Widget::onTimeEndLineEditReturnPressed()
{
    int sectionIndex = sectionComboBox->currentIndex();

    currentModel->sections[sectionIndex].timeEnd = timeEndLineEdit->text().toDouble();
    currentModel->onTimeEndChanged(sectionIndex);

    updateTimeEndControls();
    updateValidators();

    integrate(currentModel, false);
}

void Widget::onTimeStartSliderValueChanged(int value)
{
    int sectionIndex = sectionComboBox->currentIndex();

    if (shiftTimeRangesCheckbox->isChecked())
    {
        double timeStart0 = currentModel->sections[sectionIndex].timeStart;
        currentModel->sections[sectionIndex].setTimeStart(value, timeStartSlider->maximum());

        double timeStart1 = currentModel->sections[sectionIndex].timeStart;
        currentModel->shiftTimeRanges(sectionIndex, timeStart1 - timeStart0);

        updateTimeEndControls();
    }
    else
    {
        currentModel->sections[sectionIndex].setTimeStart(value, timeStartSlider->maximum());
        currentModel->onTimeStartChanged(sectionIndex);
    }

    timeStartLineEdit->setText(QString("%1").arg(currentModel->sections[sectionIndex].timeStart));

    updateValidators();

    integrate(currentModel, true);
}

void Widget::onTimeEndSliderValueChanged(int value)
{
    int sectionIndex = sectionComboBox->currentIndex();

    currentModel->sections[sectionIndex].setTimeEnd(value, timeEndSlider->maximum());
    currentModel->onTimeEndChanged(sectionIndex);

    timeEndLineEdit->setText(QString("%1").arg(currentModel->sections[sectionIndex].timeEnd));

    if (value == timeEndSlider->maximum())
    {
        bool endSliderState = timeEndSlider->blockSignals(true);
        int sliderPosition = currentModel->sections[sectionIndex].getIndexTimeEnd(timeEndSlider->maximum());
        timeEndSlider->setValue(sliderPosition);
        timeEndSlider->blockSignals(endSliderState);
    }

    bool startSliderState = timeStartSlider->blockSignals(true);
    int timeStartSliderPosition = currentModel->sections[sectionIndex].getIndexTimeStart(timeStartSlider->maximum());
    timeStartSlider->setValue(timeStartSliderPosition);
    timeStartSlider->blockSignals(startSliderState);

    updateValidators();

    integrate(currentModel, false);
}

void Widget::onParameterLineEditReturnPressed(int index)
{
    int sectionIndex = sectionComboBox->currentIndex();

    currentModel->sections[sectionIndex].parameters[index] = parameterLineEdit[index]->text().toDouble();

    int sliderPosition = currentModel->sections[sectionIndex].getIndexParameter(index, parameterSlider[index]->maximum());
    parameterSlider[index]->setSliderPosition(sliderPosition);

    integrate(currentModel, false);
}

void Widget::onParameterSliderValueChanged(int index, int value)
{
    int sectionIndex = sectionComboBox->currentIndex();

    currentModel->sections[sectionIndex].setParameter(index, value, parameterSlider[index]->maximum());

    parameterLineEdit[index]->setText(QString("%1").arg(currentModel->sections[sectionIndex].parameters[index]));

    integrate(currentModel, false);
}

void Widget::onInitialConditionsLineEditReturnPressed(int index)
{
    currentModel->sections[0].x0[index] = initialConditionsLineEdit[index]->text().toDouble();

    updateSumInitialConditionsLabel();

    integrate(currentModel, false);
}

void Widget::updateInitialConditionsControls()
{
    int sectionIndex = currentModel->currentSectionIndex;

    for (int i = 0; i < currentModel->dimension; i++)
    {
        initialConditionsLineEdit[i]->setText(QString("%1").arg(currentModel->sections[sectionIndex].x0[i]));
    }

    updateSumInitialConditionsLabel();
}

void Widget::updateSumInitialConditionsLabel()
{
    int sectionIndex = currentModel->currentSectionIndex;

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

        sum += currentModel->sections[sectionIndex].x0[i];
    }

    text.append(QString("%1").arg(sum));

    sumInitialConditionsLabel->setText(text);
    sumInitialConditionsLabel->repaint();
}

void Widget::takeSnapshot()
{
    int modelIndex = modelComboBox->currentIndex();

    snapshots[modelIndex].push_back(*models[modelIndex]);

    int snapshotIndex = snapshots[modelIndex].size() - 1;

    snapshotComboBox->addItem(QString("Snapshot %1").arg(snapshotIndex + 1));
    snapshotComboBox->setCurrentIndex(snapshotIndex);

    updateSnapshotTab(snapshotIndex);

    removeSnapshotPushButton->setEnabled(true);

    currentModel->currentSnapshotIndex = snapshotIndex;
}

void Widget::selectSnapshot(int snapshotIndex)
{
    updateSnapshotTab(snapshotIndex);

    int snapshotTabIndex = 2 + currentModel->dimension;

    plotsTabWidget->setCurrentIndex(snapshotTabIndex);

    currentModel->currentSnapshotIndex = snapshotIndex;
}

void Widget::updateSnapshotTab(int snapshotIndex)
{
    int modelIndex = modelComboBox->currentIndex();
    int snapshotTabIndex = 2 + currentModel->dimension;
    int plotsTabWidgetIndex = plotsTabWidget->currentIndex();

    if (snapshots[modelIndex].size() > 1)
    {
        plotsTabWidget->removeTab(snapshotTabIndex);
    }

    auto it = std::next(snapshots[modelIndex].begin(), snapshotIndex);
    plotsTabWidget->addTab((*it).plotsGridWidget, "Snapshot");

    plotsTabWidget->setCurrentIndex(plotsTabWidgetIndex);
}

void Widget::removeSnapshot()
{
    int modelIndex = modelComboBox->currentIndex();
    int snapshotTabIndex = 2 + currentModel->dimension;
    int plotsTabWidgetIndex = plotsTabWidget->currentIndex();
    int snapshotIndex = snapshotComboBox->currentIndex();

    plotsTabWidget->removeTab(snapshotTabIndex);

    std::list<ModelFramework>::iterator it = snapshots[modelIndex].begin();
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
        plotsTabWidget->addTab((*it).plotsGridWidget, "Snapshot");

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

void Widget::updateSnapshotWidgets(int modelIndex)
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

void Widget::integrate(ModelFramework *model, bool interpolation)
{
    using namespace boost::numeric::odeint;

    typedef runge_kutta_dopri5<state_type> error_stepper_type;

    int sectionIndex = sectionComboBox->currentIndex();

    if (shiftTimeRangesCheckbox->isChecked())
    {
        sectionIndex = 0;
    }

    for (unsigned long i = sectionIndex; i < model->sections.size(); i++)
    {
        Section *section = &model->sections[i];

        if (interpolation && i > 0)
        {
            section->interpolateX0(model->sections[i - 1]);
        }

        interpolation = true;

        section->x = section->x0;
        section->steps.clear();
        section->times.clear();

        if (model->modelIndex == 0) // SIR model
        {
            SIR sir(section->parameters[0]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sir, section->x, section->timeStart, section->timeEnd, 0.01, push_back_state_and_time(section->steps, section->times));
        }
        else if (model->modelIndex == 1) // SIRS model
        {
            SIRS sirs(section->parameters[0], section->parameters[1]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirs, section->x, section->timeStart, section->timeEnd, 0.01, push_back_state_and_time(section->steps, section->times));
        }
        else if (model->modelIndex == 2) // SIRA model
        {
            SIRA sira(section->parameters[0], section->parameters[1]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sira, section->x, section->timeStart, section->timeEnd, 0.01, push_back_state_and_time(section->steps, section->times));
        }
        else if (model->modelIndex == 3) // SIR + Vital dynamics model
        {
            SIRVitalDynamics sirVitalDynamics(section->parameters[0], section->parameters[1]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirVitalDynamics, section->x, section->timeStart, section->timeEnd, 0.01, push_back_state_and_time(section->steps, section->times));
        }
        else if (model->modelIndex == 4) // SIRS + Vital dynamics model
        {
            SIRSVitalDynamics sirsVitalDynamics(section->parameters[0], section->parameters[1], section->parameters[2]);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirsVitalDynamics, section->x, section->timeStart, section->timeEnd, 0.01, push_back_state_and_time(section->steps, section->times));
        }
    }

    updateInitialConditionsControls();

    model->setPlotsData();
}
