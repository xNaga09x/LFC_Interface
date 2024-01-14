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
	finalMode = false;
	deleteMode = false;
	dragMode = false;
	//connect(ui.deleteButton, &QPushButton::clicked, this, &AutomatonInterface::onButtonClicked);
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
	//Deschide file explorer-ul cu QFileDialog si pune absolute path-ul file-ului ales intr-un QString
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Automaton"), "C:/", tr("Image Files(*.txt *.in)"));
	//Transforma QString to std::string
	std::string stringFileName = fileName.toStdString();
	//Deschide fisierul ales si citeste din el
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

	//apelare functie in maindow de desenare a automatului respectiv
	uint32_t x, y;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib_x(194, 583);
	std::uniform_int_distribution<> distrib_y(139, 362);
	std::unordered_set<int> coordsUsed_x;
	std::unordered_set<int> coordsUsed_y;
	if (automatonType != AutomatonType::APDType)
	{
		//noduri parcurgand vectorul de stari
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
		//arce parcurgand vectorul de tranzitii
		std::vector<std::tuple<int, char, int>> transitions = toDrawAutomaton->getDelta();
		for (uint32_t i = 0; i < transitions.size(); i++)
		{
			uint32_t firstNode = std::get<0>(transitions[i]);
			char symbol = std::get<1>(transitions[i]);
			QString label(symbol);
			uint32_t secondNode = std::get<2>(transitions[i]);
			Node* first = graf.getNodeById(firstNode);
			Node* second = graf.getNodeById(secondNode);
			graf.addArch(first, second, label);	//nu prea se vad bine arcele de la un nod la el insusi check
			//nu se marcheza nodurile finale visual
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
		//arce parcurgand vectorul de tranzitii
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
			//vizual probleme: se suprapun tranzitiile care pornesc si ajung in acelasi nod
			//(la apd sunt destul de comune)
			//nici aici nu sunt marcate nodurile finale
		}
	}
	update();
	file.close();
}

void Automaton_Interface::on_saveToFileButton_clicked()
{
	//Deschide file explorer-ul cu QFileDialog si pune absolute path-ul file-ului ales intr-un QString
	QString fileName = QFileDialog::getOpenFileName(this, tr("Choose save file"), "C:/", tr("Image Files(*.txt *.in)"));
	//Transforma QString to std::string
	std::string stringFileName = fileName.toStdString();
	std::ofstream file(stringFileName);
	if (automatonType != AutomatonType::APDType)
		automaton->printAutomaton(file);
	else automatonPD.printAutomaton(file);
	file.close();
}
// Adăugați implementarea acestor funcții în fișierul .cpp al clasei Automaton_Interface
void Automaton_Interface::highlightNode(Node* node, QColor color)
{
	// Implementați schimbarea culorii pentru nodul dat
	node->color = color;
	update();  // Asigurați-vă că actualizați interfața grafică
}

void Automaton_Interface::highlightArc(Arch* arc, QColor color)
{
	// Implementați schimbarea culorii pentru arcul dat
	arc->color = color;
	update();  // Asigurați-vă că actualizați interfața grafică
}

void Automaton_Interface::resetColors()
{
	// Implementați revenirea la culorile inițiale pentru toate nodurile și arcele automatului
	for (Node* node : graf.getNodes())
		node->color = Qt::black;  // Sau culoarea inițială a nodurilor

	for (Arch* arc : graf.getArches())
		arc->color = Qt::black;  // Sau culoarea inițială a arcelor

	update();  // Asigurați-vă că actualizați interfața grafică
}
void Automaton_Interface::on_testWordButton_clicked()
{
	QString label = openWordBox();
	if (!label.isEmpty())
	{
		
		// Verificați tipul automatului
		if (automatonType == AutomatonType::AFDType)
		{
			// Automat de tip AFD
			//label = openWordBox();
			std::vector<Arch*>& arches = graf.getArches();
			for (int i = 0; i < arches.size(); i++)
				currentStates.insert(i);
			//currentStates = { automaton->getq0() };
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
			//currentStates = { automaton->getq0() };
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
			// Automat de tip AFN
			std::vector<Arch*>& arches = graf.getArches();
			label = openWordBox();
			for (int i = 0; i < arches.size(); i++)
				currentStates.insert(i);
			//currentStates = { automaton->getq0() };
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
			// Automat de tip AFN_lambda
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
			// Automat de tip APD
			label = openWordBox();
			std::string word = label.toStdString();
			std::vector<Arch*>& arches = graf.getArches();
			for (int i = 0; i < arches.size(); i++)
				currentStates.insert(i);
			//currentStates = { automaton->getq0() };
			currentIndex = 0;
			std::stack<char> currentStack;
			std::vector<uint32_t> currentStatesVector(currentStates.begin(), currentStates.end());
			bool verify = automatonPD.checkWord(automaton->getq0(), currentStack, word); //changes here
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
		//currentStates = { automaton->getq0() };
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
		//currentStates = { automaton->getq0() };
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
		//currentStates = { automaton->getq0() };
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
		/*for (int i = 0; i < arches.size(); i++)
			currentStates.insert(i);*/
		currentStates = { automaton->getq0() };
		std::stack<char> currentStack;
		std::vector<uint32_t> currentStatesVector(currentStates.begin(), currentStates.end());
		while (inputFile >> word)
		{
			bool verify = automatonPD.checkWord(automaton->getq0(),currentStack,word); //check if works
			if (verify)
				outputFile << word << '-' << "este acceptat" << '\n';
			else
				outputFile << word << '-' << "nu este acceptat" << '\n';
		}
		inputFile.close();
		outputFile.close();
	}
}
//void Automaton_Interface::on_testWordButton_clicked()
//{
//	QString label = openWordBox();
//	if (!label.isEmpty())
//	{// Verificați tipul automatului
//		 // Verificați tipul automatului
//		// Verificați tipul automatului
//		if (automatonType == AutomatonType::AFDType)
//		{
//			//currentStates = { automaton->getq0() };
//			//currentIndex = 0;
//			//wordAccepted = automaton->checkWord(currentStates, label.toStdString(), currentIndex);
//
//			//// Clear previous animations
//			//resetColors();
//
//			//// Realizați animația
//			//QTimer* animationTimer = new QTimer(this);
//			//connect(animationTimer, &QTimer::timeout, [=]() {
//			//	if (currentIndex < label.length())
//			//	{
//			//		int currentStateValue = *currentStates.begin();
//			//		highlightNode(graf.getNodeByCoordinates(graf.getNodes()[currentStateValue]->getCoordinate()), QColor("green"));
//
//			//		std::unordered_set<int> nextStateSet;
//			//		for (int currentState : currentStates)
//			//		{
//			//			std::unordered_set<int> tempState = { currentState };
//			//			if (automaton->checkWord(tempState, label.toStdString(), currentIndex + 1))
//			//			{
//			//				int next = *tempState.begin();
//			//				nextStateSet.insert(next);
//			//			}
//			//		}
//
//			//		if (!nextStateSet.empty())
//			//		{
//			//			int nextState = *nextStateSet.begin();
//			//			highlightArc(graf.getArcByNodes(graf.getNodes()[currentStateValue], graf.getNodes()[nextState]), QColor("blue"));
//			//			currentStates.clear();
//			//			currentStates.insert(nextState);
//			//			currentIndex++;
//
//			//			// Update the GUI
//			//			update();
//			//		}
//			//		else
//			//		{
//			//			// Dacă nextStateSet este gol, cuvântul nu este acceptat
//			//			animationTimer->stop();
//			//			delete animationTimer;
//
//			//			// Afișați pop-up-ul cu rezultatul
//			//			if (wordAccepted)
//			//				QMessageBox::information(this, "Rezultat", "Cuvântul este acceptat!");
//			//			else
//			//				QMessageBox::warning(this, "Rezultat", "Cuvântul nu este acceptat!");
//			//		}
//			//	}
//			//	else
//			//	{
//			//		// Cuvântul a fost procesat complet
//			//		animationTimer->stop();
//			//		delete animationTimer;
//
//			//		// Afișați pop-up-ul cu rezultatul
//			//		if (wordAccepted)
//			//			QMessageBox::information(this, "Rezultat", "Cuvântul este acceptat!");
//			//		else
//			//			QMessageBox::warning(this, "Rezultat", "Cuvântul nu este acceptat!");
//			//	}
//			//	});
//
//			//// Start the animation timer with a 2-second interval
//			//animationTimer->start(2000);
//			//animationTimer->setSingleShot(true); // Ensures that the timer stops after one shot
//			//// Setează starea inițială
//			//currentStates = { automaton->getq0() };
//			//currentIndex = 0;
//			//wordAccepted = automaton->checkWord(currentStates, label.toStdString(), currentIndex);
//
//			//// Verifică și afișează animația într-o fereastră separată
//			//if (wordAccepted)
//			//{
//			//	/*AnimationInterface* animationInterface = new AnimationInterface(this);
//			//	animationInterface->setLabel(label);
//			//	animationInterface->show();
//			//	animationInterface->startAnimation();*/
//			//}
//			//else
//			//{
//			//	QMessageBox::warning(this, "Rezultat", "Cuvântul nu este acceptat!");
//			//}
//		}
//		else if (automatonType == AutomatonType::AFNType)
//		{
//			// Automat de tip AFN
//			label = openWordBox();
//		}
//		else if (automatonType == AutomatonType::AFNLType)
//		{
//			// Automat de tip AFN_lambda
//			label = openWordBox();
//		}
//		else if (automatonType == AutomatonType::APDType)
//		{
//			// Automat de tip APD
//			label = openWordBox();
//		}
//	}
//}

//void Automaton_Interface::on_testWordButton_clicked()
//{
//	// Deschideți un dialog pentru introducerea cuvântului
//	QString label = openWordBox();
//	if (label.isEmpty()) {
//		// Anulați dacă cuvântul este gol
//		return;
//	}
//
//	// Verificați tipul automatului
//	if (automatonType == AutomatonType::AFDType) {
//		// Setați starea inițială
//		currentStates = { automaton->getq0() };
//		currentIndex = 0;
//		wordAccepted = automaton->checkWord(currentStates, label.toStdString(), currentIndex);
//
//		// Clear previous animations
//		resetColors();
//
//		// Create and show a progress dialog
//		QProgressDialog progressDialog("Testare cuvânt în curs...", "Anulare", 0, label.length(), this);
//		progressDialog.setWindowModality(Qt::WindowModal);
//		progressDialog.setMinimumDuration(0);  // Permiteți afișarea imediată
//
//		// Realizați animația
//		while (currentIndex < label.length()) {
//			int currentStateValue = *currentStates.begin();
//			highlightNode(graf.getNodeByCoordinates(graf.getNodes()[currentStateValue]->getCoordinate()), QColor("green"));
//
//			std::unordered_set<int> nextStateSet;
//			for (int currentState : currentStates) {
//				std::unordered_set<int> tempState = { currentState };
//				if (automaton->checkWord(tempState, label.toStdString(), currentIndex + 1)) {
//					int next = *tempState.begin();
//					nextStateSet.insert(next);
//				}
//			}
//
//			if (!nextStateSet.empty()) {
//				int nextState = *nextStateSet.begin();
//				Node* currentNode = graf.getNodes()[currentStateValue];
//				Node* nextNode = graf.getNodes()[nextState];
//
//				// Verificați dacă există un arc între nodul curent și nodul următor
//				Arch* arc = graf.getArcByNodes(currentNode, nextNode);
//				if (arc) {
//					// Dacă există, evidențiați arcul
//					highlightArc(arc, QColor("blue"));
//				}
//				else {
//					// Dacă nu există un arc, tratați această situație corespunzător (de exemplu, afișați un mesaj de eroare)
//					qDebug() << "Arcul nu exista intre nodurile " << currentStateValue << " si " << nextState;
//				}
//
//				currentStates.clear();
//				currentStates.insert(nextState);
//				currentIndex++;
//
//				// Update the GUI
//				update();
//				progressDialog.setValue(currentIndex);
//				QCoreApplication::processEvents();  // Permiteți procesării evenimentelor pentru a răspunde la anulare
//				QThread::msleep(2000);  // Pausează pentru 2 secunde
//				resetColors();  // Resetați culorile
//
//				// Verificați dacă utilizatorul a anulat
//				if (progressDialog.wasCanceled()) {
//					progressDialog.reset();  // Resetare progres la valoarea inițială
//					return;
//				}
//			}
//			else {
//				// Dacă nextStateSet este gol, cuvântul nu este acceptat
//				wordAccepted = false;
//				break;
//			}
//		}
//
//		// Afișați pop-up-ul cu rezultatul
//		if (wordAccepted)
//			QMessageBox::information(this, "Rezultat", "Cuvântul este acceptat!");
//		else
//			QMessageBox::warning(this, "Rezultat", "Cuvântul nu este acceptat!");
//	}
//	else if (automatonType == AutomatonType::AFNType) {
//		// Automat de tip AFN
//		// ... (Implementați tratamentul pentru tipul AFN)
//	}
//	else if (automatonType == AutomatonType::AFNLType) {
//		// Automat de tip AFN_lambda
//		// ... (Implementați tratamentul pentru tipul AFN_lambda)
//	}
//	else if (automatonType == AutomatonType::APDType) {
//		// Automat de tip APD
//		// ... (Implementați tratamentul pentru tipul APD)
//	}
//}

void Automaton_Interface::showAutomatonTypeDialog()
{
	AutomatonTypeDialog dialog(this);

	if (dialog.exec() == QDialog::Accepted) {
		automatonType = dialog.getSelectedAutomatonType();
		//delete automaton;
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
			//se va folosi variabila automatonPD pentru bg obj
			break;
		}
	}
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
			//adauga starile in obiect
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
		//sa verific daca exista nod
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
					//adaugare arce in functie de tipul de automat
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
							//adaug tranzitia corespunzatoare in obiectul APD
							p.setInitialState(firstNode->getValue());
							p.setFinalState(n->getValue());
							automatonPD.addTransition(p);
						}
						else
						{
							// Adaugare arc care se intoarce la acelasi nod
							graf.addAPDArch(n, firstNode, labels);
							//adaug tranzitia corespunzatoare in obiectul APD
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
						// Aici se face verificarea pentru existenta unui arc cu acelasi sens intre aceleasi doua noduri
						if (!graf.arcExists(firstNode, n))
						{
							graf.addArch(firstNode, n, label);
							//adaug tranzitia corespunzatoare in obiect
							automaton->addSymbolToAlphabet(label.toStdString()[0]);
							automaton->addTransition(firstNode->getValue(), label.toStdString()[0], n->getValue());
						}
						else
						{
							// Adaugare arc care se intoarce la acelasi nod
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

		QPen innerPen(Qt::black); // Culoarea și grosimea conturului cercului interior
		innerPen.setWidth(2);
		p.setPen(innerPen);
		p.drawEllipse(r);

		QString s = "q" + QString::number(n->getValue());
		p.drawText(r, Qt::AlignCenter, s);
		if (nodeAnimation && std::find(nodeAnimations.begin(), nodeAnimations.end(), n) != nodeAnimations.end())
		{
			//QPen innerPen(Qt::blue); // Culoarea și grosimea conturului cercului interior
			//innerPen.setWidth(2);
			//p.setPen(innerPen);
			//if(std::find(nodeAnimations.begin(),nodeAnimations.end(),n)!=nodeAnimations.end())
			//	p.drawEllipse(r);
			//QString s = "q" + QString::number(n->getValue());
			//p.drawText(r, Qt::AlignCenter, s);
			QRect outerRect(n->getX() - 15, n->getY() - 15, 30, 30); // Dimensiunile cercului exterior
			QRect r(n->getX() - 10, n->getY() - 10, 20, 20);//xr=xnode-10 yr=yn-10 si dim dreptunghiului
			if (n->getFinalState())
			{
				QPen outerPen(Qt::blue); // Culoarea și grosimea conturului cercului exterior
				outerPen.setWidth(2);
				p.setPen(outerPen);
				p.drawEllipse(outerRect);
			}
			if (n->getInitialState())
			{
				//Desenarea sagetii pentru nodul initial
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

			QPen innerPen(Qt::blue); // Culoarea și grosimea conturului cercului interior
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
		//Cazul in care avem automat !=APD
	{
		for (Arch* a : arches)//desenam linie intre coordonatele primului si al doilea nod
		{	//Cazul in care avem arc de la un nod la el insusi
		//Cazul in care avem arc de la un nod tot la el
			if (a->getFirstNode()->getValue() == a->getSecondNode()->getValue())
			{
				Node* n = a->getFirstNode();

				//Setare dimensiuni linii
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
				QRect elementRect(middle.x() - 40, elementY - 30, 40, 40);//crearea patratului coordonatele de pe stanga si dreapta,latimea si inaltimea dreptunghiului
				p.drawText(elementRect, Qt::AlignCenter, a->getLabel());//element);
				elementY += 15;//Spatiile intre elemente
			}
			//Cazul in care avem arc intre 2 noduri diferite
			else
			{
				//verifica daca am arc de la nod la celalalt si invers
				bool existForward = graf.arcExists(a->getFirstNode(), a->getSecondNode());
				bool existBackward = graf.arcExists(a->getSecondNode(), a->getFirstNode());
				//verifica arcul daca este de la primul la al doilea sau nu
				bool isForward = a->getFirstNode()->getX() < a->getSecondNode()->getX();
				bool isBackward = a->getFirstNode()->getX() > a->getSecondNode()->getX();
				//verifica daca exista arcul de la un nod la altul si invers 
				if (existForward && existBackward)
				{
					//face arcul de la primul nod la al doilea
					if (isForward)
					{
						// Calculeaza pozitiile de start si sfarsit pentru arcul in directia initiala
						double separation = 7.0;
						double angle = atan2(a->getSecondNode()->getY() - a->getFirstNode()->getY(),
							a->getSecondNode()->getX() - a->getFirstNode()->getX());
						double arrowSize = 10.0;

						QPointF startForward = QPointF(a->getFirstNode()->getX() + cos(angle) * (arrowSize),
							a->getFirstNode()->getY() + sin(angle) * (arrowSize)-separation);
						QPointF endForward = QPointF(a->getSecondNode()->getX() - cos(angle) * (arrowSize),
							a->getSecondNode()->getY() - sin(angle) * (arrowSize)-separation);

						// Deseneaza arcul în directia initiala
						p.drawLine(startForward, endForward);
						QPointF arrowP1Forward = QPointF(endForward.x() - arrowSize * cos(angle - M_PI / 6),
							endForward.y() - arrowSize * sin(angle - M_PI / 6));
						QPointF arrowP2Forward = QPointF(endForward.x() - arrowSize * cos(angle + M_PI / 6),
							endForward.y() - arrowSize * sin(angle + M_PI / 6));
						p.drawLine(endForward, arrowP1Forward);
						p.drawLine(endForward, arrowP2Forward);
						// Calcularea mijlocului arcului
						QPointF middleForward = (a->getFirstNode()->getCoordinate() + a->getSecondNode()->getCoordinate()) / 2.0;

						// Desenarea elementelor in mijlocul arcului
						QStringList elementsForward = a->getElements();

						// Ajusteaza inaltimea pentru a fi pozitionat mai aproape de arc
						int elementYForward = middleForward.y() - 7;

						QRect elementRectForward(middleForward.x() - 5, elementYForward, 10, 10);
						p.drawText(elementRectForward, Qt::AlignCenter, a->getLabel());
						elementYForward += 15;
					}
					else
						//face arcul de la al doilea nod la primul
						if (isBackward)
						{
							// Calculeaza pozitiile de start si sfarsit pentru arcul in directia inversa
							double separationBackward = 6.0;
							double angleBackward = atan2(a->getSecondNode()->getY() - a->getFirstNode()->getY(),
								a->getSecondNode()->getX() - a->getFirstNode()->getX());
							double arrowSize = 10.0;
							QPointF startBackward = QPointF(a->getFirstNode()->getX() + cos(angleBackward) * (arrowSize),
								a->getFirstNode()->getY() + sin(angleBackward) * (arrowSize)+separationBackward);
							QPointF endBackward = QPointF(a->getSecondNode()->getX() - cos(angleBackward) * (arrowSize),
								a->getSecondNode()->getY() - sin(angleBackward) * (arrowSize)+separationBackward);

							// Deseneaza arcul in directia inversa
							p.drawLine(startBackward, endBackward);
							QPointF arrowP1Backward = QPointF(endBackward.x() - arrowSize * cos(angleBackward - M_PI / 6),
								endBackward.y() - arrowSize * sin(angleBackward - M_PI / 6));
							QPointF arrowP2Backward = QPointF(endBackward.x() - arrowSize * cos(angleBackward + M_PI / 6),
								endBackward.y() - arrowSize * sin(angleBackward + M_PI / 6));
							p.drawLine(endBackward, arrowP1Backward);
							p.drawLine(endBackward, arrowP2Backward);
							// Calcularea mijlocului arcului
							QPointF middleBackward = (a->getFirstNode()->getCoordinate() + a->getSecondNode()->getCoordinate()) / 2.0;

							// Desenarea elementelor in mijlocul arcului
							QStringList elementsBackward = a->getElements();
							int elementYBackward = middleBackward.y() - 5;

							QRect elementRectBackward(middleBackward.x() - 5, elementYBackward + 15, 10, 10);
							p.drawText(elementRectBackward, Qt::AlignCenter, a->getLabel());
							elementYBackward += 15;
						}
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

					// Calcularea punctelor pentru capul sagetii
					QPointF arrowP1 = QPointF(end.x() - arrowSize * cos(angle - M_PI / 6),
						end.y() - arrowSize * sin(angle - M_PI / 6));
					QPointF arrowP2 = QPointF(end.x() - arrowSize * cos(angle + M_PI / 6),
						end.y() - arrowSize * sin(angle + M_PI / 6));

					// Desenarea capului sagetii
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
					//int index = std::distance(arches.begin(), std::find(arches.begin(), arches.end(), a));
					QRect elementRect(middle.x() - 5, elementY + 15, 10, 10);//crearea patratului coordonatele de pe stanga si dreapta,latimea si inaltimea dreptunghiului
					p.drawText(elementRect, Qt::AlignCenter, a->getLabel());//element);
					elementY += 15;//Spatiile intre elemente
				}
				if (firstNode)
				{
					QRect r(firstNode->getX() - 10, firstNode->getY() - 10, 20, 20);
					QPen pen;
					//pen.setColor(Qt::red);
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

				//Setare dimensiuni linii
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
				int elementY = middle.y() - 5;
				a->setNoLabels(a->getLabels().size());
				for (int i = 0; i < a->getNoLabels(); i++)
				{
					QRect elementRect(middle.x() - 30, elementY + 10, 10, 10);
					p.drawText(elementRect, Qt::AlignCenter, a->getLabels()[i]);//element);
					elementY -= 15;//Spatiile intre elemente
				}
			}
		//Cazul in care avem arc intre 2 noduri diferite
			else
			{
				double angle = atan2(a->getSecondNode()->getY() - a->getFirstNode()->getY(),
					a->getSecondNode()->getX() - a->getFirstNode()->getX());
				double arrowSize = 10.0; // Ajustarea dimensiunii sagetii dupa preferinta

				QPointF start = QPointF(a->getFirstNode()->getX() + cos(angle) * (arrowSize),
					a->getFirstNode()->getY() + sin(angle) * (arrowSize));
				QPointF end = QPointF(a->getSecondNode()->getX() - cos(angle) * (arrowSize),
					a->getSecondNode()->getY() - sin(angle) * (arrowSize));

				p.drawLine(start, end);

				// Calcularea punctelor pentru capul sagetii
				QPointF arrowP1 = QPointF(end.x() - arrowSize * cos(angle - M_PI / 6),
					end.y() - arrowSize * sin(angle - M_PI / 6));
				QPointF arrowP2 = QPointF(end.x() - arrowSize * cos(angle + M_PI / 6),
					end.y() - arrowSize * sin(angle + M_PI / 6));

				// Desenarea capului sagetii
				p.drawLine(end, arrowP1);
				p.drawLine(end, arrowP2);

				//Calcularea mijlocului arcului
				QPointF middle = (a->getFirstNode()->getCoordinate() + a->getSecondNode()->getCoordinate()) / 2.0;
				//Desenarea elementelor in mijlocul arcului
				QStringList elements = a->getElements();
				int elementY = middle.y() - 5;

				a->setNoLabels(a->getLabels().size());
				for (int i = 0; i < a->getNoLabels(); i++)
				{
					QRect elementRect(middle.x() - 5, elementY + 15, 50, 10);
					p.drawText(elementRect, Qt::AlignCenter, a->getLabels()[i]);
					elementY += 15;
				}

				//}
				if (firstNode)
				{
					QRect r(firstNode->getX() - 10, firstNode->getY() - 10, 20, 20);
					QPen pen;
					//pen.setColor(Qt::red);
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
