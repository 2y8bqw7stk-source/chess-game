#include <QApplication>
#include <QFont>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Police par défaut (pour les symboles Unicode échecs)
    QFont defaultFont("Segoe UI", 10);
    app.setFont(defaultFont);

    app.setApplicationName("Jeu d'Échecs");
    app.setOrganizationName("ChessQt");

    MainWindow w;
    w.show();

    return app.exec();
}
