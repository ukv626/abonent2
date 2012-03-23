#ifndef ABONENTTYPESDIALOG_H
#define ABONENTTYPESDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlTableModel>

class QLabel;
class QLineEdit;
class QTableView;
class QSortFilterProxyModel;


class AbonentTypesModel : public QSqlTableModel {
  Q_OBJECT

public:
  enum { Id, Text, K1, K2, K3, K4, K5, K6 };
  AbonentTypesModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
     
};


// -- AbonentTypesDialog -----------------------------------------------
class AbonentTypesDialog : public QDialog
{
  Q_OBJECT

public:
  AbonentTypesDialog(QWidget *parent = 0);
  ~AbonentTypesDialog();

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
  AbonentTypesModel *relModel;
  //QSqlRelationalTableModel *relModel;
  QSortFilterProxyModel *proxyModel;
};

#endif
