#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QString>

struct ApplicationSettings
{
    QString configurationVersion = "";
    QString headerFooterText = "";
    QString systemConfigFilePath = "";
    unsigned socketPort = 0U;
    unsigned transmissionPeriodicity = 0U;
};

#endif // APPLICATIONSETTINGS_H
