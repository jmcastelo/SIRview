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

#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <list>
#include <QLabel>
#include <QString>

class BaseModel
{
public:
    QString name;

    int modelIndex;
    int dimension;
    int numParameters;

    std::vector<QLabel*> variableShortNames;
    std::vector<QLabel*> variableLongNames;
    std::vector<QLabel*> parameterNames;

    std::vector<QString> plotNames;

    std::vector<double> parameterMin;
    std::vector<double> parameterMax;

    BaseModel(
        int index,
        QString modelName,
        std::list<QString> variableShortNamesList,
        std::list<QString> variableLongNamesList,
        std::list<QString> parameterNamesList,
        std::list<double> parameterMinList,
        std::list<double> parameterMaxList);

    BaseModel(const BaseModel &bm);

    ~BaseModel();
};

#endif // BASEMODEL_H
