#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    app.setOrganizationName("QtProject");
    app.setApplicationName("Application Example");
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}