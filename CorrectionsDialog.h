#ifndef CORRECTIONSDIALOG_H
#define CORRECTIONSDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlTableModel>

class QTableView;

class CorrectionsModel : public QSqlTableModel {
  Q_OBJECT

public:
  enum { Id, ClientId, TelA, Date, Sum, Text };
  CorrectionsModel(quint32 clientId, QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
};


// -- CorrectionsDialog -----------------------------------------------
class CorrectionsDialog : public QDialog
{
  Q_OBJECT

public:
  CorrectionsDialog(quint32 clientId, const QString &caption, QWidget *parent = 0);
  ~CorrectionsDialog();

private slots:
  void newRow();
  void removeRow();

private:
  QTableView *tableView;
  CorrectionsModel *relModel;

  quint32 clientId_;
};

#endif
