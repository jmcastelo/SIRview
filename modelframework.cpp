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
    int index,
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
    currentSnapshotIndex = -1;

    name = modelName;

    modelIndex = index;

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

ModelFramework::ModelFramework(const ModelFramework &mf, QWidget *parent): QWidget(parent)
{
    name = mf.name;

    dimension = mf.dimension;
    numParameters = mf.numParameters;

    for (unsigned long i = 0; i < mf.variableShortNames.size(); i++)
    {
        QLabel *label = new QLabel;
        label->setText(mf.variableShortNames[i]->text());
        variableShortNames.push_back(label);
    }

    for (unsigned long i = 0; i < mf.variableLongNames.size(); i++)
    {
        QLabel *label = new QLabel;
        label->setText(mf.variableLongNames[i]->text());
        variableLongNames.push_back(label);
    }

    for (unsigned long i = 0; i < mf.parameterNames.size(); i++)
    {
        QLabel *label = new QLabel;
        label->setText(mf.parameterNames[i]->text());
        parameterNames.push_back(label);
    }

    parameterMin = mf.parameterMin;
    parameterMax = mf.parameterMax;
    parameterInit = mf.parameterInit;

    initialConditions = mf.initialConditions;

    sections = mf.sections;
    currentSectionIndex = mf.currentSectionIndex;

    for (int i = 0; i < 14; i++)
    {
        colors[i] = mf.colors[i];
    }

    constructPlots();
    constructGraphs();
}

ModelFramework::~ModelFramework()
{
    for (unsigned long i = 0; i < variableShortNames.size(); i++)
    {
        delete variableShortNames[i];
        variableShortNames[i] = nullptr;
    }

    variableShortNames.clear();

    for (unsigned long i = 0; i < variableLongNames.size(); i++)
    {
        delete variableLongNames[i];
        variableLongNames[i] = nullptr;
    }

    variableLongNames.clear();

    for (unsigned long i = 0; i < parameterNames.size(); i++)
    {
        delete parameterNames[i];
        parameterNames[i] = nullptr;
    }

    parameterNames.clear();

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

void ModelFramework::constructGraphs()
{
    int lastSectionIndex = sections.size() - 1;

    // Plots

    for (unsigned long i = 0; i < plots.size(); i++)
    {
        // Add graphs corresponding to all sections except for the last one

        for (int j = 0; j < lastSectionIndex; j++)
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

        // Last graph (last section)

        QPen pen = QPen(colors[lastSectionIndex % 14]);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);

        plots[i]->addGraph();

        int numGraphs = plots[i]->graphCount();

        plots[i]->graph(numGraphs - 1)->setPen(pen);
    }

    // All variables plot

    // Add as many graphs to plot as dimensions of the model

    for (unsigned long j = 0; j < sections.size(); j++)
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

void ModelFramework::onTimeStartChanged(int sectionIndex)
{
    if (sectionIndex - 1 >= 0)
    {
        sections[sectionIndex - 1].timeStartMax = sections[sectionIndex].timeStart;
        sections[sectionIndex - 1].timeEndMin = sections[sectionIndex].timeStart;
    }

    if (sectionIndex + 1 < static_cast<int>(sections.size()))
    {
        sections[sectionIndex + 1].timeStartMin = sections[sectionIndex].timeStart;
    }
    else
    {
        sections[sectionIndex].timeEndMin = sections[sectionIndex].timeStart;
    }

}

void ModelFramework::onTimeEndChanged(int sectionIndex)
{
    if (sectionIndex + 1 < static_cast<int>(sections.size()))
    {
        sections[sectionIndex + 1].timeStartMax = sections[sectionIndex].timeEnd;
    }

    if (sections.size() == 1)
    {
        sections[sectionIndex].timeStartMax = sections[sectionIndex].timeEnd;
    }

    if (sections[sectionIndex].timeEnd >= sections[sectionIndex].timeEndMax)
    {
        sections[sectionIndex].timeEndMax += 10.0;
    }
}

void ModelFramework::shiftTimeRanges(int sectionIndex, double delta)
{
    for (size_t i = sectionIndex + 1; i < sections.size(); i++)
    {
        sections[i].timeStart += delta;
        sections[i].timeStartMin += delta;
    }

    if (sections.size() == 1)
    {
        sections[sectionIndex].timeStartMin += delta;
    }

    int i0 = sectionIndex - 1 >= 0 ? sectionIndex - 1 : 0;

    for (size_t i = i0; i < sections.size(); i++)
    {
        sections[i].timeStartMax += delta;
    }

    for (size_t i = i0; i < sections.size(); i++)
    {
        sections[i].timeEnd += delta;
        sections[i].timeEndMin += delta;
        sections[i].timeEndMax += delta;
    }
}

void ModelFramework::updateTimeRangeMinMax()
{
    for (size_t i = 0; i < sections.size(); i++)
    {
        onTimeStartChanged(i);
        onTimeEndChanged(i);
    }
}

void ModelFramework::addSection()
{
    if (sections.empty())
    {
        Section section(initialConditions, parameterInit, parameterMin, parameterMax, 0.0, 0.0, 100.0, 100.0, 0.0, 110.0);
        sections.push_back(section);
    }
    else
    {
        Section section = sections.back();
        section.timeStartMin = section.timeStart;
        section.timeStartMax = section.timeEnd;
        sections.push_back(section);
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
