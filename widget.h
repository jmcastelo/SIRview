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

#ifndef WIDGET_H
#define WIDGET_H

#include "models.h"
#include "modelframework.h"
#include "customvalidator.h"
#include <vector>
#include <list>
#include <iterator>
#include <boost/numeric/odeint.hpp>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QDoubleValidator>
#include <QLineEdit>
#include <QSlider>
#include <QGridLayout>
#include <QCheckBox>

class Widget: public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    ModelFramework *currentModel;
    std::vector<ModelFramework*> models;
    std::vector<std::list<ModelFramework>> snapshots;

    QComboBox *modelComboBox;

    QPushButton *takeSnapshotPushButton;
    QPushButton *removeSnapshotPushButton;
    QComboBox *snapshotComboBox;

    QVBoxLayout *mainControlsVBoxLayout;

    QPushButton *addSectionPushButton;
    QPushButton *removeSectionPushButton;
    QComboBox *sectionComboBox;

    QCheckBox *shiftTimeRangesCheckbox;
    //CustomValidator *timeStartDoubleValidator;
    //CustomValidator *timeEndDoubleValidator;
    QDoubleValidator *timeStartDoubleValidator;
    QDoubleValidator *timeEndDoubleValidator;
    QLineEdit *timeStartLineEdit;
    QLineEdit *timeEndLineEdit;
    QSlider *timeStartSlider;
    QSlider *timeEndSlider;

    QVBoxLayout *initialConditionsVBoxLayout;
    std::vector<QLineEdit*> initialConditionsLineEdit;
    QLabel *sumInitialConditionsLabel;

    QVBoxLayout *parameterVBoxLayout;
    std::vector<QLineEdit*> parameterLineEdit;
    std::vector<QSlider*> parameterSlider;

    QTabWidget *plotsTabWidget;

    void onTimeStartLineEditReturnPressed();
    void onTimeEndLineEditReturnPressed();
    void onTimeStartSliderValueChanged(int value);
    void onTimeEndSliderValueChanged(int value);

    void onParameterLineEditReturnPressed(int index);
    void onParameterSliderValueChanged(int index, int value);

    void onInitialConditionsLineEditReturnPressed(int index);

    void updateInitialConditionsControls();
    void updateSumInitialConditionsLabel();

    void deleteParameterControls();
    void constructParameterControls();

    void deleteInitialConditionsControls();
    void constructInitialConditionsControls();

    void setPlotTabs();

    void updateSectionComboBox();

    void updateSectionControls();
    void updateTimeStartControls();
    void updateTimeEndControls();
    void updateValidators();
    void updateParameterControls();
    void updateTimeRangeMinMax(bool shift);

    void addInitialSections();
    void selectSection(int index);
    void addSection();
    void removeSection();

    void takeSnapshot();
    void selectSnapshot(int snapshotIndex);
    void updateSnapshotTab(int snapshotIndex);
    void removeSnapshot();
    void updateSnapshotWidgets(int modelIndex);

    void integrate(ModelFramework *model, bool interpolation);
};

#endif // WIDGET_H
