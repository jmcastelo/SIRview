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

#ifndef PHASESPACEWIDGET_H
#define PHASESPACEWIDGET_H

#include "phasespacemodel.h"
#include "customvalidator.h"
#include <vector>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QVBoxLayout>
#include <QSlider>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QGridLayout>
#include <QStackedLayout>

class PhaseSpaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhaseSpaceWidget(QWidget *parent = nullptr);
    ~PhaseSpaceWidget();

private:
    PhaseSpaceModel *currentModel;
    std::vector<PhaseSpaceModel*> models;

    QComboBox *modelComboBox;

    QComboBox *xAxisComboBox;
    QComboBox *yAxisComboBox;

    QLineEdit *icGridDimensionLineEdit;

    QLineEdit *timeEndLineEdit;

    QVBoxLayout *parameterVBoxLayout;
    std::vector<QLineEdit*> parameterLineEdit;
    std::vector<QSlider*> parameterSlider;

    QStackedLayout *plotsStackedLayout;

    void initAxesComboBoxes();
    void updateAxisComboBox(QComboBox *axisComboBox, int variableIndex);

    void updateControls();

    void deleteParameterControls();
    void constructParameterControls();

    void onParameterLineEditReturnPressed(int index);
    void onParameterSliderValueChanged(int index, int value);

    void updateInitialConditions();

    void updateTimeEnd();
};

#endif // PHASESPACEWIDGET_H
