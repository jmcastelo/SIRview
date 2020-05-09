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

#ifndef PHASESPACEMODEL_H
#define PHASESPACEMODEL_H

#include "basemodel.h"
#include "models.h"
#include "qcustomplot.h"
#include <boost/numeric/odeint.hpp>
#include <list>
#include <vector>
#include <QVector>
#include <QPoint>
#include <QMenu>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QDialog>
#include <QString>
#include <QFileDialog>

typedef std::vector<double> state_type;

class PhaseSpaceModel: public QWidget, public BaseModel
{
    Q_OBJECT

public:
    std::vector<double> parameter;

    int xAxis;
    int yAxis;

    int icGridDimension;

    double timeEnd;

    QCustomPlot *plot;

    PhaseSpaceModel(
        int index,
        QString modelName,
        std::list<QString> variableShortNamesList,
        std::list<QString> variableLongNamesList,
        std::list<QString> parameterNamesList,
        std::list<double> parameterMinList,
        std::list<double> parameterMaxList,
        std::list<double> parameterInitList,
        QWidget *parent = nullptr);

    ~PhaseSpaceModel();

    void setParameter(int parameterIndex, int index, int indexMax);
    int getIndexParameter(int parameterIndex, int indexMax);
    void updateParameter(int parameterIndex, double value);

    void updateInitialConditions(int dim);
    void updateTimeEnd(double time);

    void setXAxis(int xIndex);
    void setYAxis(int yIndex);

private:
    std::vector<double> parameterInit;

    std::vector<state_type> initialConditions;
    std::vector<std::vector<state_type>> steps;
    std::vector<QVector<double>> times;

    std::vector<QCPCurve*> curves;

    int imgWidth;
    int imgHeight;

    void integrate();
    void updateCurves();
    void setCurvesData();

    void contextMenuRequest(QPoint pos);
    void savePlot(int format, QPoint pos);
};

#endif // PHASESPACEMODEL_H
