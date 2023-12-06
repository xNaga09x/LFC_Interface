#include "TextBox.h"

TextBox::TextBox(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Input Dialog");

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setPlaceholderText("Introduceți litera:");

    QPushButton* okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &TextBox::accept);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_lineEdit);
    layout->addWidget(okButton);

    setLayout(layout);
}

QString TextBox::getEnteredText()
{
    return m_lineEdit->text();
}
