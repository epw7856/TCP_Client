#ifndef CONFIGFILEVERIFICATIONHANDLER_H
#define CONFIGFILEVERIFICATIONHANDLER_H

#include <QString>

class ConfigFileVerificationHandler
{
public:
    ConfigFileVerificationHandler() {}
    bool verifyFilePath(const QString& filePath);
    void showConfigFileErrorPopup(const QString& message);
};

#endif // CONFIGFILEVERIFICATIONHANDLER_H
