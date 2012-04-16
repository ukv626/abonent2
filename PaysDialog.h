#ifndef PAYSDIALOG_H
#define PAYSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

class QLabel;
class QLineEdit;
class QDateEdit;
class QSortFilterProxyModel;
class QTableView;

class PaysQueryModel : public QSqlQueryModel {
     Q_OBJECT
 public:
  enum { Id, Client, TelA, Date, PDate, Summa, Prim, User, IsMan };
  
  PaysQueryModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
  		int role = Qt::DisplayRole) const;
  void refresh(const QDate &date1, const QDate &date2);
};


// -- PaysDialog -----------------------------------------------
class PaysDialog : public QDialog
{
    Q_OBJECT

public:
  PaysDialog(quint8 userId, QWidget *parent = 0);
  ~PaysDialog();

private slots:
  void filterRegExpChanged();
  void date1Changed(const QDate &date);
  void date2Changed(const QDate &date);
  void insert();
  void edit();
  void remove();
  
private:
  void updateActions();

  QLabel *findLabel_;
  QLabel *date1Label_;
  QLabel *date2Label_;
  QLineEdit *findEdit_;
  QDateEdit *date1Edit_;
  QDateEdit *date2Edit_;

  QTableView *tableView_;
  PaysQueryModel *tableModel_;
  QSortFilterProxyModel *proxyModel_;

  QAction *newAction;
  QAction *editAction;
  QAction *removeAction;

  quint8 userId_;
};


#endif
