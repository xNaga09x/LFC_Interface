#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

class TextBox : public QDialog
{
    Q_OBJECT

public:
    TextBox(QWidget* parent = nullptr);

    QString getEnteredText();

private:
    QLineEdit* m_lineEdit;
};

