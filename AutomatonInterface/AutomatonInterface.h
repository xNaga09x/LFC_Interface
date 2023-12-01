#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AutomatonInterface.h"

class AutomatonInterface : public QMainWindow
{
    Q_OBJECT

public:
    AutomatonInterface(QWidget *parent = nullptr);
    ~AutomatonInterface();

private:
    Ui::AutomatonInterfaceClass ui;
};
