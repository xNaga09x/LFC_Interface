#include "WordBox.h"
WordBox::WordBox(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Input Dialog");

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setPlaceholderText("Introduceti cuvantul:");

    QPushButton* okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &WordBox::accept);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_lineEdit);
    layout->addWidget(okButton);

    setLayout(layout);
    connect(m_lineEdit, &QLineEdit::returnPressed, this, &WordBox::onReturnPressed);
}

QString WordBox::getEnteredText()
{
    if (m_lineEdit->text() == "`")return "\u03BB";
    return m_lineEdit->text();
}