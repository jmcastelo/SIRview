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

#include "scenariosiramodel.h"

void ScenarioSIRAModel::constructAdditionalPlots()
{
    plots.push_back(new QCustomPlot(this));

    plots.back()->xAxis->setLabel("t/Tr");
    plots.back()->yAxis->setLabel("(A - I)/(A + I)");

    plots.back()->yAxis->setRange(-1.0, 1.0);

    plots.back()->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);

    plots.back()->axisRect()->setupFullAxesBox(true);
    plots.back()->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
    plots.back()->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
}

void ScenarioSIRAModel::setAdditionalPlotsData()
{
    // We assume variable A (asymptomatic) has index 3
    // We assume variable I (infected) has index 1

    std::vector<QVector<double>> fractionsLeft;
    std::vector<QVector<double>> fractionsRight;

    for (size_t k = 0; k < scenarios.size() - 1; k++)
    {
        QVector<double> dataLeft;

        for (int i = 0; i < scenarios[k].ordinateLeft[3].size(); i++)
        {
            double A = scenarios[k].ordinateLeft[3][i];
            double I = scenarios[k].ordinateLeft[1][i];

            double f = (A - I) / (A + I);

            dataLeft.push_back(f);
        }

        fractionsLeft.push_back(dataLeft);

        QVector<double> dataRight;

        for (int i = 0; i < scenarios[k].ordinateRight[3].size(); i++)
        {
            double A = scenarios[k].ordinateRight[3][i];
            double I = scenarios[k].ordinateRight[1][i];

            double f = (A - I) / (A + I);

            dataRight.push_back(f);
        }

        fractionsRight.push_back(dataRight);
    }

    QVector<double> fractions;

    int k = scenarios.size() - 1;

    for (int i = 0; i < scenarios[k].ordinate[3].size(); i++)
    {
        double A = scenarios[k].ordinate[3][i];
        double I = scenarios[k].ordinate[1][i];

        double f = (A - I) / (A + I);

        fractions.push_back(f);
    }

    int numGraphs = plots.back()->graphCount();

    k = 0;

    for (int j = 0; j < numGraphs - 2; j += 2)
    {
        plots.back()->graph(j)->setData(scenarios[k].abscissaLeft, fractionsLeft[k], true);
        plots.back()->graph(j + 1)->setData(scenarios[k].abscissaRight, fractionsRight[k], true);

        k++;
    }

    plots.back()->graph(numGraphs - 1)->setData(scenarios[k].abscissa, fractions, true);

    plots.back()->xAxis->rescale();
    plots.back()->replot();
}
