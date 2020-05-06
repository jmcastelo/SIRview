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

#include "scenariomodel.h"

ScenarioModel::ScenarioModel(
    int index,
    QString modelName,
    std::list<QString> variableShortNamesList,
    std::list<QString> variableLongNamesList,
    std::list<QString> parameterNamesList,
    std::list<double> parameterMinList,
    std::list<double> parameterMaxList,
    std::list<double> parameterInitList,
    std::list<double> initialConditionsList,
    QWidget *parent): QWidget(parent), BaseModel(index, modelName, variableShortNamesList, variableLongNamesList, parameterNamesList, parameterMinList, parameterMaxList)
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

    currentScenarioIndex = 0;
    currentSnapshotIndex = -1;

    for (std::list<double>::iterator it = parameterInitList.begin(); it != parameterInitList.end(); ++it)
    {
        parameterInit.push_back(*it);
    }

    for (std::list<double>::iterator it = initialConditionsList.begin(); it != initialConditionsList.end(); ++it)
    {
        initialConditions.push_back(*it);
    }

    constructPlots();
}

ScenarioModel::ScenarioModel(const ScenarioModel &model, QWidget *parent): QWidget(parent), BaseModel(model)
{
    parameterInit = model.parameterInit;

    initialConditions = model.initialConditions;

    scenarios = model.scenarios;
    currentScenarioIndex = model.currentScenarioIndex;

    for (int i = 0; i < 14; i++)
    {
        colors[i] = model.colors[i];
    }

    constructPlots();
    constructGraphs();
}

ScenarioModel::~ScenarioModel()
{
    for (unsigned long i = 0; i < plots.size(); i++)
    {
        delete plots[i];
        plots[i] = nullptr;
    }

    plots.clear();

    delete allVariablesPlot;
    allVariablesPlot = nullptr;

    delete plotsGridWidget;
    plotsGridWidget = nullptr;
}

void ScenarioModel::constructPlots()
{
    for (int i = 0; i < 2 * dimension; i++)
    {
        plots.push_back(new QCustomPlot(this));

        plots[i]->xAxis->setLabel("t/Tr");
        plots[i]->yAxis->setLabel(variableLongNames[i % dimension]->text() + " fraction");

        plots[i]->yAxis->setRange(0.0, 1.0);

        plots[i]->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);

        plots[i]->axisRect()->setupFullAxesBox(true);
        plots[i]->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
        plots[i]->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
    }

    QGridLayout *plotsGridLayout = new QGridLayout;

    if (dimension == 3)
    {
        plotsGridLayout->addWidget(plots[3], 0, 0);
        plotsGridLayout->addWidget(plots[5], 0, 1);
        plotsGridLayout->addWidget(plots[4], 1, 0, -1, -1);
    }
    else if (dimension == 4)
    {
        plotsGridLayout->addWidget(plots[4], 0, 0);
        plotsGridLayout->addWidget(plots[6], 0, 1);
        plotsGridLayout->addWidget(plots[5], 1, 0);
        plotsGridLayout->addWidget(plots[7], 1, 1);
    }

    plotsGridWidget = new QWidget;
    plotsGridWidget->setLayout(plotsGridLayout);

    allVariablesPlot = new QCustomPlot(this);

    allVariablesPlot->xAxis->setLabel("t/Tr");
    allVariablesPlot->yAxis->setLabel("All fractions");

    allVariablesPlot->yAxis->setRange(0.0, 1.0);

    allVariablesPlot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);

    allVariablesPlot->axisRect()->setupFullAxesBox(true);
    allVariablesPlot->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
    allVariablesPlot->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
}

void ScenarioModel::constructGraphs()
{
    int lastScenarioIndex = scenarios.size() - 1;

    // Plots

    for (unsigned long i = 0; i < plots.size(); i++)
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

    // All variables plot

    // Add as many graphs to plot as dimensions of the model

    for (unsigned long j = 0; j < scenarios.size(); j++)
    {
        QPen pen = QPen(colors[j % 14]);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);

        for (int i = 0; i < dimension; i++)
        {
            allVariablesPlot->addGraph();

            int numGraphs = allVariablesPlot->graphCount();

            allVariablesPlot->graph(numGraphs - 1)->setPen(pen);
        }
    }

    setPlotsData();
}

void ScenarioModel::setPlotsData()
{
    unsigned long jmax = scenarios.size() - 1;

    // Compute left and right data for scenarios until last one

    for (unsigned long j = 0; j < jmax; j++)
    {
        scenarios[j].setAbscissaOrdinate(scenarios[j + 1].timeStart);
    }

    // Compute data for last scenario

    scenarios[jmax].setAbscissaOrdinate();

    // Set plots data

    for (unsigned long i = 0; i < plots.size(); i++)
    {
        int numGraphs = plots[i]->graphCount();

        int k = 0;

        for (int j = 0; j < numGraphs - 2; j += 2)
        {
            plots[i]->graph(j)->setData(scenarios[k].abscissaLeft, scenarios[k].ordinateLeft[i % dimension], true);
            plots[i]->graph(j + 1)->setData(scenarios[k].abscissaRight, scenarios[k].ordinateRight[i % dimension], true);

            k++;
        }

        plots[i]->graph(numGraphs - 1)->setData(scenarios[k].abscissa, scenarios[k].ordinate[i % dimension], true);

        plots[i]->xAxis->rescale();
        plots[i]->replot();
    }

    // Set data for all variables plot

    int lastScenarioIndex = scenarios.size() - 1;

    for (int i = 0; i < lastScenarioIndex; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            allVariablesPlot->graph(i * dimension + j)->setData(scenarios[i].abscissaLeft, scenarios[i].ordinateLeft[j], true);
        }
    }

    for (int j = 0; j < dimension; j++)
    {
        allVariablesPlot->graph(lastScenarioIndex * dimension + j)->setData(scenarios[lastScenarioIndex].abscissa, scenarios[lastScenarioIndex].ordinate[j], true);
    }

    allVariablesPlot->xAxis->rescale();
    allVariablesPlot->replot();
}

void ScenarioModel::setGraphsOnAddScenario(int scenarioIndex)
{
    if (scenarioIndex == 0) // Adding first scenario
    {
        QPen pen = QPen(colors[0]);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);

        // Plots array: add one graph per plot

        for (unsigned long i = 0; i < plots.size(); i++)
        {
            plots[i]->addGraph();
            plots[i]->graph(0)->setPen(pen);
        }

        // All variables plot: add as many graphs as model dimensions

        for (int i = 0; i < dimension; i++)
        {
            allVariablesPlot->addGraph();
            allVariablesPlot->graph(i)->setPen(pen);
        }
    }
    else // Adding subsequent scenario
    {
        // Plots array

        // Remove last graph (previous scenario) from plots

        for (unsigned long i = 0; i < plots.size(); i++)
        {
            int numGraphs = plots[i]->graphCount();

            plots[i]->removeGraph(numGraphs - 1);
        }

        // Add two graphs (left and right from previous scenario) to plots

        for (unsigned long i = 0; i < plots.size(); i++)
        {
            plots[i]->addGraph();
            plots[i]->addGraph();

            int numGraphs = plots[i]->graphCount();

            QPen pen = QPen(colors[(scenarioIndex - 1) % 14]);
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(3);

            plots[i]->graph(numGraphs - 2)->setPen(pen);

            pen.setStyle(Qt::DashLine);
            pen.setWidth(1);

            plots[i]->graph(numGraphs - 1)->setPen(pen);
        }

        QPen pen = QPen(colors[scenarioIndex % 14]);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);

        // Add one graph (added scenario) to plots

        for (unsigned long i = 0; i < plots.size(); i++)
        {
            plots[i]->addGraph();

            int numGraphs = plots[i]->graphCount();

            plots[i]->graph(numGraphs - 1)->setPen(pen);
        }

        // All variables plot

        // Add as many graphs to plot as dimensions of the model

        for (int i = 0; i < dimension; i++)
        {
            allVariablesPlot->addGraph();

            int numGraphs = allVariablesPlot->graphCount();

            allVariablesPlot->graph(numGraphs - 1)->setPen(pen);
        }

        setPlotsData();
    }
}

void ScenarioModel::setGraphsOnRemoveScenario(int scenarioIndex)
{
    QPen pen = QPen(colors[(scenarioIndex - 1) % 14]);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);

    // Plots array

    for (unsigned long i = 0; i < plots.size(); i++)
    {
        int numGraphs = plots[i]->graphCount();

        // Remove graph from last scenario

        plots[i]->removeGraph(numGraphs - 1);

        // Remove all graphs from last scenario until removed scenario

        for (int j = numGraphs - 2; j >= 2 * scenarioIndex - 1; j--)
        {
            plots[i]->removeGraph(j);
        }

        numGraphs = plots[i]->graphCount();

        plots[i]->graph(numGraphs - 1)->setPen(pen);

        plots[i]->xAxis->rescale();
        plots[i]->replot();
    }

    // All variables plot

    int numGraphs = allVariablesPlot->graphCount();

    for (int i = numGraphs - 1; i >= dimension * scenarioIndex; i--)
    {
        allVariablesPlot->removeGraph(i);
    }
}

void ScenarioModel::onTimeStartChanged(int scenarioIndex)
{
    if (scenarioIndex - 1 >= 0)
    {
        scenarios[scenarioIndex - 1].timeStartMax = scenarios[scenarioIndex].timeStart;
        scenarios[scenarioIndex - 1].timeEndMin = scenarios[scenarioIndex].timeStart;
    }

    if (scenarioIndex + 1 < static_cast<int>(scenarios.size()))
    {
        scenarios[scenarioIndex + 1].timeStartMin = scenarios[scenarioIndex].timeStart;
    }
    else
    {
        scenarios[scenarioIndex].timeEndMin = scenarios[scenarioIndex].timeStart;
    }

}

void ScenarioModel::onTimeEndChanged(int scenarioIndex)
{
    if (scenarioIndex + 1 < static_cast<int>(scenarios.size()))
    {
        scenarios[scenarioIndex + 1].timeStartMax = scenarios[scenarioIndex].timeEnd;
    }

    if (scenarios.size() == 1)
    {
        scenarios[scenarioIndex].timeStartMax = scenarios[scenarioIndex].timeEnd;
    }

    if (scenarios[scenarioIndex].timeEnd >= scenarios[scenarioIndex].timeEndMax)
    {
        scenarios[scenarioIndex].timeEndMax += 10.0;
    }
}

void ScenarioModel::shiftTimeRanges(int scenarioIndex, double delta)
{
    for (size_t i = scenarioIndex + 1; i < scenarios.size(); i++)
    {
        scenarios[i].timeStart += delta;
        scenarios[i].timeStartMin += delta;
    }

    if (scenarios.size() == 1)
    {
        scenarios[scenarioIndex].timeStartMin += delta;
    }

    int i0 = scenarioIndex - 1 >= 0 ? scenarioIndex - 1 : 0;

    for (size_t i = i0; i < scenarios.size(); i++)
    {
        scenarios[i].timeStartMax += delta;
    }

    for (size_t i = i0; i < scenarios.size(); i++)
    {
        scenarios[i].timeEnd += delta;
        scenarios[i].timeEndMin += delta;
        scenarios[i].timeEndMax += delta;
    }
}

void ScenarioModel::updateTimeRangeMinMax()
{
    for (size_t i = 0; i < scenarios.size(); i++)
    {
        onTimeStartChanged(i);
        onTimeEndChanged(i);
    }
}

void ScenarioModel::addScenario()
{
    if (scenarios.empty())
    {
        Scenario scenario(initialConditions, parameterInit, parameterMin, parameterMax, 0.0, 0.0, 100.0, 100.0, 0.0, 110.0);
        scenarios.push_back(scenario);
    }
    else
    {
        Scenario scenario = scenarios.back();
        scenario.timeStartMin = scenario.timeStart;
        scenario.timeStartMax = scenario.timeEnd;
        scenarios.push_back(scenario);
    }

    currentScenarioIndex = scenarios.size() - 1;

    setGraphsOnAddScenario(currentScenarioIndex);
}

void ScenarioModel::removeScenario(int scenarioIndex)
{
    scenarios.erase(scenarios.begin() + scenarioIndex, scenarios.begin() + scenarios.size());
    setGraphsOnRemoveScenario(scenarioIndex);
    currentScenarioIndex = scenarios.size() - 1;
}
