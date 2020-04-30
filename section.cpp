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
