#ifndef ABOHISTORYDIALOG_H
#define ABOHISTORYDIALOG_H

#include <QDialog>
#include <QSqlRelationalTableModel>

class QTableView;

class AboHistoryModel : public QSqlRelationalTableModel {
     Q_OBJECT
 public:
  enum { Id, Client, Abonent, TelA, TPlan, AbonPay, Operator, PBalance, Limit, Type, Date, User };
  
  AboHistoryModel(const QString &telA, QObject *parent = 0);
  QVariant data(const QModelIndex &index,
  		int role = Qt::DisplayRole) const;
};


// -- AboHistoryDialog -----------------------------------------------
class AboHistoryDialog : public QDialog
{
    Q_OBJECT

public:
  AboHistoryDialog(const QString &telA, QWidget *parent = 0);
  ~AboHistoryDialog();

private:
  QTableView *tableView_;
  AboHistoryModel *tableModel_;
};


#endif
