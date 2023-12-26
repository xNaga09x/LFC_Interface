#pragma once

#include <QMainWindow>
#include "ui_AnimationInterface.h"

class AnimationInterface : public QMainWindow
{
	Q_OBJECT

public:
	AnimationInterface(QWidget *parent = nullptr);
	~AnimationInterface();

private:
	Ui::AnimationInterfaceClass ui;
};
