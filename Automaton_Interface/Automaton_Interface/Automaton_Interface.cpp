#include "Automaton_Interface.h"
#include "qdebug.h" 

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
	finalMode = false;
	deleteMode = false;
	dragMode = false;
	currentStates.clear();
	currentIndex = 0;
	wordAccepted = false;
	nodeAnimation = false;
	archAnimation = false;
}

Automaton_Interface::~Automaton_Interface()
{
	delete automaton;
}

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

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Automaton"), "C:/", tr("Image Files(*.txt *.in)"));

	std::string stringFileName = fileName.toStdString();

	std::ifstream file(stringFileName);
	std::string type;
	file >> type;
	if (type == "AFD")
	{
		automatonType = AutomatonType::AFDType;
		toDrawAutomaton = new AFD;
		toDrawAutomaton->readAutomaton(file);
	}
	else if (type == "AFN")
	{
		automatonType = AutomatonType::AFNType;
		toDrawAutomaton = new AFN;
		toDrawAutomaton->readAutomaton(file);
	}
	else if (type == "AFNL")
	{
		automatonType = AutomatonType::AFNLType;
		toDrawAutomaton = new AFN_lambda;
		toDrawAutomaton->readAutomaton(file);
	}
	else if (type == "APD")
	{
		automatonType = AutomatonType::APDType;
		toDrawAPD.readAutomaton(file);
	}
	else exit(1);


	uint32_t x, y;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib_x(194, 583);
	std::uniform_int_distribution<> distrib_y(139, 362);
	std::unordered_set<int> coordsUsed_x;
	std::unordered_set<int> coordsUsed_y;
	if (automatonType != AutomatonType::APDType)
	{

		for (uint32_t i = 0; i < toDrawAutomaton->getQ().size(); i++)
		{
			do
			{
				x = distrib_x(gen);
				y = distrib_y(gen);
			} while (coordsUsed_x.count(x) > 0 && coordsUsed_y.count(y) > 0);

			QPoint pos(x, y);
			graf.addNode(pos);
			coordsUsed_x.insert(x);
			coordsUsed_y.insert(y);
			if (toDrawAutomaton->getQ()[i] == toDrawAutomaton->getq0())
				graf.getNodes()[i]->setInitialState(true);
		}
		std::vector<int> finalNodes = toDrawAutomaton->getF();
		std::vector<Node*> nodes = graf.getNodes();
		for (uint32_t i = 0; i < nodes.size(); i++)
		{
			if (std::find(finalNodes.begin(), finalNodes.end(), nodes[i]->getValue()) != finalNodes.end())
				nodes[i]->setFinalState(true);
		}

		std::vector<std::tuple<int, char, int>> transitions = toDrawAutomaton->getDelta();
		for (uint32_t i = 0; i < transitions.size(); i++)
		{
			uint32_t firstNode = std::get<0>(transitions[i]);
			char symbol = std::get<1>(transitions[i]);
			QString label(symbol);
			uint32_t secondNode = std::get<2>(transitions[i]);
			Node* first = graf.getNodeById(firstNode);
			Node* second = graf.getNodeById(secondNode);
			graf.addArch(first, second, label);

		}
	}
	else
	{
		for (uint32_t i = 0; i < toDrawAPD.getQ().size(); i++)
		{
			do
			{
				x = distrib_x(gen);
				y = distrib_y(gen);
			} while (coordsUsed_x.count(x) > 0 && coordsUsed_y.count(y) > 0);

			QPoint pos(x, y);
			graf.addNode(pos);
			coordsUsed_x.insert(x);
			coordsUsed_y.insert(y);
			if (toDrawAPD.getQ()[i] == toDrawAPD.getq0())
				graf.getNodes()[i]->setInitialState(true);
		}
		std::vector<int> finalNodes = toDrawAPD.getF();
		std::vector<Node*> nodes = graf.getNodes();
		for (uint32_t i = 0; i < nodes.size(); i++)
		{
			if (std::find(finalNodes.begin(), finalNodes.end(), nodes[i]->getValue()) != finalNodes.end())
				nodes[i]->setFinalState(true);
		}

		std::vector<Prod> transitions = toDrawAPD.getDelta();
		for (uint32_t i = 0; i < transitions.size(); i++)
		{
			std::vector<QString> labels;
			uint32_t firstNode = transitions[i].getInitialState();
			char symbol = transitions[i].getAlphabet();
			QString label1(symbol);
			labels.emplace_back(label1);
			char stackSymbol = transitions[i].getTopStack();
			QString label2(stackSymbol);
			labels.emplace_back(label2);
			uint32_t secondNode = transitions[i].getFinalState();
			std::string overstack = transitions[i].getOverStack();
			QString label3 = QString::fromUtf8(overstack.c_str());
			labels.emplace_back(label3);
			Node* first = graf.getNodeById(firstNode);
			Node* second = graf.getNodeById(secondNode);
			graf.addAPDArch(first, second, labels);

		}
	}
	update();
	file.close();
}

void Automaton_Interface::on_saveToFileButton_clicked()
{

	QString fileName = QFileDialog::getOpenFileName(this, tr("Choose save file"), "C:/", tr("Image Files(*.txt *.in)"));

	std::string stringFileName = fileName.toStdString();
	std::ofstream file(stringFileName);
	if (automatonType != AutomatonType::APDType)
		automaton->printAutomaton(file);
	else automatonPD.printAutomaton(file);
	file.close();
}

void Automaton_Interface::highlightNode(Node* node, QColor color)
{
	node->color = color;
	update(); 
}

void Automaton_Interface::highlightArc(Arch* arc, QColor color)
{

	arc->color = color;
	update();
}

void Automaton_Interface::resetColors()
{

	for (Node* node : graf.getNodes())
		node->color = Qt::black;

	for (Arch* arc : graf.getArches())
		arc->color = Qt::black;

	update();
}

void Automaton_Interface::on_testWordButton_clicked()
{
	QString label = openWordBox();
	if (!label.isEmpty())
	{
		

		if (automatonType == AutomatonType::AFDType)
		{

			std::vector<Arch*>& arches = graf.getArches();
			for (int i = 0; i < arches.size(); i++)
				currentStates.insert(i);
			currentIndex = 0;
			auto closestResults = dynamic_cast<AFD*>(automaton)->checkWordDetails(currentStates, label.toStdString(), currentIndex);
			nodeAnimations.clear();
			archAnimations.clear();
			for (Node* node : closestResults.first)
			{
				nodeAnimations.push_back(node);
			}

			for (Arch* arch : closestResults.second)
			{
				archAnimations.push_back(arch);
			}
			nodeAnimation = !nodeAnimation;
			archAnimation = !archAnimation;
			for (int i = 0; i < arches.size(); i++)
				currentStates.insert(i);
			std::string word = label.toStdString();
			currentIndex = 0;

			bool verify = automaton->checkWord(currentStates, word, currentIndex);
			if (verify)
				QMessageBox::information(this, "Testare","Cuvantul "+ label + " este acceptat");
			else
				QMessageBox::information(this, "Testare", "Cuvantul " + label + " nu este acceptat");
			update();
		}
		else if (automatonType == AutomatonType::AFNType)
		{

			std::vector<Arch*>& arches = graf.getArches();
			label = openWordBox();
			for (int i = 0; i < arches.size(); i++)
				currentStates.insert(i);
			std::string word = label.toStdString();
			currentIndex = 0;

			bool verify = automaton->checkWord(currentStates, word, currentIndex);
			if (verify)
				QMessageBox::information(this, "Testare", "Cuvantul " + label + " este acceptat");
			else
				QMessageBox::information(this, "Testare", "Cuvantul " + label + " nu este acceptat");
		}
		else if (automatonType == AutomatonType::AFNLType)
		{

			label = openWordBox();
			std::string word = label.toStdString();
			bool verify = automaton->checkWordLambda(word);
			if (verify)
				QMessageBox::information(this, "Testare", "Cuvantul " + label + " este acceptat");
			else
				QMessageBox::information(this, "Testare", "Cuvantul " + label + " nu este acceptat");
		}
		else if (automatonType == AutomatonType::APDType)
		{

			label = openWordBox();
			std::string word = label.toStdString();
			std::vector<Arch*>& arches = graf.getArches();
			for (int i = 0; i < arches.size(); i++)
				currentStates.insert(i);
			currentIndex = 0;
			std::stack<char> currentStack;
			std::vector<uint32_t> currentStatesVector(currentStates.begin(), currentStates.end());
			bool verify = automatonPD.checkWord(automaton->getq0(), currentStack, word);
			if (verify)
				QMessageBox::information(this, "Testare", "Cuvantul " + label + " este acceptat");
			else
				QMessageBox::information(this, "Testare", "Cuvantul " + label + " nu este acceptat");
		}
	}
}

void Automaton_Interface::on_verifyWordsButton_clicked()
{
	if (automatonType == AutomatonType::AFDType)
	{
		std::ifstream inputFile("WordstoVerify.txt");
		std::ofstream outputFile("WordsVerifyed.txt");
		std::string word;
		std::vector<Arch*>& arches = graf.getArches();
		for (int i = 0; i < arches.size(); i++)
			currentStates.insert(i);
		currentIndex = 0;
		while (inputFile >> word)
		{
			bool verify = automaton->checkWord(currentStates, word, currentIndex);
			if (verify)
				outputFile << word << '-' << "este acceptat"<<'\n';
			else
				outputFile << word << '-' << "nu este acceptat"<<'\n';
		}
		inputFile.close();
		outputFile.close();
	}
	else if (automatonType == AutomatonType::AFNType)
	{
		std::ifstream inputFile("WordstoVerify.txt");
		std::ofstream outputFile("WordsVerifyed.txt");
		std::string word;
		std::vector<Arch*>& arches = graf.getArches();
		for (int i = 0; i < arches.size(); i++)
			currentStates.insert(i);
		currentIndex = 0;
		while (inputFile >> word)
		{
			bool verify = automaton->checkWord(currentStates, word, currentIndex);
			if (verify)
				outputFile << word << '-' << "este acceptat" << '\n';
			else
				outputFile << word << '-' << "nu este acceptat" << '\n';
		}
		inputFile.close();
		outputFile.close();
	}
	else if (automatonType == AutomatonType::AFNLType)
	{
		std::ifstream inputFile("WordstoVerify.txt");
		std::ofstream outputFile("WordsVerifyed.txt");
		std::string word;
		std::vector<Arch*>& arches = graf.getArches();
		for (int i = 0; i < arches.size(); i++)
			currentStates.insert(i);
		currentIndex = 0;
		while (inputFile >> word)
		{
			bool verify = automaton->checkWordLambda(word);
			if (verify)
				outputFile << word << '-' << "este acceptat" << '\n';
			else
				outputFile << word << '-' << "nu este acceptat" << '\n';
		}
		inputFile.close();
		outputFile.close();
	}
	else if (automatonType == AutomatonType::APDType)
	{
		std::ifstream inputFile("WordstoVerify.txt");
		std::ofstream outputFile("WordsVerifyed.txt");
		std::string word;
		std::vector<Arch*>& arches = graf.getArches();

		currentStates = { automaton->getq0() };
		std::stack<char> currentStack;
		std::vector<uint32_t> currentStatesVector(currentStates.begin(), currentStates.end());
		while (inputFile >> word)
		{
			bool verify = automatonPD.checkWord(automaton->getq0(),currentStack,word);
			if (verify)
				outputFile << word << '-' << "este acceptat" << '\n';
			else
				outputFile << word << '-' << "nu este acceptat" << '\n';
		}
		inputFile.close();
		outputFile.close();
	}
}

void Automaton_Interface::showAutomatonTypeDialog()
{
	AutomatonTypeDialog dialog(this);

	if (dialog.exec() == QDialog::Accepted) {
		automatonType = dialog.getSelectedAutomatonType();

		switch (automatonType)
		{
		case AutomatonType::AFDType:
			automaton = new AFD;
			break;
		case AutomatonType::AFNType:
			automaton = new AFN;
			break;
		case AutomatonType::AFNLType:
			automaton = new AFN_lambda;
			break;
		case AutomatonType::APDType:

			break;
		}
	}
}

void Automaton_Interface::mouseReleaseEvent(QMouseEvent* e)
{

	if (e->button() == Qt::RightButton)
	{

		bool overlapping = false;
		for (Node* n : graf.getNodes())
		{
			Node tempNode;
			tempNode.setCoordinate(e->pos()); 
			tempNode.setValue(10); 

			if (n->isOverlapping(tempNode))
			{
				overlapping = true;
				break;
			}
		}

		if (!overlapping)
		{

			int stateValue = graf.getNodes().size();
			if (automatonType == AutomatonType::APDType)
			{
				automatonPD.addState(stateValue);
				automatonPD.setSizeQ(automatonPD.getQ().size());
			}
			else
			{
				automaton->addState(stateValue);
				automaton->setSizeQ(automaton->getQ().size());
			}

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

		Prod p;
		std::vector<Node*> nodes = graf.getNodes();
		for (Node* n : nodes)
		{
			if (abs(e->pos().x() - n->getX()) < 10 && abs(e->pos().y() - n->getY()) < 10 && deleteMode == false && finalMode == false && dragMode == false)
			{
				if (!firstNode)
				{
					firstNode = n;
				}
				else
				{

					if (automatonType == AutomatonType::APDType)
					{
						QString label;
						int noLabels;
						label = openTextBox();
						noLabels = label.toInt();
						std::vector<QString> labels;
						for (int j = 0; j < noLabels; j++)
						{
							QString secondLabel;
							secondLabel = openTextBox();
							labels.push_back(secondLabel);
							if (j == 0)
							{
								p.setAlphabet(secondLabel.toStdString()[0]);
								if (secondLabel.toStdString()[0] != '~')
									automatonPD.addSymbol(secondLabel.toStdString()[0]);
							}
							else if (j == 1)
							{
								p.setTopStack(secondLabel.toStdString()[0]);
								if (secondLabel.toStdString()[0] != '~')
									automatonPD.addStackSymbol(secondLabel.toStdString()[0]);
							}
							else p.setOverStack(secondLabel.toStdString());
						}
						if (!graf.arcExists(firstNode, n))
						{
							graf.addAPDArch(firstNode, n, labels);

							p.setInitialState(firstNode->getValue());
							p.setFinalState(n->getValue());
							automatonPD.addTransition(p);
						}
						else
						{

							graf.addAPDArch(n, firstNode, labels);

							p.setInitialState(n->getValue());
							p.setFinalState(firstNode->getValue());
							automatonPD.addTransition(p);
						}
						firstNode = nullptr;
					}
					else
					{
						QString label;
						label = openTextBox();

						if (!graf.arcExists(firstNode, n))
						{
							graf.addArch(firstNode, n, label);

							automaton->addSymbolToAlphabet(label.toStdString()[0]);
							automaton->addTransition(firstNode->getValue(), label.toStdString()[0], n->getValue());
						}
						else
						{

							graf.addArch(n, firstNode, label);
							automaton->addTransition(n->getValue(), label.toStdString()[0], firstNode->getValue());
						}
						firstNode = nullptr;
					}
				}
				update();
				break;
			}
		}
	}
}

void Automaton_Interface::paintEvent(QPaintEvent* e)
{
	QPainter p(this);
	std::vector<Node*>& noduri = graf.getNodes();
	for (Node* n : noduri)
	{
		QRect outerRect(n->getX() - 15, n->getY() - 15, 30, 30);
		QRect r(n->getX() - 10, n->getY() - 10, 20, 20);

		if (n->getFinalState())
		{
			QPen outerPen(Qt::black); 
			outerPen.setWidth(2);
			p.setPen(outerPen);
			p.drawEllipse(outerRect);
		}
		if (n->getInitialState())
		{

			QPen initialPen(Qt::black);
			initialPen.setWidth(2);
			p.setPen(initialPen);

			QPointF aux(n->getX() - 20, n->getY() - 20);
			QPointF aux2(n->getX() - 7.5, n->getY() - 7.5);
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

		QPen innerPen(Qt::black);
		innerPen.setWidth(2);
		p.setPen(innerPen);
		p.drawEllipse(r);

		QString s = "q" + QString::number(n->getValue());
		p.drawText(r, Qt::AlignCenter, s);
		if (nodeAnimation && std::find(nodeAnimations.begin(), nodeAnimations.end(), n) != nodeAnimations.end())
		{
			QRect outerRect(n->getX() - 15, n->getY() - 15, 30, 30);
			QRect r(n->getX() - 10, n->getY() - 10, 20, 20);
			if (n->getFinalState())
			{
				QPen outerPen(Qt::blue);
				outerPen.setWidth(2);
				p.setPen(outerPen);
				p.drawEllipse(outerRect);
			}
			if (n->getInitialState())
			{

				QPen initialPen(Qt::blue);
				initialPen.setWidth(2);
				p.setPen(initialPen);

				QPointF aux(n->getX() - 20, n->getY() - 20);
				QPointF aux2(n->getX() - 7.5, n->getY() - 7.5);
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

			QPen innerPen(Qt::blue);
			innerPen.setWidth(2);
			p.setPen(innerPen);
			p.drawEllipse(r);

			QString s = "q" + QString::number(n->getValue());
			p.drawText(r, Qt::AlignCenter, s);
		}
	}
	std::vector<Arch*>& arches = graf.getArches();
	std::vector<APDArch*>& apd_arches = graf.getAPDArches();
	if (automatonType != AutomatonType::APDType)

	{
		for (Arch* a : arches)
		{
			if (a->getFirstNode()->getValue() == a->getSecondNode()->getValue())
			{
				Node* n = a->getFirstNode();


				double firstLineLength = 30.0;
				p.drawLine(QPointF(n->getX(), n->getY() - 10),
					QPointF(n->getX(), n->getY() - firstLineLength));

				double secondLineLength = 30.0;
				p.drawLine(QPointF(n->getX(), n->getY() - firstLineLength),
					QPointF(n->getX() - secondLineLength, n->getY() - firstLineLength));
				QPointF l1(n->getX(), n->getY() - firstLineLength);
				QPointF l2(n->getX() - secondLineLength, n->getY() - firstLineLength);

				p.drawLine(QPointF(n->getX() - secondLineLength, n->getY() - firstLineLength),
					QPointF(n->getX() - secondLineLength, n->getY()));

				p.drawLine(QPointF(n->getX() - secondLineLength, n->getY()),
					QPointF(n->getX() - 10, n->getY()));

				QPointF lineStart = QPointF(n->getX() - secondLineLength, n->getY());
				QPointF lineEnd = QPointF(n->getX() - 10, n->getY());

				double arrowSize = 10.0; 
				double arrowAngle = M_PI / 6.0; 

				QPointF arrowP1 = QPointF(lineEnd.x() - arrowSize * cos(arrowAngle),
					lineEnd.y() - arrowSize * sin(arrowAngle));
				QPointF arrowP2 = QPointF(lineEnd.x() - arrowSize * cos(-arrowAngle),
					lineEnd.y() - arrowSize * sin(-arrowAngle));
				p.drawLine(lineEnd, arrowP1);
				p.drawLine(lineEnd, arrowP2);


				QPointF middle = (l1 + l2) / 2.0;
				QStringList elements = a->getElements();
				int elementY = middle.y() - 5;
				int index = std::distance(arches.begin(), std::find(arches.begin(), arches.end(), a));
				QRect elementRect(middle.x() - 40, elementY - 30, 40, 40);
				p.drawText(elementRect, Qt::AlignCenter, a->getLabel());
				elementY += 15;
			}

			else
			{

				bool existForward = graf.arcExists(a->getFirstNode(), a->getSecondNode());
				bool existBackward = graf.arcExists(a->getSecondNode(), a->getFirstNode());

				bool isForward = a->getFirstNode()->getX() < a->getSecondNode()->getX();
				bool isBackward = a->getFirstNode()->getX() > a->getSecondNode()->getX();

				if (existForward && existBackward)
				{

					if (isForward)
					{

						double separation = 7.0;
						double angle = atan2(a->getSecondNode()->getY() - a->getFirstNode()->getY(),
							a->getSecondNode()->getX() - a->getFirstNode()->getX());
						double arrowSize = 10.0;

						QPointF startForward = QPointF(a->getFirstNode()->getX() + cos(angle) * (arrowSize),
							a->getFirstNode()->getY() + sin(angle) * (arrowSize)-separation);
						QPointF endForward = QPointF(a->getSecondNode()->getX() - cos(angle) * (arrowSize),
							a->getSecondNode()->getY() - sin(angle) * (arrowSize)-separation);


						p.drawLine(startForward, endForward);
						QPointF arrowP1Forward = QPointF(endForward.x() - arrowSize * cos(angle - M_PI / 6),
							endForward.y() - arrowSize * sin(angle - M_PI / 6));
						QPointF arrowP2Forward = QPointF(endForward.x() - arrowSize * cos(angle + M_PI / 6),
							endForward.y() - arrowSize * sin(angle + M_PI / 6));
						p.drawLine(endForward, arrowP1Forward);
						p.drawLine(endForward, arrowP2Forward);

						QPointF middleForward = (a->getFirstNode()->getCoordinate() + a->getSecondNode()->getCoordinate()) / 2.0;


						QStringList elementsForward = a->getElements();

						int elementYForward = middleForward.y() - 7;

						QRect elementRectForward(middleForward.x() - 5, elementYForward, 10, 10);
						p.drawText(elementRectForward, Qt::AlignCenter, a->getLabel());
						elementYForward += 15;
					}
					else

						if (isBackward)
						{

							double separationBackward = 6.0;
							double angleBackward = atan2(a->getSecondNode()->getY() - a->getFirstNode()->getY(),
								a->getSecondNode()->getX() - a->getFirstNode()->getX());
							double arrowSize = 10.0;
							QPointF startBackward = QPointF(a->getFirstNode()->getX() + cos(angleBackward) * (arrowSize),
								a->getFirstNode()->getY() + sin(angleBackward) * (arrowSize)+separationBackward);
							QPointF endBackward = QPointF(a->getSecondNode()->getX() - cos(angleBackward) * (arrowSize),
								a->getSecondNode()->getY() - sin(angleBackward) * (arrowSize)+separationBackward);


							p.drawLine(startBackward, endBackward);
							QPointF arrowP1Backward = QPointF(endBackward.x() - arrowSize * cos(angleBackward - M_PI / 6),
								endBackward.y() - arrowSize * sin(angleBackward - M_PI / 6));
							QPointF arrowP2Backward = QPointF(endBackward.x() - arrowSize * cos(angleBackward + M_PI / 6),
								endBackward.y() - arrowSize * sin(angleBackward + M_PI / 6));
							p.drawLine(endBackward, arrowP1Backward);
							p.drawLine(endBackward, arrowP2Backward);

							QPointF middleBackward = (a->getFirstNode()->getCoordinate() + a->getSecondNode()->getCoordinate()) / 2.0;


							QStringList elementsBackward = a->getElements();
							int elementYBackward = middleBackward.y() - 5;

							QRect elementRectBackward(middleBackward.x() - 5, elementYBackward + 15, 10, 10);
							p.drawText(elementRectBackward, Qt::AlignCenter, a->getLabel());
							elementYBackward += 15;
						}
				}
				else
				{

					double angle = atan2(a->getSecondNode()->getY() - a->getFirstNode()->getY(),
						a->getSecondNode()->getX() - a->getFirstNode()->getX());
					double arrowSize = 10.0;

					QPointF start = QPointF(a->getFirstNode()->getX() + cos(angle) * (arrowSize),
						a->getFirstNode()->getY() + sin(angle) * (arrowSize));
					QPointF end = QPointF(a->getSecondNode()->getX() - cos(angle) * (arrowSize),
						a->getSecondNode()->getY() - sin(angle) * (arrowSize));

					p.drawLine(start, end);


					QPointF arrowP1 = QPointF(end.x() - arrowSize * cos(angle - M_PI / 6),
						end.y() - arrowSize * sin(angle - M_PI / 6));
					QPointF arrowP2 = QPointF(end.x() - arrowSize * cos(angle + M_PI / 6),
						end.y() - arrowSize * sin(angle + M_PI / 6));


					p.drawLine(end, arrowP1);
					p.drawLine(end, arrowP2);


					QPointF middle = (a->getFirstNode()->getCoordinate() + a->getSecondNode()->getCoordinate()) / 2.0;

					QStringList elements = a->getElements();
					int elementY = middle.y() - 5;

					QRect elementRect(middle.x() - 5, elementY + 15, 10, 10);
					p.drawText(elementRect, Qt::AlignCenter, a->getLabel());
					elementY += 15;
				}
				if (firstNode)
				{
					QRect r(firstNode->getX() - 10, firstNode->getY() - 10, 20, 20);
					QPen pen;

					pen.setWidth(2);
					p.setPen(pen);
					p.drawEllipse(r);
					QString s;
					s = "q" + QString::number(firstNode->getValue());
					p.drawText(r, Qt::AlignCenter, s);
				}
			}
			if (archAnimation)
			{
				p.setPen(QPen(Qt::blue, 2));
				if (std::find(archAnimations.begin(), archAnimations.end(), a) != archAnimations.end())
					p.drawLine(a->getFirstNode()->getCoordinate(), a->getSecondNode()->getCoordinate());
			}
		}
	}
	else
	{
		for (APDArch* a : apd_arches)
			if (a->getFirstNode()->getValue() == a->getSecondNode()->getValue())
			{
				Node* n = a->getFirstNode();


				double firstLineLength = 30.0;
				p.drawLine(QPointF(n->getX(), n->getY() - 10),
					QPointF(n->getX(), n->getY() - firstLineLength));

				double secondLineLength = 30.0;
				p.drawLine(QPointF(n->getX(), n->getY() - firstLineLength),
					QPointF(n->getX() - secondLineLength, n->getY() - firstLineLength));
				QPointF l1(n->getX(), n->getY() - firstLineLength);
				QPointF l2(n->getX() - secondLineLength, n->getY() - firstLineLength);

				p.drawLine(QPointF(n->getX() - secondLineLength, n->getY() - firstLineLength),
					QPointF(n->getX() - secondLineLength, n->getY()));

				p.drawLine(QPointF(n->getX() - secondLineLength, n->getY()),
					QPointF(n->getX() - 10, n->getY()));

				QPointF lineStart = QPointF(n->getX() - secondLineLength, n->getY());
				QPointF lineEnd = QPointF(n->getX() - 10, n->getY());


				double arrowSize = 10.0; 
				double arrowAngle = M_PI / 6.0; 

				QPointF arrowP1 = QPointF(lineEnd.x() - arrowSize * cos(arrowAngle),
					lineEnd.y() - arrowSize * sin(arrowAngle));
				QPointF arrowP2 = QPointF(lineEnd.x() - arrowSize * cos(-arrowAngle),
					lineEnd.y() - arrowSize * sin(-arrowAngle));
				p.drawLine(lineEnd, arrowP1);
				p.drawLine(lineEnd, arrowP2);


				QPointF middle = (l1 + l2) / 2.0;
				int elementY = middle.y() - 5;
				a->setNoLabels(a->getLabels().size());
				for (int i = 0; i < a->getNoLabels(); i++)
				{
					QRect elementRect(middle.x() - 30, elementY + 10, 10, 10);
					p.drawText(elementRect, Qt::AlignCenter, a->getLabels()[i]);
					elementY -= 15;
				}
			}

			else
			{
				double angle = atan2(a->getSecondNode()->getY() - a->getFirstNode()->getY(),
					a->getSecondNode()->getX() - a->getFirstNode()->getX());
				double arrowSize = 10.0;

				QPointF start = QPointF(a->getFirstNode()->getX() + cos(angle) * (arrowSize),
					a->getFirstNode()->getY() + sin(angle) * (arrowSize));
				QPointF end = QPointF(a->getSecondNode()->getX() - cos(angle) * (arrowSize),
					a->getSecondNode()->getY() - sin(angle) * (arrowSize));

				p.drawLine(start, end);


				QPointF arrowP1 = QPointF(end.x() - arrowSize * cos(angle - M_PI / 6),
					end.y() - arrowSize * sin(angle - M_PI / 6));
				QPointF arrowP2 = QPointF(end.x() - arrowSize * cos(angle + M_PI / 6),
					end.y() - arrowSize * sin(angle + M_PI / 6));

				p.drawLine(end, arrowP1);
				p.drawLine(end, arrowP2);

				QPointF middle = (a->getFirstNode()->getCoordinate() + a->getSecondNode()->getCoordinate()) / 2.0;

				QStringList elements = a->getElements();
				int elementY = middle.y() - 5;

				a->setNoLabels(a->getLabels().size());
				for (int i = 0; i < a->getNoLabels(); i++)
				{
					QRect elementRect(middle.x() - 5, elementY + 15, 50, 10);
					p.drawText(elementRect, Qt::AlignCenter, a->getLabels()[i]);
					elementY += 15;
				}

				if (firstNode)
				{
					QRect r(firstNode->getX() - 10, firstNode->getY() - 10, 20, 20);
					QPen pen;
					pen.setWidth(2);
					p.setPen(pen);
					p.drawEllipse(r);
					QString s;
					s = "q" + QString::number(firstNode->getValue());
					p.drawText(r, Qt::AlignCenter, s);
				}
			}
	}

}

void Automaton_Interface::mousePressEvent(QMouseEvent* e) {
	if (deleteMode && e->button() == Qt::LeftButton) {
		std::vector<Node*> nodes = graf.getNodes();

		for (Node* n : nodes) {
			if (abs(e->pos().x() - n->getX()) < 10 && abs(e->pos().y() - n->getY()) < 10) {

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

						n->setFinalState(true);
						if (automatonType != AutomatonType::APDType)
							automaton->addFinalState(n->getValue());
						else automatonPD.addFinalState(n->getValue());
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

						draggedNode = n;
						update();
						break;
					}
				}
			}

}

void Automaton_Interface::mouseMoveEvent(QMouseEvent* e) {
	if (dragMode) {

		int dx = e->pos().x() - lastMousePos.x();
		int dy = e->pos().y() - lastMousePos.y();

		draggedNode->setCoordinate(draggedNode->getCoordinate() + QPoint(dx, dy));
		lastMousePos = e->pos();
		update();
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

QString Automaton_Interface::openWordBox()
{
	WordBox inputDialog;
	QString labelEnt;
	if (inputDialog.exec() == QDialog::Accepted)
	{
		labelEnt = inputDialog.getEnteredText();
	}
	return labelEnt;
}
