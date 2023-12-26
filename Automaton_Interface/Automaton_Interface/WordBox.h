#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
class WordBox : public QDialog
{
public:
    WordBox(QWidget* parent = nullptr);

    QString getEnteredText();
    void onReturnPressed()
    {
        accept(); // �nchide dialogul doar la ap?sarea tastei Enter
    }
private:
    QLineEdit* m_lineEdit;
	Q_OBJECT
};

