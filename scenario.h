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

#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>
#include <QVector>

typedef std::vector<double> state_type;

class Scenario
{
public:
    state_type x;
    std::vector<double> x0;
    std::vector<state_type> steps;
    std::vector<double> times;

    QVector<double> abscissa, abscissaLeft, abscissaRight;
    std::vector<QVector<double>> ordinate, ordinateLeft, ordinateRight;

    double timeStart, timeStartMin, timeStartMax;
    double timeEnd, timeEndMin, timeEndMax;
    std::vector<double> parameters, parametersMin, parametersMax;

    Scenario(std::vector<double> xStart, std::vector<double> p, std::vector<double> pMin, std::vector<double> pMax, double t0, double t0Min, double t0Max, double t1, double t1Min, double t1Max):
        x0(xStart),
        timeStart(t0),
        timeStartMin(t0Min),
        timeStartMax(t0Max),
        timeEnd(t1),
        timeEndMin(t1Min),
        timeEndMax(t1Max),
        parameters(p),
        parametersMin(pMin),
        parametersMax(pMax){}

    void setTimeStart(int i, int indexMax);
    void setTimeEnd(int i, int indexMax);
    void setParameter(int k, int i, int indexMax);

    int getIndexTimeStart(int indexMax);
    int getIndexTimeEnd(int indexMax);
    int getIndexParameter(int k, int indexMax);

    void interpolateX0(Scenario scenario);

    void setAbscissaOrdinate();
    void setAbscissaOrdinate(double time);
};

#endif // SCENARIO_H
