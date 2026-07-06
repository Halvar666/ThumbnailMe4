// SPDX-License-Identifier: GPL-2.0-or-later
#include "MainWindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Thumbnail me 4b2");
    app.setApplicationDisplayName("Thumbnail me 4b2");
    app.setOrganizationName("Thumbnail me");
    app.setWindowIcon(QIcon(":/icons/Thumbnailme.ico"));

    MainWindow window;
    window.show();

    return app.exec();
}
