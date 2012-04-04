#include <QtGui>
#include <QtSql>

#include "mainwindow.h"
#include "UsersDialog.h"


bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QSettings settings("abonent2.conf", QSettings::IniFormat);
    db.setHostName(settings.value("ip", "127.0.0.1").toString());
    db.setDatabaseName("db_kilofon");
    db.setUserName("k_user");
    db.setPassword("k_user");
    // db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=5");

    if(!db.open()) {
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

    UsersDialog dlg;
    app.processEvents();
    
    if(dlg.exec() != QDialog::Accepted)
      return -1;
    
    MainWindow mainWin(dlg.getUserGroup());
    // mainWin.resize(1100, 800);
    mainWin.setWindowState(Qt::WindowMaximized);
    mainWin.show();


    return app.exec();
}
