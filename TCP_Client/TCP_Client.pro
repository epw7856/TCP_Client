QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = App.ico

SOURCES += \
    AboutDialog.cpp \
    ApplicationSettingsController.cpp \
    ApplicationSettingsDialog.cpp \
    CommunicationsManager.cpp \
    CommunicationsUtilities.cpp \
    DataUtilities.cpp \
    EnumSelectionController.cpp \
    EnumSelectionDialog.cpp \
    EnumSelectionTableModel.cpp \
    EnumType.cpp \
    FileOperationsHandler.cpp \
    InboundDataTableModel.cpp \
    Main.cpp \
    MainWindowController.cpp \
    OutboundDataTableModel.cpp \
    RangeCheckHandler.cpp \
    SettingsManager.cpp \
    SocketProtocol.cpp \
    SystemDataSource.cpp \
    MainWindow.cpp

HEADERS += \
    AboutDialog.h \
    ApplicationInterface.h \
    ApplicationSettings.h \
    ApplicationSettingsController.h \
    ApplicationSettingsDialog.h \
    CommunicationsManager.h \
    CommunicationsUtilities.h \
    ConnectionStatus.h \
    DataItem.h \
    DataUtilities.h \
    EnumInterface.h \
    EnumSelectionController.h \
    EnumSelectionDialog.h \
    EnumSelectionTableModel.h \
    EnumType.h \
    FileOperationsHandler.h \
    InboundDataInterface.h \
    InboundDataTableModel.h \
    MainWindow.h \
    MainWindowController.h \
    OutboundDataInterface.h \
    OutboundDataTableModel.h \
    RangeCheckHandler.h \
    SemanticVersion.h \
    SettingsInterface.h \
    SettingsManager.h \
    SocketProtocol.h \
    SystemDataSource.h

FORMS += \
    AboutDialog.ui \
    ApplicationSettingsDialog.ui \
    EnumSelectionDialog.ui \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
