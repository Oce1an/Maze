#include "MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("MazePathfinder"));
    QApplication::setOrganizationName(QStringLiteral("Maze"));

    MainWindow window;
    window.show();

    return app.exec();
}
