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

#ifndef MODELFRAMEWORK_H
#define MODELFRAMEWORK_H

#include "section.h"
#include "qcustomplot.h"
#include <list>
#include <vector>
#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QWidget>
#include <QPen>

class ModelFramework: public QWidget
{
    Q_OBJECT

public:
    QString name;

    int modelIndex;
    int dimension;
    int numParameters;

    std::vector<QLabel*> variableShortNames;
    std::vector<QLabel*> variableLongNames;
    std::vector<QLabel*> parameterNames;

    std::vector<double> parameterMin;
    std::vector<double> parameterMax;
    std::vector<double> parameterInit;

    std::vector<double> initialConditions;

    std::vector<QCustomPlot*> plots;
    QCustomPlot *allVariablesPlot;
    QWidget *plotsGridWidget;

    std::vector<Section> sections;
    int currentSectionIndex;

    int currentSnapshotIndex;

    ModelFramework(
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

    ModelFramework(const ModelFramework &mf, QWidget *parent = nullptr);

    ~ModelFramework();

    void setPlotsData();
    void setGraphsOnAddSection(int index);
    void setGraphsOnRemoveSection(int index);

    void onTimeStartChanged(int sectionIndex);
    void onTimeEndChanged(int sectionIndex);

    void shiftTimeRanges(int sectionIndex, double delta);

    void updateTimeRangeMinMax();

    void deleteSections();
    void addSection();
    void removeSection(int sectionIndex);

private:
    Qt::GlobalColor colors[14];

    void constructPlots();
    void constructGraphs();
};

#endif // MODELFRAMEWORK_H
