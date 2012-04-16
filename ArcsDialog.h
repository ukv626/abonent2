#ifndef ARCSDIALOG_H
#define ARCSDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlTableModel>

class QLabel;
class QLineEdit;
class QTableView;
class QSortFilterProxyModel;


class ArcsModel : public QSqlTableModel {
  Q_OBJECT

public:
  enum { Date, IsClosed };
  ArcsModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
};


// -- ArcsDialog -----------------------------------------------
class ArcsDialog : public QDialog
{
  Q_OBJECT

public:
  ArcsDialog(QWidget *parent = 0);
  ~ArcsDialog();

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
  ArcsModel *relModel;
  //QSqlRelationalTableModel *relModel;
  QSortFilterProxyModel *proxyModel;
};

#endif
