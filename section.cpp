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

#include "section.h"

void Section::setTimeStart(int i, int indexMax)
{
    timeStart = timeStartMin + (timeStartMax - timeStartMin) * i / indexMax;
}

void Section::setTimeEnd(int i, int indexMax)
{
    timeEnd = timeEndMin + (timeEndMax - timeEndMin) * i / indexMax;
}

void Section::setParameter(int k, int i, int indexMax)
{
    parameters[k] = parametersMin[k] + (parametersMax[k] - parametersMin[k]) * i / indexMax;
}

int Section::getIndexTimeStart(int indexMax)
{
    return static_cast<int>(indexMax * (timeStart - timeStartMin) / (timeStartMax - timeStartMin));
}

int Section::getIndexTimeEnd(int indexMax)
{
    return static_cast<int>(indexMax * (timeEnd - timeEndMin) / (timeEndMax - timeEndMin));
}

int Section::getIndexParameter(int k, int indexMax)
{
    return static_cast<int>(indexMax * (parameters[k] - parametersMin[k]) / (parametersMax[k] - parametersMin[k]));
}

void Section::interpolateX0(Section section)
{
    if (section.times.size() > 1) // Times array with at least 2 elements
    {
        double time0, time1;
        state_type step0, step1;

        for (size_t i = 1; i < section.times.size(); i++)
        {
            if (section.times[i - 1] <= timeStart && timeStart <= section.times[i])
            {
                time0 = section.times[i - 1];
                time1 = section.times[i];

                step0 = section.steps[i - 1];
                step1 = section.steps[i];

                break;
            }
        }

        for (size_t i = 0; i < x0.size(); i++)
        {
            x0[i] = step0[i] + (timeStart - time0) * (step1[i] - step0[i]) / (time1 - time0);
        }
    }
    else // Times array with only 1 element
    {
        x0 = section.steps.back();
    }
}

void Section::setAbscissaOrdinate()
{
    abscissa = QVector<double>(times.begin(), times.end());

    ordinate.clear();

    for (unsigned long i = 0; i < steps[0].size(); i++)
    {
        QVector<double> v;

        for (unsigned long j = 0; j < steps.size(); j++)
        {
            v.push_back(steps[j][i]);
        }

        ordinate.push_back(v);
    }
}

void Section::setAbscissaOrdinate(double time)
{
    unsigned long index = 0;

    for (unsigned long i = 1; i < times.size(); i++)
    {
        if (times[i - 1] <= time && time <= times[i])
        {
            index = i;
            break;
        }
    }

    abscissaLeft.clear();
    abscissaRight.clear();

    for (unsigned long i = 0; i < index; i++)
    {
        abscissaLeft.push_back(times[i]);
    }

    if (index > 0)
    {
        abscissaLeft.push_back(time);
    }

    for (unsigned long i = index - 1; i < times.size(); i++)
    {
        abscissaRight.push_back(times[i]);
    }

    ordinateLeft.clear();
    ordinateRight.clear();

    for (unsigned long i = 0; i < steps[0].size(); i++)
    {
        QVector<double> v;

        for (unsigned long j = 0; j < index; j++)
        {
            v.push_back(steps[j][i]);
        }

        if (index > 0)
        {
            double interpolatedStep = steps[index - 1][i] + (time - times[index - 1]) * (steps[index][i] - steps[index - 1][i]) / (times[index] - times[index - 1]);
            v.push_back(interpolatedStep);
        }

        ordinateLeft.push_back(v);

        QVector<double> w;

        for (unsigned long j = index - 1; j < steps.size(); j++)
        {
            w.push_back(steps[j][i]);
        }

        ordinateRight.push_back(w);
    }
}
