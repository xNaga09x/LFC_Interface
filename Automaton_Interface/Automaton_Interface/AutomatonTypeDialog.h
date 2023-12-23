#pragma once

#include <QDialog>
#include "ui_AutomatonTypeDialog.h"
#include "AutomatonType.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include <QDialogButtonBox>

class AutomatonTypeDialog : public QDialog
{
	Q_OBJECT

public:
    AutomatonTypeDialog(QWidget* parent = nullptr);
    ~AutomatonTypeDialog();

    AutomatonType getSelectedAutomatonType() const;

private:
    QRadioButton* afdRadioButton;
    QRadioButton* afnRadioButton;
    QRadioButton* afnlRadioButton;
    QRadioButton* apdRadioButton;
};
