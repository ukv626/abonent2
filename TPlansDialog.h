#ifndef TPLANSDIALOG_H
#define TPLANSDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlTableModel>

class QLabel;
class QLineEdit;
class QTableView;
class QSortFilterProxyModel;


class TPlansModel : public QSqlTableModel {
  Q_OBJECT

public:
  enum { Id, Text, CostR, Cost };
  TPlansModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
     
};


// -- TPlansDialog -----------------------------------------------
class TPlansDialog : public QDialog
{
  Q_OBJECT

public:
  TPlansDialog(QWidget *parent = 0);
  ~TPlansDialog();

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
  TPlansModel *relModel;
  //QSqlRelationalTableModel *relModel;
  QSortFilterProxyModel *proxyModel;
};

#endif
