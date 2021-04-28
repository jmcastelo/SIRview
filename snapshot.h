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

#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "scenariomodel.h"
#include "qcustomplot.h"
#include <QWidget>
#include <QPen>

class Snapshot: public QWidget
{
    Q_OBJECT

public:
    QWidget *plotsGridWidget;

    Snapshot(ScenarioModel *model);
    ~Snapshot();

private:
    std::vector<QCustomPlot*> plots;

    Qt::GlobalColor colors[14];

    void constructPlots(ScenarioModel *model);
    void constructGraphs(ScenarioModel *model);
    void copyPlotsData(ScenarioModel *model);
};

#endif // SNAPSHOT_H
