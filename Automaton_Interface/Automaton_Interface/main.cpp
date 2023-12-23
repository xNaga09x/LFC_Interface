#include "Automaton_Interface.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Automaton_Interface w;
    w.showAutomatonTypeDialog(); //popup pentru tipul automatului
    w.show();
    return a.exec();
}
