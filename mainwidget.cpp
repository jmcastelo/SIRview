#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent): QWidget(parent)
{
    QString aboutString;
    aboutString.append("SIRview - Solve and visualize SIR epidemiological models.\n\n");
    aboutString.append("Authors: J. M. Castelo & J. A. Oteo (Universidad de Valencia)\n");
    aboutString.append("Code by J. M. Castelo\n\n");
    aboutString.append("Contact email: jose.maria.castelo@gmail.com\n\n");
    aboutString.append("Copyright 2020 José María Castelo & José Ángel Oteo\n");
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

    scenarioWidget = new ScenarioWidget;

    mainTabWidget = new QTabWidget;
    mainTabWidget->setTabPosition(QTabWidget::North);
    mainTabWidget->addTab(scenarioWidget, "Scenario editing");
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
