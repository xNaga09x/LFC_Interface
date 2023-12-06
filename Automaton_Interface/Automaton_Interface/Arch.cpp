#include "Arch.h"

Arch::Arch()
{
	firstNode = nullptr;
	secondNode = nullptr;
}

Arch::Arch(Node* firstNode, Node* secondNode, const QString& label)
	: firstNode(firstNode), secondNode(secondNode), label(label)
{
}

QString Arch::getLabel() const
{
	return label;
}

void Arch::setLabel(const QString& newLabel)
{
	label = newLabel;
}

const QStringList& Arch::getElements() const
{
	return elements;
}

void Arch::addElement(const QString& element)
{
	elements.append(element);
}
