#ifndef CORRECTIONSDIALOG_H
#define CORRECTIONSDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlRelationalTableModel>

class QLabel;
class QLineEdit;
class QDateEdit;
class QSortFilterProxyModel;
class QTableView;

class CorrectionsModel : public QSqlRelationalTableModel {
  Q_OBJECT

public:
  enum { Id, Client, TelA, Date, Sum, Text };
  CorrectionsModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
  void refresh(const QDate &date1, const QDate &date2 );
};


// -- CorrectionsDialog -----------------------------------------------
class CorrectionsDialog : public QDialog
{
  Q_OBJECT

public:
  CorrectionsDialog(quint8 userId, QWidget *parent = 0);
  ~CorrectionsDialog();

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
  CorrectionsModel *tableModel_;
  QSortFilterProxyModel *proxyModel_;

  QAction *newAction;
  QAction *editAction;
  QAction *removeAction;

  quint8 userId_;
};

#endif
