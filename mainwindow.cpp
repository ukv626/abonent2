#include <QtGui>
#include <QtSql>

#include "mainwindow.h"
#include "AbonentsWindow.h"
#include "ClientsDialog.h"
#include "AbonentTypesDialog.h"
#include "TPlansDialog.h"
#include "ServicesDialog.h"
#include "PaysDialog.h"

MainWindow::MainWindow()
{
    abonentsWindow = new AbonentsWindow;
    setCentralWidget(abonentsWindow);

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    // createStatusBar();

    //readSettings();

    //findDialog = 0;

    //setWindowIcon(QIcon(":/images/icon.png"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  writeSettings();
  event->accept();
}

/*
void MainWindow::find()
{
    if (!findDialog) {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(findNext(const QString &,
                                            Qt::CaseSensitivity)),
                spreadsheet, SLOT(findNext(const QString &,
                                           Qt::CaseSensitivity)));
        connect(findDialog, SIGNAL(findPrevious(const QString &,
                                                Qt::CaseSensitivity)),
                spreadsheet, SLOT(findPrevious(const QString &,
                                               Qt::CaseSensitivity)));
    }

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}
*/

void MainWindow::clients()
{
  ClientsDialog dialog(this);
  dialog.exec();
}

void MainWindow::abonentTypes()
{
  AbonentTypesDialog dialog(this);
  dialog.exec();
}


void MainWindow::tplans()
{
  TPlansDialog dialog(this);
  dialog.exec();
}

void MainWindow::services()
{
  ServicesDialog dialog(this);
  dialog.exec();
}

void MainWindow::pays()
{
  PaysDialog dialog(this);
  dialog.exec();
}

void MainWindow::calc4abonents()
{
  abonentsWindow->calculate("");
}


void MainWindow::about()
{
    // QMessageBox::about(this, tr("About Spreadsheet"),
    //         tr("<h2>Spreadsheet 1.1</h2>"
    //            "<p>Copyright &copy; 2008 Software Inc."
    //            "<p>Spreadsheet is a small application that "
    //            "demonstrates QAction, QMainWindow, QMenuBar, "
    //            "QStatusBar, QTableWidget, QToolBar, and many other "
    //            "Qt classes."));
}


// void MainWindow::updateStatusBar()
// {
//   locationLabel->setText(tr(""));
//   formulaLabel->setText(tr(""));
// }

/*
void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}
*/


void MainWindow::createActions()
{
    exitAction = new QAction(trUtf8("Выход"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(trUtf8("Выйти из программы"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    clientsAction = new QAction(trUtf8("Клиенты"), this);
    // clientsAction->setStatusTip(trUtf8("Информация по клиентам"));
    connect(clientsAction, SIGNAL(triggered()), this, SLOT(clients()));

    abonentTypesAction = new QAction(trUtf8("Типы абонентов"), this);
    // abonentTypesAction->setStatusTip(trUtf8("Информация по клиентам"));
    connect(abonentTypesAction, SIGNAL(triggered()), this, SLOT(abonentTypes()));

    tplansAction = new QAction(trUtf8("Тарифные планы"), this);
    // tplansAction->setStatusTip(trUtf8("Информация по клиентам"));
    connect(tplansAction, SIGNAL(triggered()), this, SLOT(tplans()));

    servicesAction = new QAction(trUtf8("Услуги"), this);
    // tplansAction->setStatusTip(trUtf8("Информация по клиентам"));
    connect(servicesAction, SIGNAL(triggered()), this, SLOT(services()));

    paysAction = new QAction(trUtf8("Платежи"), this);
    paysAction->setStatusTip(trUtf8("Информация по платежам"));
    connect(paysAction, SIGNAL(triggered()), this, SLOT(pays()));

    // monthsAction = new QAction(trUtf8("Архив"), this);
    // monthsAction->setStatusTip(trUtf8("Информация по закрытым месяцам"));
    // connect(monthsAction, SIGNAL(triggered()), this, SLOT(months()));

    calc4abonentsAction = new QAction(trUtf8("Рассчитать по всем абонентам"), this);
    // calc4abonentsAction->setStatusTip(trUtf8("Информация по платежам"));
    connect(calc4abonentsAction, SIGNAL(triggered()), this, SLOT(calc4abonents()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(trUtf8("&Файл"));
    //fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    refMenu = menuBar()->addMenu(trUtf8("&Справочники"));
    refMenu->addAction(clientsAction);
    refMenu->addAction(abonentTypesAction);
    refMenu->addAction(tplansAction);
    refMenu->addAction(servicesAction);
    refMenu->addAction(paysAction);
    // refMenu->addAction(monthsAction);


    //editMenu->addSeparator();
    
    

    toolsMenu = menuBar()->addMenu(trUtf8("&Инструменты"));
    toolsMenu->addAction(calc4abonentsAction);
    // toolsMenu->addAction(dealsAction);
    // toolsMenu->addAction(storagesAction);
    // toolsMenu->addAction(paysAction);
    //toolsMenu->addAction(recalculateAction);
    //toolsMenu->addAction(sortAction);

    reportsMenu = menuBar()->addMenu(trUtf8("&Отчеты"));
    //optionsMenu->addAction(showGridAction);
    //optionsMenu->addAction(autoRecalcAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(trUtf8("&Помощь"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createContextMenu()
{
    // spreadsheet->addAction(cutAction);
    // spreadsheet->addAction(copyAction);
    // spreadsheet->addAction(pasteAction);
    // spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    // fileToolBar = addToolBar(tr("&File"));
    // fileToolBar->addAction(newAction);
    // fileToolBar->addAction(openAction);
    // fileToolBar->addAction(saveAction);

    // editToolBar = addToolBar(tr("&Edit"));
    // editToolBar->addAction(cutAction);
    // editToolBar->addAction(copyAction);
    // editToolBar->addAction(pasteAction);
    // editToolBar->addSeparator();
    // editToolBar->addAction(findAction);
    // editToolBar->addAction(goToCellAction);
}

// void MainWindow::createStatusBar()
// {
//     locationLabel = new QLabel(" W999 ");
//     locationLabel->setAlignment(Qt::AlignHCenter);
//     locationLabel->setMinimumSize(locationLabel->sizeHint());

//     formulaLabel = new QLabel;
//     formulaLabel->setIndent(3);

//     statusBar()->addWidget(locationLabel);
//     statusBar()->addWidget(formulaLabel, 1);

//     // connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)),
//     //         this, SLOT(updateStatusBar()));
//     // connect(spreadsheet, SIGNAL(modified()),
//     //         this, SLOT(spreadsheetModified()));

//     updateStatusBar();
// }

void MainWindow::readSettings()
{
    // QSettings settings("Software Inc.", "Spreadsheet");

    // restoreGeometry(settings.value("geometry").toByteArray());

    // recentFiles = settings.value("recentFiles").toStringList();
    // updateRecentFileActions();

    // bool showGrid = settings.value("showGrid", true).toBool();
    // showGridAction->setChecked(showGrid);

    // bool autoRecalc = settings.value("autoRecalc", true).toBool();
    // autoRecalcAction->setChecked(autoRecalc);
}

void MainWindow::writeSettings()
{
    // QSettings settings("Software Inc.", "Spreadsheet");

    // settings.setValue("geometry", saveGeometry());
    // settings.setValue("recentFiles", recentFiles);
    // settings.setValue("showGrid", showGridAction->isChecked());
    // settings.setValue("autoRecalc", autoRecalcAction->isChecked());
}

