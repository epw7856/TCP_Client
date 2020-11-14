#include "MainWindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include "SemanticVersion.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Local Control Application");
    QCoreApplication::setApplicationVersion(SemanticVersion);

    QCommandLineParser parser;
    parser.setApplicationDescription("\nThis application is a TCP client used to communicate with a server over socket port.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("config-file",
                                 QCoreApplication::translate("main", "JSON configuration file to load on startup."));

    parser.process(app);
    MainWindow w((!parser.positionalArguments().isEmpty()) ? parser.positionalArguments().at(0) : QString());
    w.show();

    return app.exec();
}
