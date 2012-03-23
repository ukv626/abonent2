#include <QtGui>
#include <QtSql>

#include "mainwindow.h"


bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    // db.setHostName("192.168.20.169");
    db.setDatabaseName("db_kilofon");
    db.setUserName("k_user");
    db.setPassword("k_user");

    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
        return false;
    }
    return true;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!createConnection())
        return 1;

    MainWindow mainWin;
    // mainWin.resize(1100, 800);
    mainWin.setWindowState(Qt::WindowMaximized);
    mainWin.show();

    return app.exec();
}
