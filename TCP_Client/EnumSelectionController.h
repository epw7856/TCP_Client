#ifndef ENUMSELECTIONCONTROLLER_H
#define ENUMSELECTIONCONTROLLER_H

#include <QString>

class EnumSelectionController
{
public:
    EnumSelectionController() {}
    void setNewValue(const QString& value, unsigned index);
    bool accept(const QString& value, unsigned index);
    bool enableAcceptButton() const;
    bool isAccepted() const;
    QString getNewValue();

private:
    QString newValue = "";
    unsigned newValueIndex = 0U;
    bool enableAccept = false;
    bool accepted = false;
};

#endif // ENUMSELECTIONCONTROLLER_H
