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

#include "snapshot.h"

Snapshot::Snapshot(ScenarioModel *model)
{
    colors[0] = Qt::black;
    colors[1] = Qt::red;
    colors[2] = Qt::green;
    colors[3] = Qt::blue;
    colors[4] = Qt::darkRed;
    colors[5] = Qt::darkGreen;
    colors[6] = Qt::darkBlue;
    colors[7] = Qt::magenta;
    colors[8] = Qt::yellow;
    colors[9] = Qt::cyan;
    colors[10] = Qt::darkMagenta;
    colors[11] = Qt::darkYellow;
    colors[12] = Qt::darkCyan;
    colors[13] = Qt::darkGray;

    constructPlots(model);
    constructGraphs(model);
    copyPlotsData(model);
}

Snapshot::~Snapshot()
{
    for (size_t i = 0; i < plots.size(); i++)
    {
        delete plots[i];
        plots[i] = nullptr;
    }

    plots.clear();
}

void Snapshot::constructPlots(ScenarioModel *model)
{
    for (int i = 0; i < model->dimension; i++)
    {
        plots.push_back(new QCustomPlot(this));

        plots[i]->xAxis->setLabel("t/Tr");
        plots[i]->yAxis->setLabel(model->variableLongNames[i]->text() + " fraction");

        plots[i]->yAxis->setRange(0.0, 1.0);

        plots[i]->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);

        plots[i]->axisRect()->setupFullAxesBox(true);
        plots[i]->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
        plots[i]->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
    }


    QGridLayout *plotsGridLayout = new QGridLayout;

    if (model->dimension == 3)
    {
        plotsGridLayout->addWidget(plots[0], 0, 0);
        plotsGridLayout->addWidget(plots[2], 0, 1);
        plotsGridLayout->addWidget(plots[1], 1, 0, -1, -1);
    }
    else if (model->dimension == 4)
    {
        plotsGridLayout->addWidget(plots[0], 0, 0);
        plotsGridLayout->addWidget(plots[1], 0, 1);
        plotsGridLayout->addWidget(plots[2], 1, 0);
        plotsGridLayout->addWidget(plots[3], 1, 1);
    }

    plotsGridWidget = new QWidget;
    plotsGridWidget->setLayout(plotsGridLayout);
}

void Snapshot::constructGraphs(ScenarioModel *model)
{
    int lastScenarioIndex = model->scenarios.size() - 1;

    // Plots

    for (size_t i = 0; i < plots.size(); i++)
    {
        // Add graphs corresponding to all scenarios except for the last one

        for (int j = 0; j < lastScenarioIndex; j++)
        {
            plots[i]->addGraph();
            plots[i]->addGraph();

            int numGraphs = plots[i]->graphCount();

            QPen pen = QPen(colors[j % 14]);
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(3);

            plots[i]->graph(numGraphs - 2)->setPen(pen);

            pen.setStyle(Qt::DashLine);
            pen.setWidth(1);

            plots[i]->graph(numGraphs - 1)->setPen(pen);
        }

        // Last graph (last scenario)

        QPen pen = QPen(colors[lastScenarioIndex % 14]);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);

        plots[i]->addGraph();

        int numGraphs = plots[i]->graphCount();

        plots[i]->graph(numGraphs - 1)->setPen(pen);
    }
}

void Snapshot::copyPlotsData(ScenarioModel *model)
{
    for (size_t i = 0; i < plots.size(); i++)
    {
        int numGraphs = plots[i]->graphCount();

        for (int j = 0; j < numGraphs; j++)
        {
            plots[i]->graph(j)->data()->set(*model->plots[i]->graph(j)->data());
        }

        plots[i]->xAxis->rescale();
        plots[i]->replot();
    }
}
