#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Automaton_Interface.h"
#include "Graph.h"
#include <QKeyEvent>
#include <QMainWindow>
#include<algorithm>
#include <QMouseEvent>
#include <QFile>
#include <QTextEdit>
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
#include <QFileDialog>
#include <QFile>
#include <fstream>
#include <vector>
#include <algorithm>

#include "AFD.h"

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
public slots:
    void on_deleteButton_clicked();  // Slot pentru gestionarea evenimentului de clic pe buton
    void on_finalButton_clicked();  // Slot pentru gestionarea evenimentului de clic pe buton
    void on_dragButton_clicked();  // Slot pentru gestionarea evenimentului de clic pe buton
    void on_addFromFileButton_clicked(); // Slot pentru adaugarea unui automat dintr-un fisier
    void on_saveToFileButton_clicked(); // Slot pentru salvarea unui automat intr-un fisier
    //Slot-uri pentru tipul obiectului (!poate puse intr-un pop up inainte de a incepe desenarea & facut enum pt type)
    void on_afdRadioButton_clicked();
    void on_afnRadioButton_clicked();
    void on_afnlRadioButton_clicked();
    void on_apdRadioButton_clicked();

private:
    Ui::Automaton_InterfaceClass ui;
    Node* firstNode;
    Graph graf;
    bool orientedGraph;
    bool nodeIsBeingDragged;  // Folosit pentru a verifica daca un nod este în miscare
    Node* draggedNode;        // Nodul care este tras
    QPoint lastMousePos;      // Pozitia ultimei pozitii a mouse-ului in timpul tragerii
    uint32_t automatonType; // Variabila pentru determinarea tipului de obiect generat (doar pt scriere in fisier)
    AFD automaton;

    bool dragMode;
    bool finalMode;
    bool deleteMode;  // Variabila pentru a tine evidenta starii de stergere
};
