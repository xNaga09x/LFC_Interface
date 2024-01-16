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
#include <random>
#include <unordered_set>
#include <QLabel>
#include <QVBoxLayout>

class Automaton_Interface : public QMainWindow
{
    Q_OBJECT

public:
    Automaton_Interface(QWidget* parent = nullptr);
    ~Automaton_Interface();
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    QString openTextBox();
    QString openWordBox();
    void highlightNode(Node* node, QColor color);
    void highlightArc(Arch* arc, QColor color);
    void resetColors();
public slots:
    void on_deleteButton_clicked(); 
    void on_finalButton_clicked(); 
    void on_dragButton_clicked(); 
    void on_addFromFileButton_clicked(); 
    void on_saveToFileButton_clicked(); 
    void on_testWordButton_clicked();
    void on_verifyWordsButton_clicked();
    void showAutomatonTypeDialog();

private:
    Ui::Automaton_InterfaceClass ui;
    Node* firstNode;
    Graph graf;
    bool orientedGraph;
    bool nodeIsBeingDragged;  
    Node* draggedNode;       
    QPoint lastMousePos;      
    AutomatonType automatonType; 
    Automaton* automaton;
    Automaton* toDrawAutomaton; 
    APD automatonPD;
    APD toDrawAPD; 

    bool dragMode;
    bool finalMode;
    bool deleteMode;  

    std::unordered_set<int> currentStates;
    int currentIndex;
    bool wordAccepted;

    std::vector<Arch*> archAnimations;
    std::vector<Node*>nodeAnimations;
    bool nodeAnimation;
    bool archAnimation;

    QLabel* wordLabel;
};
