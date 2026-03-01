#include "app/MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Mini-CAD");
    app.setOrganizationName("University");

    MainWindow window;
    window.show();

    return app.exec();
}
