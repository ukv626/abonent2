#ifndef ABONENTSNOTEDIALOG_H
#define ABONENTSNOTEDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlTableModel>

class QTableView;

class AboCommentsModel : public QSqlTableModel {
  Q_OBJECT

public:
  enum { Uid, TelA, Date, Text };
  AboCommentsModel(const QString &telA, QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
};


// -- AboCommentsDialog -----------------------------------------------
class AboCommentsDialog : public QDialog
{
  Q_OBJECT

public:
  AboCommentsDialog(const QString &telA, QWidget *parent = 0);
  ~AboCommentsDialog();

private slots:
  void newRow();
  void removeRow();

private:
  QTableView *tableView;
  AboCommentsModel *relModel;

  QString telA_;
};

#endif
