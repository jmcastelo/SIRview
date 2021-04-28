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

#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent): QWidget(parent)
{
    // About

    QString aboutString;
    aboutString.append("SIRview - Solve and visualize SIR epidemiological models.\n\n");
    aboutString.append("Authors: J. M. Castelo & J. A. Oteo (Universidad de Valencia)\n");
    aboutString.append("Code by J. M. Castelo\n\n");
    aboutString.append("Contact email: jose.maria.castelo@gmail.com\n\n");
    aboutString.append("Copyright 2021 Jose Maria Castelo & Jose Angel Oteo\n");
    aboutString.append("Released under GPLv3 license.");

    QLabel *aboutLabel = new QLabel;
    aboutLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    aboutLabel->setText(aboutString);

    QFont aboutFont = aboutLabel->font();
    aboutFont.setPointSize(21);
    aboutLabel->setFont(aboutFont);

    QVBoxLayout *aboutVBoxLayout = new QVBoxLayout;
    aboutVBoxLayout->addWidget(aboutLabel);
    aboutVBoxLayout->setAlignment(aboutLabel, Qt::AlignHCenter | Qt::AlignVCenter);

    QWidget *aboutWidget = new QWidget;
    aboutWidget->setLayout(aboutVBoxLayout);

    // Model widgets

    scenarioWidget = new ScenarioWidget;
    phaseSpaceWidget = new PhaseSpaceWidget;

    mainTabWidget = new QTabWidget;
    mainTabWidget->setTabPosition(QTabWidget::North);
    mainTabWidget->addTab(scenarioWidget, "Scenario editing");
    mainTabWidget->addTab(phaseSpaceWidget, "Phase-space diagram");
    mainTabWidget->addTab(aboutWidget, "About");

    QVBoxLayout *mainVBoxLayout = new QVBoxLayout;
    mainVBoxLayout->addWidget(mainTabWidget);

    setLayout(mainVBoxLayout);

    setWindowTitle("SIRview");
    resize(1200, 800);
}

MainWidget::~MainWidget()
{

}
