#ifndef SUMMARYCDIALOG_H
#define SUMMARYCDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlTableModel>

class QAction;
class QTableView;

class SummaryCModel : public QSqlTableModel {
  Q_OBJECT

public:
  enum { Date, TelA, F1, F2, F3, F4, F5, F6, Text };
  SummaryCModel(const QString &telA, QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
};


// -- SummaryCDialog -----------------------------------------------
class SummaryCDialog : public QDialog
{
  Q_OBJECT

public:
  SummaryCDialog(const QString &telA, QWidget *parent = 0);
  ~SummaryCDialog();

private slots:
  void insert();
  void remove();

private:
  void updateActions();
  
  QTableView *tableView;
  SummaryCModel *relModel;

  QAction *newAction;
  QAction *removeAction;

  QString telA_;
};

#endif
