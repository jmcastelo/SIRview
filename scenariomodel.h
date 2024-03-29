// Copyright 2021 Jose Maria Castelo Ares

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

#ifndef SCENARIOMODEL_H
#define SCENARIOMODEL_H

#include "basemodel.h"
#include "scenario.h"
#include "qcustomplot.h"
#include <list>
#include <vector>
#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QWidget>
#include <QPen>
#include <QPoint>
#include <QMenu>
#include <QSpinBox>
#include <QPushButton>
#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

class ScenarioModel: virtual public QWidget, public BaseModel
{
    Q_OBJECT

public:
    std::vector<double> parameterInit;

    std::vector<double> initialConditions;

    std::vector<QCustomPlot*> plots;
    QCustomPlot *allVariablesPlot;
    QWidget *plotsGridWidget;

    std::vector<Scenario> scenarios;

    int currentScenarioIndex;
    int currentSnapshotIndex;

    ScenarioModel(
        int index,
        QString modelName,
        std::list<QString> variableShortNamesList,
        std::list<QString> variableLongNamesList,
        std::list<QString> parameterNamesList,
        std::list<double> parameterMinList,
        std::list<double> parameterMaxList,
        std::list<double> parameterInitList,
        std::list<double> initialConditionsList,
        QWidget *parent = nullptr);

    ScenarioModel(const ScenarioModel &model, QWidget *parent = nullptr);

    ~ScenarioModel();

    void connectPlots();

    void setPlotsData();
    void setGraphsOnAddScenario(int scenarioIndex);
    void setGraphsOnRemoveScenario(int scenarioIndex);

    void onTimeStartChanged(int scenarioIndex);
    void onTimeEndChanged(int scenarioIndex);

    void shiftTimeRanges(int scenarioIndex, double delta);

    void updateTimeRangeMinMax();

    void deleteScenarios();
    void addScenario();
    void removeScenario(int scenarioIndex);

    void exportData();

private:
    Qt::GlobalColor colors[14];

    int imgWidth;
    int imgHeight;

    void constructPlots();
    void constructGraphs();

    void contextMenuRequest(int plotIndex, QPoint pos);
    void savePlot(int plotIndex, int format, QPoint pos);

    virtual void constructAdditionalPlots() = 0;
    virtual void setAdditionalPlotsData() = 0;
};

#endif // SCENARIOMODEL_H
