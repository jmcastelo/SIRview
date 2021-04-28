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

#ifndef SCENARIOGENERICMODEL_H
#define SCENARIOGENERICMODEL_H

#include "scenariomodel.h"

class ScenarioGenericModel: public ScenarioModel
{
public:
    ScenarioGenericModel(
        int index,
        QString modelName,
        std::list<QString> variableShortNamesList,
        std::list<QString> variableLongNamesList,
        std::list<QString> parameterNamesList,
        std::list<double> parameterMinList,
        std::list<double> parameterMaxList,
        std::list<double> parameterInitList,
        std::list<double> initialConditionsList,
        QWidget *parent = nullptr
        ): ScenarioModel(
            index,
            modelName,
            variableShortNamesList,
            variableLongNamesList,
            parameterNamesList,
            parameterMinList,
            parameterMaxList,
            parameterInitList,
            initialConditionsList,
            parent)
    {
        constructAdditionalPlots();
        connectPlots();
    }

private:
    void constructAdditionalPlots(){}
    void setAdditionalPlotsData(){}
};

#endif // SCENARIOGENERICMODEL_H
