#ifndef SUMMARYFIXCDIALOG_H
#define SUMMARYFIXCDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlTableModel>

class QAction;
class QTableView;

class SummaryFixCModel : public QSqlTableModel {
  Q_OBJECT

public:
  enum { Date, TelA, F1, F2, F3, F4, Text };
  SummaryFixCModel(const QString &telA, QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
};


// -- SummaryFixCDialog -----------------------------------------------
class SummaryFixCDialog : public QDialog
{
  Q_OBJECT

public:
  SummaryFixCDialog(const QString &telA, QWidget *parent = 0);
  ~SummaryFixCDialog();

private slots:
  void insert();
  void remove();

private:
  void updateActions();
  
  QTableView *tableView;
  SummaryFixCModel *relModel;

  QAction *newAction;
  QAction *removeAction;

  QString telA_;
};

#endif
