#include "Automaton_Interface.h"

Automaton_Interface::Automaton_Interface(QWidget* parent)
    : QMainWindow(parent)
{
    deleteMode = false;
    ui.setupUi(this);
    firstNode = nullptr;
    orientedGraph = false;
    nodeIsBeingDragged = false;
    draggedNode = nullptr;
    lastMousePos;
    //connect(ui.deleteButton, &QPushButton::clicked, this, &AutomatonInterface::onButtonClicked);
}

Automaton_Interface::~Automaton_Interface()
{}
void Automaton_Interface::toggleDeleteMode()
{
    deleteMode = !deleteMode;
    // Actualizarea vizuala a butonul sau indicatorul pentru a reflecta starea curent? (folosind, de exemplu, setStyleSheet)
}
//Aceste functioneaza asa:Daca apesi pe el,pe orice nod pe care apesi click stanga se sterge,cu tot cu arcele asociate,iar
//daca apesi inca o data pe buton acesta is dezactiveaza functia
void Automaton_Interface::on_deleteButton_clicked()
{
    toggleDeleteMode();
}

void Automaton_Interface::mouseReleaseEvent(QMouseEvent* e)
{
    if (nodeIsBeingDragged) {
        nodeIsBeingDragged = false;
        draggedNode = nullptr;
    }
    if (e->button() == Qt::RightButton)
    {
        // Verifica?i suprapunerea cu nodurile existente
        bool overlapping = false;
        for (Node* n : graf.getNodes())
        {
            Node tempNode;
            tempNode.setCoordinate(e->pos()); // Ini?ializare coordonate
            tempNode.setValue(10); // Ini?ializare raz? (în acest exemplu, 10 de unit??i)

            if (n->isOverlapping(tempNode))
            {
                overlapping = true;
                break;
            }
        }

        if (!overlapping)
        {
            graf.addNode(e->pos());
            update();
        }
    }
    else if (e->button() == Qt::LeftButton)
    {
        //sa verific daca exista nod
        std::vector<Node*> nodes = graf.getNodes();
        for (Node* n : nodes)
        {
            //verific daca fac click oriunde in cercul respectiv
            //|xn-xc|<10 (xnod -xclick in modul)
            //|yn-yc|<10
            if (abs(e->pos().x() - n->getX()) < 10 &&
                abs(e->pos().y() - n->getY()) < 10)
            {
                if (!firstNode)
                {
                    firstNode = n;
                }
                else
                {
                    QString label;
                    label = openTextBox();
                    // Aici se face verificarea pentru existenta unui arc cu acela?i sens între aceleasi doua noduri
                    if (!graf.arcExists(firstNode, n))
                    {
                        graf.addArch(firstNode, n, label);
                    }
                    else
                    {
                        // Adaugare arc care se intoarce la acelasi nod
                        graf.addArch(n, firstNode,label);
                    }
                    firstNode = nullptr;
                }
                update();
                break;
            }
        }
    }
}

void Automaton_Interface::paintEvent(QPaintEvent* e)//aici creeam noduri
{
    QPainter p(this);
    std::vector<Node*>& noduri = graf.getNodes();
    for (Node* n : noduri)
    {
        QRect r(n->getX() - 10, n->getY() - 10, 20, 20);//xr=xnode-10 yr=yn-10 si dim dreptunghiului
        p.drawEllipse(r);
        QString s;
        // s.append('q');
        // s.append(s.setNum(n->getValue()));
         //s.setNum(n->getValue());//transforma un nr in string
        s = "q" + QString::number(n->getValue());
        p.drawText(r, Qt::AlignCenter, s);
    }
    std::vector<Arch*>& arches = graf.getArches();
    for (Arch* a : arches)//desenam linie intre coordonatele primului si al doilea nod
        if (a->getFirstNode() == a->getSecondNode())
        {
            //Rotirea unui arc in jurul nodului
            //Varianata 1
            //double radius = 20.0; // Ajusteaz? raza cercului
            //double arrowSize = 10.0; // Ajusteaz? dimensiunea s?ge?ii

            //QPointF center = a->getFirstNode()->getCoordinate();

            //// Creeaz? un traseu pentru cerc
            //QPainterPath circlePath;
            //circlePath.addEllipse(center, radius, radius);

            //// Deseneaz? cercul
            //p.drawPath(circlePath);

            //// Calculeaz? coordonatele punctelor pentru s?geat?
            //double arrowAngle = 0.0; // Unghiul s?ge?ii (0 rad = la dreapta)
            //QPointF arrowP1 = QPointF(center.x() + radius * cos(arrowAngle - M_PI / 6),
            //    center.y() + radius * sin(arrowAngle - M_PI / 6));
            //QPointF arrowP2 = QPointF(center.x() + radius * cos(arrowAngle + M_PI / 6),
            //    center.y() + radius * sin(arrowAngle + M_PI / 6));

            //// Deseneaz? s?geata la cap?tul cercului
            //p.drawLine(center, arrowP1);
            //p.drawLine(center, arrowP2);
            // 
            // 
            // Varianata 2
            //double radius = 20.0; // Ajusteaz? raza cercului
            //double arrowSize = 10.0; // Ajusteaz? dimensiunea s?ge?ii

            //QPointF center = a->getFirstNode()->getCoordinate();

            //// Calculeaz? coordonatele punctelor pentru s?geata
            //double arrowAngle = M_PI / 6; // Unghiul s?ge?ii (30 grade)
            //QPointF arrowP1 = QPointF(center.x() + radius * cos(arrowAngle - M_PI / 6),
            //    center.y() + radius * sin(arrowAngle - M_PI / 6));
            //QPointF arrowP2 = QPointF(center.x() + radius * cos(arrowAngle + M_PI / 6),
            //    center.y() + radius * sin(arrowAngle + M_PI / 6));

            //// Deseneaz? s?geata c?tre arc
            //p.drawLine(center, arrowP1);
            //p.drawLine(center, arrowP2);

            //// Salv?m starea curent? a dispozitivului de desenare
            //p.save();

            //// Deseneaz? arcul cu rota?ie
            //p.translate(center.x(), center.y());
            //p.rotate(180 * arrowAngle / M_PI); // Rotirea la un unghi corespunz?tor

            //p.drawArc(QRectF(-radius, -radius, 2 * radius, 2 * radius),
            //    30 * 16, 120 * 16); // 30 * 16 reprezint? unghiul de start, iar 120 * 16 reprezint? unghiul total al arcului

            //p.restore(); // Restaur?m starea dispozitivului de desenare la starea salvat? anterior

            //// Calculeaz? coordonatele punctelor pentru s?geata care se întoarce
            //double arrowReturnAngle = 210 * M_PI / 180; // Unghiul pentru s?geata care se întoarce (210 grade)
            //QPointF arrowReturnP1 = QPointF(center.x() + radius * cos(arrowReturnAngle - M_PI / 6),
            //    center.y() + radius * sin(arrowReturnAngle - M_PI / 6));
            //QPointF arrowReturnP2 = QPointF(center.x() + radius * cos(arrowReturnAngle + M_PI / 6),
            //    center.y() + radius * sin(arrowReturnAngle + M_PI / 6));

            //// Deseneaz? s?geata care se întoarce
            //p.drawLine(center, arrowReturnP1);
            //p.drawLine(center, arrowReturnP2);


        }
        else
        {
            p.drawLine(a->getFirstNode()->getCoordinate(), a->getSecondNode()->getCoordinate());
            //Daca se doreste graf orientat
            double angle = atan2(a->getSecondNode()->getY() - a->getFirstNode()->getY(),
                a->getSecondNode()->getX() - a->getFirstNode()->getX());
            double arrowSize = 10.0; // Ajustarea dimensiunii sagetii dupa preferinta

            // Calcularea punctelor pentru capul sagetii
            QPointF arrowP1 = QPointF(a->getSecondNode()->getX() - arrowSize * cos(angle - M_PI / 6),
                a->getSecondNode()->getY() - arrowSize * sin(angle - M_PI / 6));
            QPointF arrowP2 = QPointF(a->getSecondNode()->getX() - arrowSize * cos(angle + M_PI / 6),
                a->getSecondNode()->getY() - arrowSize * sin(angle + M_PI / 6));

            // Desenarea capului sagetii
            p.drawLine(a->getSecondNode()->getX(), a->getSecondNode()->getY(), arrowP1.x(), arrowP1.y());
            p.drawLine(a->getSecondNode()->getX(), a->getSecondNode()->getY(), arrowP2.x(), arrowP2.y());

            //Calcularea mijlocului arcului
            QPointF middle = (a->getFirstNode()->getCoordinate() + a->getSecondNode()->getCoordinate()) / 2.0;
            //Desenarea elementelor in mijlocul arcului
            QStringList elements = a->getElements();
            int elementY = middle.y() - 5;


            //Aici am pus doar indexul fiecarui arc(se vor pune mai incolon elementle din vector)
            //for (const QString& element : elements)
            //{
                //QRect reprezinta un dreptunghi in coordonatele dreptunghiului de pe ecran
                //Reprezinta pozitia unui obiect intr-o fereastra(acesta il folosesc pentru a insera elementele)
            int index = std::distance(arches.begin(), std::find(arches.begin(), arches.end(), a));
            QRect elementRect(middle.x() - 5, elementY + 15, 10, 10);//crearea patratului coordonatele de pe stanga si dreapta,latimea si inaltimea dreptunghiului
            p.drawText(elementRect, Qt::AlignCenter, a->getLabel());//element);
            elementY += 15;//Spatiile intre elemente
            //}
        }
    if (firstNode)
    {
        QRect r(firstNode->getX() - 10, firstNode->getY() - 10, 20, 20);//xr=xnode-10 yr=yn-10 si dim dreptunghiului
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(2);
        p.setPen(pen);
        p.drawEllipse(r);
        QString s;
        //s.setNum(firstNode->getValue());//transforma un nr in string
        s = "q" + QString::number(firstNode->getValue());
        p.drawText(r, Qt::AlignCenter, s);
    }
}

void Automaton_Interface::mousePressEvent(QMouseEvent* e) {
    if (deleteMode && e->button() == Qt::LeftButton) {
        std::vector<Node*> nodes = graf.getNodes();

        for (Node* n : nodes) {
            if (abs(e->pos().x() - n->getX()) < 10 && abs(e->pos().y() - n->getY()) < 10) {
                // Stergerea nodului si arcelor legate de el
                graf.removeNode(n);
                update();
                break;
            }
        }
    }
    else
        if (e->button() == Qt::LeftButton) {
            std::vector<Node*> nodes = graf.getNodes();

            for (Node* n : nodes) {
                if (abs(e->pos().x() - n->getX()) < 10 && abs(e->pos().y() - n->getY()) < 10) {
                    nodeIsBeingDragged = true;
                    draggedNode = n;
                    lastMousePos = e->pos();
                    break;
                }
            }
        }
}

void Automaton_Interface::mouseMoveEvent(QMouseEvent* e) {
    if (nodeIsBeingDragged) {
        // Daca un nod este in miscare, actualizati pozitia acestuia
        int dx = e->pos().x() - lastMousePos.x();
        int dy = e->pos().y() - lastMousePos.y();

        draggedNode->setCoordinate(draggedNode->getCoordinate() + QPoint(dx, dy));
        lastMousePos = e->pos();
        update();  // Redesenati fereastra pentru a reflecta noua pozitie
    }
}

QString Automaton_Interface::openTextBox()
{
    TextBox inputDialog;
    QString labelEnt;
    if (inputDialog.exec() == QDialog::Accepted)
    {
        labelEnt = inputDialog.getEnteredText();
    }
    return labelEnt;
}