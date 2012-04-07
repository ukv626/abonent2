#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class AbonentsWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  MainWindow(quint8 userId, quint8 userGr);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    // file
    void loadServices();
    void loadPays();
    
    // refs
    void clients();
    void abonentTypes();
    void tplans();
    void services();
    void pays();
    /* void months(); */

    // operations
    void calc4abonents();
    void calc4clients();
    
    void about();
    /* void updateStatusBar(); */

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    /* void createStatusBar(); */
    void readSettings();
    void writeSettings();

    AbonentsWindow *abonentsWindow;
    /* QLabel *locationLabel; */
    /* QLabel *formulaLabel; */

    QMenu *fileMenu;
    QMenu *refMenu;
    QMenu *toolsMenu;
    QMenu *reportsMenu;
    QMenu *helpMenu;
    
    //QToolBar *fileToolBar;
    //QToolBar *editToolBar;

    QAction *loadServicesAction;
    QAction *loadPaysAction;    
    QAction *exitAction;
    QAction *clientsAction;
    QAction *abonentTypesAction;
    QAction *tplansAction;
    QAction *servicesAction;
    QAction *paysAction;

    QAction *calc4abonentsAction;
    QAction *calc4clientsAction;
    /* QAction *monthsAction; */
    QAction *aboutAction;

    quint8 userId_, userGr_;
};

#endif
