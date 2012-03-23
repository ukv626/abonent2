#ifndef CLIENTSDIALOG_H
#define CLIENTSDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlRelationalTableModel>

class QLabel;
class QLineEdit;
class QTableView;
class QSortFilterProxyModel;
class QSqlRelationalTableModel;


class ClientsModel : public QSqlRelationalTableModel {
  Q_OBJECT

public:
  enum { Id, Text, Address, Tel, KPeni, Type };
  ClientsModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
     
};


// -- ClientsDialog -----------------------------------------------
class ClientsDialog : public QDialog
{
  Q_OBJECT

public:
  ClientsDialog(QWidget *parent = 0);
  ~ClientsDialog();

private slots:
  void filterRegExpChanged();
  void showFinally();
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
  ClientsModel *relModel;
  //QSqlRelationalTableModel *relModel;
  QSortFilterProxyModel *proxyModel;
};

#endif
