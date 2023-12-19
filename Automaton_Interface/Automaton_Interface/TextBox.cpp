#include "TextBox.h"

TextBox::TextBox(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Input Dialog");

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setPlaceholderText("Introduceți valoare:");

    QPushButton* okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &TextBox::accept);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_lineEdit);
    layout->addWidget(okButton);

    setLayout(layout);
}

QString TextBox::getEnteredText()
{
    if (m_lineEdit->text() == "`")return "\u03BB";
    return m_lineEdit->text();
}
