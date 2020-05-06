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

#include "basemodel.h"

BaseModel::BaseModel(
    int index,
    QString modelName,
    std::list<QString> variableShortNamesList,
    std::list<QString> variableLongNamesList,
    std::list<QString> parameterNamesList,
    std::list<double> parameterMinList,
    std::list<double> parameterMaxList)
{
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

    dimension = static_cast<int>(variableShortNames.size());
    numParameters = static_cast<int>(parameterNames.size());
}

BaseModel::BaseModel(const BaseModel &bm)
{
    name = bm.name;

    dimension = bm.dimension;
    numParameters = bm.numParameters;

    for (size_t i = 0; i < bm.variableShortNames.size(); i++)
    {
        QLabel *label = new QLabel;
        label->setText(bm.variableShortNames[i]->text());
        variableShortNames.push_back(label);
    }

    for (size_t i = 0; i < bm.variableLongNames.size(); i++)
    {
        QLabel *label = new QLabel;
        label->setText(bm.variableLongNames[i]->text());
        variableLongNames.push_back(label);
    }

    for (size_t i = 0; i < bm.parameterNames.size(); i++)
    {
        QLabel *label = new QLabel;
        label->setText(bm.parameterNames[i]->text());
        parameterNames.push_back(label);
    }

    parameterMin = bm.parameterMin;
    parameterMax = bm.parameterMax;
}

BaseModel::~BaseModel()
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
}
