#include "AutomatonTypeDialog.h"

AutomatonTypeDialog::~AutomatonTypeDialog()
{
    /* EMPTY */
}

AutomatonTypeDialog::AutomatonTypeDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Select Automaton Type");

    QVBoxLayout* layout = new QVBoxLayout(this);

    afdRadioButton = new QRadioButton("AFD", this);
    afnRadioButton = new QRadioButton("AFN", this);
    afnlRadioButton = new QRadioButton("AFNL", this);
    apdRadioButton = new QRadioButton("APD", this);

    layout->addWidget(afdRadioButton);
    layout->addWidget(afnRadioButton);
    layout->addWidget(afnlRadioButton);
    layout->addWidget(apdRadioButton);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AutomatonTypeDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AutomatonTypeDialog::reject);

    layout->addWidget(buttonBox);
}

AutomatonType AutomatonTypeDialog::getSelectedAutomatonType() const
{
    if (afdRadioButton->isChecked()) return AutomatonType::AFDType;
    if (afnRadioButton->isChecked()) return AutomatonType::AFNType;
    if (afnlRadioButton->isChecked()) return AutomatonType::AFNLType;
    if (apdRadioButton->isChecked()) return AutomatonType::APDType;

    return AutomatonType::AFDType;
}
