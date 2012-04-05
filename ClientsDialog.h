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
  enum { Id, Text, Address, Tel, KPeni, Type, IsActive, IsChecked };
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

  static bool getPays(quint32 clientId, const QDate &date, double *pSum);
  static bool getCorrections(quint32 clientId, const QDate &date, double *cSum);
  static bool getLastSum(quint32 clientId, double *cSum);
  static bool AddFinally(quint32 clientId, const QDate &date,
			 double previous, double aSum,
			 double pSum, double corrections, double peni);
  static bool calculate(quint32 clientId);


private slots:
  void filterRegExpChanged();
  void showFinally();
  void showCorrections();
  void newRow();
  void calc();

private:
  QLabel *findLabel;
  QLineEdit *findEdit;
  QTableView *tableView;
  ClientsModel *relModel;
  //QSqlRelationalTableModel *relModel;
  QSortFilterProxyModel *proxyModel;
};

#endif
