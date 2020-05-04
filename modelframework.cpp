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

#include "modelframework.h"

ModelFramework::ModelFramework(
    QString modelName,
    std::list<QString> variableShortNamesList,
    std::list<QString> variableLongNamesList,
    std::list<QString> parameterNamesList,
    std::list<double> parameterMinList,
    std::list<double> parameterMaxList,
    std::list<double> parameterInitList,
    std::list<double> initialConditionsList,
    QWidget *parent): QWidget(parent)
{
    timeMin = 0.0;
    timeMax = 1000.0;

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

    currentSectionIndex = 0;

    name = modelName;

    for (std::list<QString>::iterator it = variableShortNamesList.begin(); it != variableShortNamesList.end(); ++it)
    {
        variableShortNames.push_back(new QLabel(*it));
    }

    for (std::list<QString>::iterator it = variableLongNamesList.begin(); it != variableLongNamesList.end(); ++it)
    {
        variableLongNames.push_back(new QLabel(*it));
    }

    for (std::list<QString>::iterator it = parameterNamesList.begin(); it != parameterNamesList.end(); ++it)
    {
        parameterNames.push_back(new QLabel(*it));
    }

    for (std::list<double>::iterator it = parameterMinList.begin(); it != parameterMinList.end(); ++it)
    {
        parameterMin.push_back(*it);
    }

    for (std::list<double>::iterator it = parameterMaxList.begin(); it != parameterMaxList.end(); ++it)
    {
        parameterMax.push_back(*it);
    }

    for (std::list<double>::iterator it = parameterInitList.begin(); it != parameterInitList.end(); ++it)
    {
        parameterInit.push_back(*it);
    }

    for (std::list<double>::iterator it = initialConditionsList.begin(); it != initialConditionsList.end(); ++it)
    {
        initialConditions.push_back(*it);
    }

    dimension = static_cast<int>(variableShortNames.size());
    numParameters = static_cast<int>(parameterNames.size());

    constructPlots();
}

void ModelFramework::constructPlots()
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

void ModelFramework::setPlotsData()
{
    unsigned long jmax = sections.size() - 1;

    // Compute left and right data for sections until last one

    for (unsigned long j = 0; j < jmax; j++)
    {
        sections[j].setAbscissaOrdinate(sections[j + 1].timeStart);
    }

    // Compute data for last section

    sections[jmax].setAbscissaOrdinate();

    // Set plots data

    for (unsigned long i = 0; i < plots.size(); i++)
    {
        int numGraphs = plots[i]->graphCount();

        int k = 0;

        for (int j = 0; j < numGraphs - 2; j += 2)
        {
            plots[i]->graph(j)->setData(sections[k].abscissaLeft, sections[k].ordinateLeft[i % dimension], true);
            plots[i]->graph(j + 1)->setData(sections[k].abscissaRight, sections[k].ordinateRight[i % dimension], true);

            k++;
        }

        plots[i]->graph(numGraphs - 1)->setData(sections[k].abscissa, sections[k].ordinate[i % dimension], true);

        plots[i]->xAxis->rescale();
        plots[i]->replot();
    }

    // Set data for all variables plot

    int lastSectionIndex = sections.size() - 1;

    for (int i = 0; i < lastSectionIndex; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            allVariablesPlot->graph(i * dimension + j)->setData(sections[i].abscissaLeft, sections[i].ordinateLeft[j], true);
        }
    }

    for (int j = 0; j < dimension; j++)
    {
        allVariablesPlot->graph(lastSectionIndex * dimension + j)->setData(sections[lastSectionIndex].abscissa, sections[lastSectionIndex].ordinate[j], true);
    }

    allVariablesPlot->xAxis->rescale();
    allVariablesPlot->replot();
}

void ModelFramework::setGraphsOnAddSection(int sectionIndex)
{
    if (sectionIndex == 0) // Adding first section
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
    else // Adding subsequent section
    {
        // Plots array

        // Remove last graph (previous section) from plots

        for (unsigned long i = 0; i < plots.size(); i++)
        {
            int numGraphs = plots[i]->graphCount();

            plots[i]->removeGraph(numGraphs - 1);
        }

        // Add two graphs (left and right from previous section) to plots

        for (unsigned long i = 0; i < plots.size(); i++)
        {
            plots[i]->addGraph();
            plots[i]->addGraph();

            int numGraphs = plots[i]->graphCount();

            QPen pen = QPen(colors[(sectionIndex - 1) % 14]);
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(3);

            plots[i]->graph(numGraphs - 2)->setPen(pen);

            pen.setStyle(Qt::DashLine);
            pen.setWidth(1);

            plots[i]->graph(numGraphs - 1)->setPen(pen);
        }

        QPen pen = QPen(colors[sectionIndex % 14]);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);

        // Add one graph (added section) to plots

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

void ModelFramework::setGraphsOnRemoveSection(int sectionIndex)
{
    QPen pen = QPen(colors[(sectionIndex - 1) % 14]);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);

    // Plots array

    for (unsigned long i = 0; i < plots.size(); i++)
    {
        int numGraphs = plots[i]->graphCount();

        // Remove graph from last section

        plots[i]->removeGraph(numGraphs - 1);

        // Remove all graphs from last section until removed section

        for (int j = numGraphs - 2; j >= 2 * sectionIndex - 1; j--)
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

    for (int i = numGraphs - 1; i >= dimension * sectionIndex; i--)
    {
        allVariablesPlot->removeGraph(i);
    }
}

void ModelFramework::setTimeStartMinMax(int sectionIndex)
{
    double t0 = timeMin;
    double t1 = sections[sectionIndex].timeEnd;

    if (sectionIndex - 1 > 0)
    {
        t0 = sections[sectionIndex - 1].timeStart;
    }

    if (sectionIndex + 1 < static_cast<int>(sections.size()))
    {
        t1 = sections[sectionIndex + 1].timeStart;
    }

    sections[sectionIndex].timeStartMin = t0;
    sections[sectionIndex].timeStartMax = t1;
}

void ModelFramework::setTimeEndMinMax(int sectionIndex)
{
    double t0 = sections[sectionIndex].timeStart;
    double t1 = timeMax;

    if (sectionIndex + 1 < static_cast<int>(sections.size()))
    {
        t0 = sections[sectionIndex + 1].timeStart;
    }

    sections[sectionIndex].timeEndMin = t0;
    sections[sectionIndex].timeEndMax = t1;
}

void ModelFramework::addSection()
{
    if (sections.empty())
    {
        Section section(initialConditions, parameterInit, parameterMin, parameterMax, 0.0, 50.0);
        sections.push_back(section);
    }
    else
    {
        sections.push_back(sections.back());
    }

    currentSectionIndex = sections.size() - 1;

    setGraphsOnAddSection(currentSectionIndex);
}

void ModelFramework::removeSection(int sectionIndex)
{
    sections.erase(sections.begin() + sectionIndex, sections.begin() + sections.size());
    setGraphsOnRemoveSection(sectionIndex);
    currentSectionIndex = sections.size() - 1;
}
