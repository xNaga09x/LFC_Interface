#include "Automaton_Interface.h"
#include "qdebug.h" //for testing to be removed

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
	automatonType = -1;
	finalMode = false;
	deleteMode = false;
	dragMode = false;
	//connect(ui.deleteButton, &QPushButton::clicked, this, &AutomatonInterface::onButtonClicked);
}

Automaton_Interface::~Automaton_Interface()
{}

void Automaton_Interface::on_deleteButton_clicked()
{
	deleteMode = !deleteMode;
}

void Automaton_Interface::on_finalButton_clicked()
{
	finalMode = !finalMode;
}

void Automaton_Interface::on_dragButton_clicked()
{
	dragMode = !dragMode;
}

void Automaton_Interface::on_addFromFileButton_clicked()
{
	//Deschide file explorer-ul cu QFileDialog si pune absolute path-ul file-ului ales intr-un QString
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Automaton"), "C:/", tr("Image Files(*.txt *.in)"));
	//Transforma QString to std::string
	std::string stringFileName = fileName.toStdString();
	//Deschide fisierul ales si citeste din el
	std::ifstream file(stringFileName);
	uint32_t type;
	file >> type;
	switch (type)
	{
	case 0: //AFD
		automatonType = 0;
		automaton.Read(file);
	}
	//In functie de type se apeleaza crearea obiectului automat
	//Deseneaza automatul citit pe ecran
	file.close();
}

void Automaton_Interface::on_saveToFileButton_clicked()
{
	//Deschide file explorer-ul cu QFileDialog si pune absolute path-ul file-ului ales intr-un QString
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Automaton"), "C:/", tr("Image Files(*.txt *.in)"));
	//Transforma QString to std::string
	std::string stringFileName = fileName.toStdString();
	//Deschide fisierul ales si salveaza automatul in el
	std::ofstream file(stringFileName);
	automaton.printToFile(file);
	file.close();
}

void Automaton_Interface::on_afdRadioButton_clicked()
{
	automatonType = 1;
	//momentan o sa cred utilizatorul pe cuvant cand zice ca a desenat un afd
	//dar putem implementa si verificari inainte sa se creeze obiectul
	std::vector<int> Q;
	std::vector<char> Sum;
	std::vector<std::tuple<int, char, int>> Delta;
	uint16_t q0;
	std::vector<int> F;

	std::vector<Node*> nodes = graf.getNodes();
	q0 = nodes[0]->getValue(); //se schimba cand implementam modificarea starii initiale/finale
	//menu pop up right click on node in care setezi campurile de isInitialState/ isFinalState
	F.emplace_back(q0); //dummy for testing, remake after menu
	for (auto& node : nodes)
	{
		Q.emplace_back(node->getValue());
	}
	std::vector<Arch*> arches = graf.getArches();
	uint32_t startState;
	char transitionSymbol;
	uint32_t finalState;
	for (auto& arch : arches)
	{
		//firstNode -> starea curenta a automatului
		//label -> simbolul citit de pe banda de intrare
		//secondNode -> starea in care ajunge automatul dupa citire
		startState = arch->getFirstNode()->getValue();
		transitionSymbol = arch->getLabel().toStdString()[0];
		finalState = arch->getSecondNode()->getValue();
		if (std::find(Sum.begin(), Sum.end(), transitionSymbol) == Sum.end())
			Sum.emplace_back(transitionSymbol);
		Delta.emplace_back(std::make_tuple(startState, transitionSymbol, finalState));
	}
	automaton.setDelta(Delta);
	automaton.setSum(Sum);
	automaton.setQ(Q);
	automaton.setF(F);
	automaton.setq0(q0);
	automaton.setSizeDelta(Delta.size());
	automaton.setSizeF(F.size());
	automaton.setSizeQ(Q.size());
	automaton.setSizeSum(Sum.size());
}

void Automaton_Interface::on_afnRadioButton_clicked()
{
	automatonType = 2;
}

void Automaton_Interface::on_afnlRadioButton_clicked()
{
	automatonType = 3;
}

void Automaton_Interface::on_apdRadioButton_clicked()
{
	automatonType = 4;
}

void Automaton_Interface::mouseReleaseEvent(QMouseEvent* e)
{
	//if (nodeIsBeingDragged) {
	//	nodeIsBeingDragged = false;
	//	draggedNode = nullptr;
	//}
	if (e->button() == Qt::RightButton)
	{
		// Verificati suprapunerea cu nodurile existente
		bool overlapping = false;
		for (Node* n : graf.getNodes())
		{
			Node tempNode;
			tempNode.setCoordinate(e->pos()); // Initializare coordonate
			tempNode.setValue(10); // Initializare raza (in acest exemplu, 10 de unitati)

			if (n->isOverlapping(tempNode))
			{
				overlapping = true;
				break;
			}
		}

		if (!overlapping)
		{
			graf.addNode(e->pos());
			if (graf.getNodes().size() == 1)
			{
				for (int i = 0; i < graf.getNodes().size(); i++)
					graf.getNodes()[0]->setInitialState(true);
			}
			update();
		}
	}
	else if (e->button() == Qt::LeftButton)
	{
		//sa verific daca exista nod
		std::vector<Node*> nodes = graf.getNodes();
		for (Node* n : nodes)
		{
			if (abs(e->pos().x() - n->getX()) < 10 && abs(e->pos().y() - n->getY()) < 10 && deleteMode==false && finalMode==false &&dragMode==false)
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
						graf.addArch(n, firstNode, label);
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
		QRect outerRect(n->getX() - 15, n->getY() - 15, 30, 30); // Dimensiunile cercului exterior
		QRect r(n->getX() - 10, n->getY() - 10, 20, 20);//xr=xnode-10 yr=yn-10 si dim dreptunghiului

		if (n->getFinalState())
		{
			QPen outerPen(Qt::black); // Culoarea și grosimea conturului cercului exterior
			outerPen.setWidth(2);
			p.setPen(outerPen);
			p.drawEllipse(outerRect);
		}

		if (n->getInitialState())
		{
			//Desenarea sagetii pentru nodul initial
			QPen initialPen(Qt::black);
			initialPen.setWidth(2);
			p.setPen(initialPen);

			QPointF aux(n->getX()-20, n->getY()-20);
			QPointF aux2(n->getX()-7.5, n->getY()-7.5);
			p.drawLine(aux, aux2);

			double arrowSize = 10.0;
			double arrowAngle = atan2(aux2.y() - aux.y(), aux2.x() - aux.x());

			QPointF arrowP1 = QPointF(aux2.x() - arrowSize * cos(arrowAngle + M_PI / 6),
				aux2.y() - arrowSize * sin(arrowAngle + M_PI / 6));
			QPointF arrowP2 = QPointF(aux2.x() - arrowSize * cos(arrowAngle - M_PI / 6),
				aux2.y() - arrowSize * sin(arrowAngle - M_PI / 6));
			p.drawLine(aux2, arrowP1);
			p.drawLine(aux2, arrowP2);
		}

		QPen innerPen(Qt::black); // Culoarea și grosimea conturului cercului interior
		innerPen.setWidth(2);
		p.setPen(innerPen);
		p.drawEllipse(r);

		QString s = "q" + QString::number(n->getValue());
		p.drawText(r, Qt::AlignCenter, s);
	}
	std::vector<Arch*>& arches = graf.getArches();
	for (Arch* a : arches)//desenam linie intre coordonatele primului si al doilea nod
		if (a->getFirstNode()->getValue() == a->getSecondNode()->getValue())
		{
			//Cazul in care avem arc de la un nod la el insusi
			Node* n = a->getFirstNode();

			//Setare dimensiuni linii
			double firstLineLength = 30.0;
			p.drawLine(QPointF(n->getX(), n->getY()-10),
				QPointF(n->getX(), n->getY() - firstLineLength));

			double secondLineLength = 30.0;
			p.drawLine(QPointF(n->getX(), n->getY() - firstLineLength),
				QPointF(n->getX() - secondLineLength, n->getY() - firstLineLength));
			QPointF l1(n->getX(), n->getY() - firstLineLength);
			QPointF l2(n->getX() - secondLineLength, n->getY() - firstLineLength);

			p.drawLine(QPointF(n->getX() - secondLineLength, n->getY() - firstLineLength),
				QPointF(n->getX() - secondLineLength, n->getY()));

			p.drawLine(QPointF(n->getX() - secondLineLength, n->getY()),
				QPointF(n->getX()-10, n->getY()));

			QPointF lineStart = QPointF(n->getX() - secondLineLength, n->getY());
			QPointF lineEnd = QPointF(n->getX() - 10, n->getY());

			// Desenează săgeata la capătul liniei
			double arrowSize = 10.0; // Ajustează dimensiunea săgeții
			double arrowAngle = M_PI / 6.0; // Ajustează unghiul săgeții

			QPointF arrowP1 = QPointF(lineEnd.x() - arrowSize * cos(arrowAngle),
				lineEnd.y() - arrowSize * sin(arrowAngle));
			QPointF arrowP2 = QPointF(lineEnd.x() - arrowSize * cos(-arrowAngle),
				lineEnd.y() - arrowSize * sin(-arrowAngle));
			p.drawLine(lineEnd, arrowP1);
			p.drawLine(lineEnd, arrowP2);

			//Desenare label
			QPointF middle = (l1 + l2) / 2.0;
			QStringList elements = a->getElements();
			int elementY = middle.y() - 5;
			int index = std::distance(arches.begin(), std::find(arches.begin(), arches.end(), a));
			QRect elementRect(middle.x() - 5, elementY + 15, 10, 10);//crearea patratului coordonatele de pe stanga si dreapta,latimea si inaltimea dreptunghiului
			p.drawText(elementRect, Qt::AlignCenter, a->getLabel());//element);
			elementY += 15;//Spatiile intre elemente
		}
		else
		{
			//Cazul in care avem arc intre 2 noduri diferite
			double angle = atan2(a->getSecondNode()->getY() - a->getFirstNode()->getY(),
				a->getSecondNode()->getX() - a->getFirstNode()->getX());
			double arrowSize = 10.0; // Ajustarea dimensiunii sagetii dupa preferinta

			QPointF start = QPointF(a->getFirstNode()->getX() + cos(angle) * (arrowSize),
				a->getFirstNode()->getY() + sin(angle) * (arrowSize));
			QPointF end = QPointF(a->getSecondNode()->getX() - cos(angle) * (arrowSize),
				a->getSecondNode()->getY() - sin(angle) * (arrowSize));

			p.drawLine(start, end);

			// Calcularea punctelor pentru capul săgeții
			QPointF arrowP1 = QPointF(end.x() - arrowSize * cos(angle - M_PI / 6),
				end.y() - arrowSize * sin(angle - M_PI / 6));
			QPointF arrowP2 = QPointF(end.x() - arrowSize * cos(angle + M_PI / 6),
				end.y() - arrowSize * sin(angle + M_PI / 6));

			// Desenarea capului săgeții
			p.drawLine(end, arrowP1);
			p.drawLine(end, arrowP2);

			//Calcularea mijlocului arcului
			QPointF middle = (a->getFirstNode()->getCoordinate() + a->getSecondNode()->getCoordinate()) / 2.0;
			//Desenarea elementelor in mijlocul arcului
			QStringList elements = a->getElements();
			int elementY = middle.y() - 5;

			//Aici am pus doar indexul fiecarui arc(se vor pune mai incolon elementle din vector)
			//QRect reprezinta un dreptunghi in coordonatele dreptunghiului de pe ecran
			//Reprezinta pozitia unui obiect intr-o fereastra(acesta il folosesc pentru a insera elementele)
			int index = std::distance(arches.begin(), std::find(arches.begin(), arches.end(), a));
			QRect elementRect(middle.x() - 5, elementY + 15, 10, 10);//crearea patratului coordonatele de pe stanga si dreapta,latimea si inaltimea dreptunghiului
			p.drawText(elementRect, Qt::AlignCenter, a->getLabel());//element);
			elementY += 15;//Spatiile intre elemente
			
			//for (Arch* arch : graf.getArches())
			//{
			//	if (arch->getFirstNode() == a->getSecondNode() && a->getFirstNode() == arch->getSecondNode())
			//	{
			//		QPointF mid=(a->getFirstNode()->getCoordinate() + a->getSecondNode()->getCoordinate()) / 2.0;
			//		QPointF aux(mid.x()-5,mid.y()-5);
			//		p.drawLine(a->getFirstNode()->getCoordinate(), aux);
			//		p.drawLine(aux, a->getSecondNode()->getCoordinate());
			//		QRect rectLabel(aux.x() - 10, aux.y() - 10, 10, 10);
			//		p.drawText(rectLabel , Qt::AlignCenter, a->getLabel());

			//		double angleA = atan2(a->getSecondNode()->getY() - aux.y(),
			//			a->getSecondNode()->getX() - aux.x());
			//		double arrowSizeA = 10.0;

			//		QPointF arrowP1A = QPointF(a->getSecondNode()->getX() - arrowSizeA * cos(angleA - M_PI / 6),
			//			end.y() - arrowSizeA * sin(angleA - M_PI / 6));
			//		QPointF arrowP2A = QPointF(a->getSecondNode()->getX() - arrowSize * cos(angleA + M_PI / 6),
			//			end.y() - arrowSizeA * sin(angleA + M_PI / 6));

			//		// Desenarea capului săgeții
			//		p.drawLine(a->getSecondNode()->getCoordinate(), arrowP1A);
			//		p.drawLine(a->getSecondNode()->getCoordinate(), arrowP2A);
			//	}
			//}
		}
	if (firstNode)
	{
		QRect r(firstNode->getX() - 10, firstNode->getY() - 10, 20, 20);
		QPen pen;
		pen.setColor(Qt::red);
		pen.setWidth(2);
		p.setPen(pen);
		p.drawEllipse(r);
		QString s;
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
		if (e->button() == Qt::LeftButton && deleteMode == false && finalMode == false && dragMode) {
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
		else
			if (e->button() == Qt::LeftButton && finalMode)
			{
				std::vector<Node*> nodes = graf.getNodes();

				for (Node* n : nodes) {
					if (abs(e->pos().x() - n->getX()) < 10 && abs(e->pos().y() - n->getY()) < 10) {
						// Setarea nodului ca nod final
						n->setFinalState(true);
						update();
						break;
					}
				}
			}
			else if (e->button() == Qt::LeftButton && dragMode)
			{
				std::vector<Node*> nodes = graf.getNodes();

				for (Node* n : nodes) {
					if (abs(e->pos().x() - n->getX()) < 10 && abs(e->pos().y() - n->getY()) < 10) {
						// Setarea nodului ca nod mutat
						draggedNode = n;
						update();
						break;
					}
				}
			}
}

void Automaton_Interface::mouseMoveEvent(QMouseEvent* e) {
	if (dragMode) {
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