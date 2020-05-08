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

#ifndef SCENARIOWIDGET_H
#define SCENARIOWIDGET_H

#include "models.h"
#include "scenariogenericmodel.h"
#include "snapshot.h"
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

class ScenarioWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ScenarioWidget(QWidget *parent = nullptr);
    ~ScenarioWidget();

private:
    ScenarioModel *currentModel;
    std::vector<ScenarioModel*> models;
    std::vector<std::list<Snapshot*>> snapshots;

    QComboBox *modelComboBox;

    QPushButton *takeSnapshotPushButton;
    QPushButton *removeSnapshotPushButton;
    QComboBox *snapshotComboBox;

    QPushButton *addScenarioPushButton;
    QPushButton *removeScenarioPushButton;
    QComboBox *scenarioComboBox;

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

    void updateScenarioComboBox();

    void updateScenarioControls();
    void updateTimeStartControls();
    void updateTimeEndControls();
    void updateValidators();
    void updateParameterControls();
    void updateTimeRangeMinMax(bool shift);

    void addInitialScenarios();
    void selectScenario(int index);
    void addScenario();
    void removeScenario();

    void takeSnapshot();
    void selectSnapshot(int snapshotIndex);
    void updateSnapshotTab(int snapshotIndex);
    void removeSnapshot();
    void updateSnapshotWidgets(int modelIndex);

    void integrate(ScenarioModel *model, bool interpolation);
};

#endif // SCENARIOWIDGET_H
