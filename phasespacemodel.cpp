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

#include "phasespacemodel.h"

PhaseSpaceModel::PhaseSpaceModel(
    int index,
    QString modelName,
    std::list<QString> variableShortNamesList,
    std::list<QString> variableLongNamesList,
    std::list<QString> parameterNamesList,
    std::list<double> parameterMinList,
    std::list<double> parameterMaxList,
    std::list<double> parameterInitList,
    QWidget *parent): QWidget(parent), BaseModel(index, modelName, variableShortNamesList, variableLongNamesList, parameterNamesList, parameterMinList, parameterMaxList)
{
    for (std::list<double>::iterator it = parameterInitList.begin(); it != parameterInitList.end(); ++it)
    {
        parameterInit.push_back(*it);
    }

    parameter = parameterInit;

    xAxis = 0;
    yAxis = 1;

    icGridDimension = 10;

    timeEnd = 50;

    plot = new QCustomPlot(this);

    plot->xAxis->setRange(0.0, 1.0);
    plot->yAxis->setRange(0.0, 1.0);

    plot->xAxis->setLabel(variableLongNames[xAxis]->text() + " fraction");
    plot->yAxis->setLabel(variableLongNames[yAxis]->text() + " fraction");

    plot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);

    plot->axisRect()->setupFullAxesBox(true);
    plot->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
    plot->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);

    imgWidth = 800;
    imgHeight = 800;

    plot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(plot, &QCustomPlot::customContextMenuRequested, this, &PhaseSpaceModel::contextMenuRequest);

    updateInitialConditions(icGridDimension);
}

PhaseSpaceModel::~PhaseSpaceModel()
{
    plot->disconnect();

    plot->clearPlottables();

    delete plot;
    plot = nullptr;
}

void PhaseSpaceModel::setParameter(int k, int i, int indexMax)
{
    parameter[k] = parameterMin[k] + (parameterMax[k] - parameterMin[k]) * i / indexMax;
    integrate();
    setCurvesData();
}

int PhaseSpaceModel::getIndexParameter(int k, int indexMax)
{
    return static_cast<int>(indexMax * (parameter[k] - parameterMin[k]) / (parameterMax[k] - parameterMin[k]));
}

void PhaseSpaceModel::updateParameter(int k, double value)
{
    parameter[k] = value;
    integrate();
    setCurvesData();
}

void PhaseSpaceModel::updateInitialConditions(int dim)
{
    icGridDimension = dim;

    initialConditions.clear();

    // We consider only models with 3 dimensions
    // A 2D projection of models with >3 dimensions can be misleading

    state_type x0(dimension);

    for (int ix = 0; ix < dim; ix++)
    {
        double x = ix / (dim - 1.0);

        if (ix == 0)
        {
            x += 1.0e-7;
        }

        for (int iy = 0; iy < dim - ix; iy++)
        {
            double y = iy / (dim - 1.0);

            if (iy == 0)
            {
                y += 1.0e-7;
            }

            for (int k = 0; k < dimension; k++)
            {
                if (k == xAxis)
                {
                    x0[k] = x;
                }
                else if (k == yAxis)
                {
                    x0[k] = y;
                }
                else
                {
                    x0[k] = 1.0 - x - y; // 3D model
                }
            }

            initialConditions.push_back(x0);
        }
    }

    integrate();
    updateCurves();
    setCurvesData();
}

void PhaseSpaceModel::updateTimeEnd(double time)
{
    timeEnd = time;
    integrate();
    setCurvesData();
}

void PhaseSpaceModel::setXAxis(int xIndex)
{
    xAxis = xIndex;
    plot->xAxis->setLabel(variableLongNames[xAxis]->text() + " fraction");
    updateInitialConditions(icGridDimension);
}


void PhaseSpaceModel::setYAxis(int yIndex)
{
    yAxis = yIndex;
    plot->yAxis->setLabel(variableLongNames[yAxis]->text() + " fraction");
    updateInitialConditions(icGridDimension);
}

void PhaseSpaceModel::integrate()
{
    using namespace boost::numeric::odeint;

    typedef runge_kutta_dopri5<state_type> error_stepper_type;

    steps.clear();
    times.clear();

    for (size_t i = 0; i < initialConditions.size(); i++)
    {
        state_type x = initialConditions[i];

        std::vector<state_type> stepsVector;
        std::vector<double> timesVector;

        if (modelIndex == 0) // SIR model
        {
            SIR sir(parameter);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sir, x, 0.0, timeEnd, 0.01, push_back_state_and_time(stepsVector, timesVector));
        }
        else if (modelIndex == 1) // SIRS model
        {
            SIRS sirs(parameter);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirs, x, 0.0, timeEnd, 0.01, push_back_state_and_time(stepsVector, timesVector));
        }
        else if (modelIndex == 2) // SIR + Vital dynamics model
        {
            SIRVitalDynamics sirVitalDynamics(parameter);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirVitalDynamics, x, 0.0, timeEnd, 0.01, push_back_state_and_time(stepsVector, timesVector));
        }
        else if (modelIndex == 3) // SIRS + Vital dynamics model
        {
            SIRSVitalDynamics sirsVitalDynamics(parameter);
            integrate_adaptive(make_controlled<error_stepper_type>(1.0e-10, 1.0e-6), sirsVitalDynamics, x, 0.0, timeEnd, 0.01, push_back_state_and_time(stepsVector, timesVector));
        }

        steps.push_back(stepsVector);
        times.push_back(QVector<double>(timesVector.begin(), timesVector.end()));
    }
}

void PhaseSpaceModel::updateCurves()
{
    plot->clearPlottables();

    curves.clear();

    for (size_t i = 0; i < initialConditions.size(); i++)
    {
        QCPCurve *curve = new QCPCurve(plot->xAxis, plot->yAxis);
        curve->setPen(QPen(Qt::black));
        curves.push_back(curve);
    }
}

void PhaseSpaceModel::setCurvesData()
{
    for (size_t i = 0; i < steps.size(); i++)
    {
        QVector<double> xData;
        QVector<double> yData;

        for (size_t j = 0; j < steps[i].size(); j++)
        {
            xData.push_back(steps[i][j][xAxis]);
            yData.push_back(steps[i][j][yAxis]);
        }

        curves[i]->setData(times[i], xData, yData, true);
    }

    plot->replot();
}

void PhaseSpaceModel::contextMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction("Save plot as PNG", [=](){ savePlot(0, pos); });
    menu->addAction("Save plot as JPG", [=](){ savePlot(1, pos); });

    menu->popup(plot->mapToGlobal(pos));
}

void PhaseSpaceModel::savePlot(int format, QPoint pos)
{
    QLabel *widthLabel = new QLabel("Width (px)");

    QSpinBox *widthSpinBox = new QSpinBox;
    widthSpinBox->setRange(1, 4096);
    widthSpinBox->setValue(imgWidth);

    QLabel *heightLabel = new QLabel("Height (px)");

    QSpinBox *heightSpinBox = new QSpinBox;
    heightSpinBox->setRange(1, 2160);
    heightSpinBox->setValue(imgHeight);

    QPushButton *defaultButton = new QPushButton("Accept");

    QVBoxLayout *dialogVBoxLayout = new QVBoxLayout;
    dialogVBoxLayout->addWidget(widthLabel);
    dialogVBoxLayout->addWidget(widthSpinBox);
    dialogVBoxLayout->addWidget(heightLabel);
    dialogVBoxLayout->addWidget(heightSpinBox);
    dialogVBoxLayout->addWidget(defaultButton);

    QDialog *sizeDialog = new QDialog;
    sizeDialog->setAttribute(Qt::WA_DeleteOnClose);
    sizeDialog->setLayout(dialogVBoxLayout);

    connect(widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [&](int value) { imgWidth = value; });
    connect(heightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [&](int value) { imgHeight = value; });
    connect(defaultButton, &QPushButton::clicked, [&](){
        widthSpinBox->disconnect();
        heightSpinBox->disconnect();
        sizeDialog->done(1);
    });

    sizeDialog->move(plot->mapToGlobal(pos));
    sizeDialog->exec();

    if (format == 0)
    {
        QString fileName = QFileDialog::getSaveFileName(this, "Save plot", "", tr("Images (*.png)"));
        if (!fileName.isEmpty()) plot->savePng(fileName, imgWidth, imgHeight);
    }
    else if (format == 1)
    {
        QString fileName = QFileDialog::getSaveFileName(this, "Save plot", "", tr("Images (*.jpg)"));
        if (!fileName.isEmpty()) plot->saveJpg(fileName, imgWidth, imgHeight);
    }
}
