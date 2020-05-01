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

#ifndef WIDGET_H
#define WIDGET_H

#include "models.h"
#include "section.h"
#include "qcustomplot.h"
#include <vector>
#include <boost/numeric/odeint.hpp>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QDoubleValidator>
#include <QLineEdit>
#include <QSlider>
#include <QGroupBox>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    std::vector<Section> sections;

    double timeMin;
    double timeMax;

    QComboBox *modelComboBox;

    QVBoxLayout *mainControlsVBoxLayout;
    QPushButton *addSectionPushButton;
    QPushButton *removeSectionPushButton;
    QComboBox *sectionComboBox;
    QDoubleValidator *timeStartDoubleValidator;
    QDoubleValidator *timeEndDoubleValidator;
    QLineEdit *timeStartLineEdit;
    QLineEdit *timeEndLineEdit;
    QSlider *timeStartSlider;
    QSlider *timeEndSlider;
    QVBoxLayout *parameterVBoxLayout;
    std::vector<QLineEdit*> parameterLineEdit;
    std::vector<QDoubleValidator*> parameterDoubleValidator;
    std::vector<QSlider*> parameterSlider;

    std::vector<QCustomPlot*> plots;
    QTabWidget *graphsTabWidget;
    Qt::GlobalColor colors[14];

    std::vector<QCustomPlot*> graphs;

    void onTimeStartLineEditReturnPressed();
    void onTimeEndLineEditReturnPressed();
    void onTimeStartSliderValueChanged(int value);
    void onTimeEndSliderValueChanged(int value);
    void onParameterLineEditReturnPressed(int index);
    void onParameterSliderValueChanged(int index, int value);

    void deleteParameterControls();
    void constructParameterControls(int index);

    void deletePlots();
    void constructPlots(int index);
    void setPlots();

    void deleteSections();

    void setTimeStartMinMax(int index);
    void setTimeEndMinMax(int index);

    void selectSection(int index);
    void addSection();
    void removeSection();

    void integrate(bool interpolation);
};

#endif // WIDGET_H
