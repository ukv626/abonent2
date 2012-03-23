#ifndef FINALLYDIALOG_H
#define FINALLYDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QTableView>

class FinallyQueryModel : public QSqlQueryModel {
     Q_OBJECT
 public:
  enum { Date, OSum, ASum, PSum, Correct, Peni, Prev, FSum };
  
  FinallyQueryModel(qint32 id, QObject *parent = 0);
  QVariant data(const QModelIndex &index,
  		int role = Qt::DisplayRole) const;
};


// -- FinallyDialog -----------------------------------------------
class FinallyDialog : public QDialog
{
    Q_OBJECT

public:
  FinallyDialog(qint32 id, const QString &caption, QWidget *parent = 0);
  ~FinallyDialog();

private:
  QTableView *tableView_;
  FinallyQueryModel *tableModel_;
};


#endif
