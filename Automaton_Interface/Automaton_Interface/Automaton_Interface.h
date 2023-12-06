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
    void toggleDeleteMode();  // Slot pentru activarea/dezactivarea modului de stergere
    void on_deleteButton_clicked();  // Slot pentru gestionarea evenimentului de clic pe buton


private:
    Ui::Automaton_InterfaceClass ui;
    Node* firstNode;
    Graph graf;
    bool orientedGraph;
    bool nodeIsBeingDragged;  // Folosit pentru a verifica daca un nod este în miscare
    Node* draggedNode;        // Nodul care este tras
    QPoint lastMousePos;      // Pozitia ultimei pozitii a mouse-ului in timpul tragerii
    bool deleteMode;  // Variabila pentru a tine evidenta starii de stergere
};
