#include "EnumSelectionController.h"

void EnumSelectionController::setNewValue(const QString& value, unsigned index)
{
    newValue = value;
    newValueIndex = index;
}

bool EnumSelectionController::accept(const QString& value, unsigned index)
{
    newValue = value;
    newValueIndex = index;
    accepted = enableAcceptButton();
    return accepted;
}

bool EnumSelectionController::enableAcceptButton() const
{
    return (newValue != "");
}

bool EnumSelectionController::isAccepted() const
{
    return accepted;
}

QString EnumSelectionController::getNewValue()
{
    return newValue;
}
