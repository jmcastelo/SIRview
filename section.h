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

#ifndef SECTION_H
#define SECTION_H

#include <vector>
#include <QVector>

typedef std::vector<double> state_type;

class Section
{
public:
    int dim;

    state_type x0, x;
    std::vector<state_type> steps;
    std::vector<double> times;

    QVector<double> abscissa, abscissaLeft, abscissaRight;
    std::vector<QVector<double>> ordinate, ordinateLeft, ordinateRight;

    double timeStart, timeStartMin, timeStartMax;
    double timeEnd, timeEndMin, timeEndMax;
    std::vector<double> parameters, parametersMin, parametersMax;

    Section(int d, state_type xStart, std::vector<double> p, std::vector<double> pMin, std::vector<double> pMax, double t0, double t1):
        dim(d),
        x0(xStart),
        timeStart(t0),
        timeEnd(t1),
        parameters(p),
        parametersMin(pMin),
        parametersMax(pMax){}

    void setTimeStart(int i, int indexMax);
    void setTimeEnd(int i, int indexMax);
    void setParameter(int k, int i, int indexMax);

    int getIndexTimeStart(int indexMax);
    int getIndexTimeEnd(int indexMax);
    int getIndexParameter(int k, int indexMax);

    void interpolateX0(Section section);

    void setAbscissaOrdinate();
    void setAbscissaOrdinate(double time);
};

#endif // SECTION_H
