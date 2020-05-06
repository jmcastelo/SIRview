#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent): QWidget(parent)
{
    scenarioWidget = new ScenarioWidget;

    mainTabWidget = new QTabWidget;
    mainTabWidget->setTabPosition(QTabWidget::North);
    mainTabWidget->addTab(scenarioWidget, "Scenario editing");

    QVBoxLayout *mainVBoxLayout = new QVBoxLayout;
    mainVBoxLayout->addWidget(mainTabWidget);

    setLayout(mainVBoxLayout);

    setWindowTitle("SIRview");
    resize(1200, 800);
}

MainWidget::~MainWidget()
{

}
