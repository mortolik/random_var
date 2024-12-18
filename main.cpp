#include "mainwindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Генерация случайной величины");
    window.resize(600, 400);
    window.show();

    return app.exec();
}
