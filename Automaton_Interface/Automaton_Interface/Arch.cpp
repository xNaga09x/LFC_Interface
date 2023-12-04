#include "Arch.h"

Arch::Arch()
{
	firstNode = nullptr;
	secondNode = nullptr;
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
