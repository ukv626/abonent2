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
    void corrections();
    void operators();
    void arcs();

    // operations
    void calc4abonents();
    void calc4clients();

    // reports
    void newAbonents();
    
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

    // file
    QAction *loadServicesAction;
    QAction *loadPaysAction;    
    QAction *exitAction;

    // refs
    QAction *clientsAction;
    QAction *abonentTypesAction;
    QAction *tplansAction;
    QAction *servicesAction;
    QAction *paysAction;
    QAction *correctionsAction;
    QAction *operatorsAction;
    QAction *arcsAction;

    // operations
    QAction *calc4abonentsAction;
    QAction *calc4clientsAction;

    // reports
    QAction *newAbonentsAction;

    QAction *aboutAction;

    quint8 userId_, userGr_;
};

#endif
