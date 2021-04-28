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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "scenariowidget.h"
#include "phasespacewidget.h"
#include <QWidget>
#include <QTabWidget>
#include <QString>
#include <QLabel>
#include <QFont>
#include <QVBoxLayout>

class MainWidget: public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    ScenarioWidget *scenarioWidget;
    PhaseSpaceWidget *phaseSpaceWidget;

    QTabWidget *mainTabWidget;
};

#endif // MAINWIDGET_H
