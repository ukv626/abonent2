#ifndef OPERATORSDIALOG_H
#define OPERATORSDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlTableModel>

class QLabel;
class QLineEdit;
class QTableView;
class QSortFilterProxyModel;


class OperatorsModel : public QSqlTableModel {
  Q_OBJECT

public:
  enum { Id, Text };
  OperatorsModel(QObject *parent = 0);
};


// -- OperatorsDialog -----------------------------------------------
class OperatorsDialog : public QDialog
{
  Q_OBJECT

public:
  OperatorsDialog(QWidget *parent = 0);
  ~OperatorsDialog();

private slots:
  void filterRegExpChanged();
  /* void showMovesDialog(); */
  void newRow();
  /*
    void showStoragesDialog();
    void showAddRequestDialog();
    void showRequestsDialog();
    //void sort(int);
    */

private:
  QLabel *findLabel;
  QLineEdit *findEdit;
  QTableView *tableView;
  OperatorsModel *relModel;
  //QSqlRelationalTableModel *relModel;
  QSortFilterProxyModel *proxyModel;
};

#endif
