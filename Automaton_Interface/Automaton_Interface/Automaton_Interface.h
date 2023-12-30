#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Automaton_Interface.h"
#include "Automaton.h"
#include "AutomatonType.h"
#include "AutomatonTypeDialog.h"
#include "Graph.h"
#include <QKeyEvent>
#include <QMainWindow>
#include<algorithm>
#include <QMouseEvent>
#include <QFile>
#include <QTextEdit>
#include<QTextStream>
#include<QFileDialog>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QtMath>
#include<qmessagebox.h>
#include<QMessageBox>
#include<QPainterPath>
#include<QTransform>
#include "TextBox.h"
#include "WordBox.h"
#include <QFileDialog>
#include <QFile>
#include <fstream>
#include <vector>
#include <algorithm>
#include<set>
#include "AFD.h"
#include "AFN.h"
#include "AFN_lambda.h"
#include "APD.h"
#include<QTimer>
#include<QThread>
#include<QProgressDialog>
#include<stack>
class Automaton_Interface : public QMainWindow
{
    Q_OBJECT

public:
    Automaton_Interface(QWidget* parent = nullptr);
    ~Automaton_Interface();
    virtual void mouseReleaseEvent(QMouseEvent* e);//pui schimbarile facute de mouse
    virtual void paintEvent(QPaintEvent* e);//iar aici este de ce afiseaza
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    QString openTextBox();
    QString openWordBox();
    void highlightNode(Node* node, QColor color);
    void highlightArc(Arch* arc, QColor color);
    void resetColors();
public slots:
    void on_deleteButton_clicked();  // Slot pentru gestionarea evenimentului de clic pe buton
    void on_finalButton_clicked();  // Slot pentru gestionarea evenimentului de clic pe buton
    void on_dragButton_clicked();  // Slot pentru gestionarea evenimentului de clic pe buton
    void on_addFromFileButton_clicked(); // Slot pentru adaugarea unui automat dintr-un fisier
    void on_saveToFileButton_clicked(); // Slot pentru salvarea unui automat intr-un fisier
    void on_testWordButton_clicked();
    void on_verifyWordsButton_clicked();
    //Slot-uri pentru tipul obiectului
    void showAutomatonTypeDialog();

private:
    Ui::Automaton_InterfaceClass ui;
    Node* firstNode;
    Graph graf;
    bool orientedGraph;
    bool nodeIsBeingDragged;  // Folosit pentru a verifica daca un nod este în miscare
    Node* draggedNode;        // Nodul care este tras
    QPoint lastMousePos;      // Pozitia ultimei pozitii a mouse-ului in timpul tragerii
    AutomatonType automatonType; //enum pt tipul automatului (am inlocuit APDMode cu asta!)
    Automaton* automaton;
    Automaton* toDrawAutomaton; //pt read from file
    APD automatonPD;
    APD toDrawAPD; //pt read from file

    bool dragMode;
    bool finalMode;
    bool deleteMode;  // Variabila pentru a tine evidenta starii de stergere

    std::unordered_set<int> currentStates;
    int currentIndex;
    bool wordAccepted;

    std::vector<Arch*> archAnimations;
    std::vector<Node*>nodeAnimations;
    bool nodeAnimation;
    bool archAnimation;
};
