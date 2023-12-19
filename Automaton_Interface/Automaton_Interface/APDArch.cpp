#include "APDArch.h"

APDArch::APDArch()
{
	firstNode = nullptr;
	secondNode = nullptr;
}

APDArch::APDArch(Node* firstNode, Node* secondNode, const std::vector<QString>& labels)
	: firstNode(firstNode), secondNode(secondNode), labels(labels)
{
}

std::vector<QString> APDArch::getLabels() const
{
	return labels;
}

void APDArch::setLabels(const std::vector<QString>& newLabels)
{
	labels = newLabels;
}

int APDArch::setNoLabels(const int& number)
{
	return noLabels = number;
}

const QStringList& APDArch::getElements() const
{
	return elements;
}

const int& APDArch::getNoLabels() const
{
	return noLabels;
}

void APDArch::addElement(const QString& element)
{
	elements.append(element);
}
