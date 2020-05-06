#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "scenariowidget.h"
#include <QWidget>
#include <QTabWidget>

class MainWidget: public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    ScenarioWidget *scenarioWidget;
    QTabWidget *mainTabWidget;
};

#endif // MAINWIDGET_H
