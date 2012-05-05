#ifndef SERVICESDIALOG_H
#define SERVICESDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QSqlRelationalTableModel>
#include <QItemDelegate>

class QLabel;
class QLineEdit;
class QTableView;
class QSortFilterProxyModel;
class QSqlRelationalTableModel;

// -- ServicesModel
class ServicesModel : public QSqlRelationalTableModel
{
  Q_OBJECT

public:
  enum { Id, Text, CostR, Cost, Prim, Type, PType, Operator };
  ServicesModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
     
};

// -- ServicesModelTypeDelegate
class ServicesModelTypeDelegate : public QItemDelegate
{
  Q_OBJECT

public:
  ServicesModelTypeDelegate(QObject *parent = 0);
  QWidget *createEditor(QWidget *parent,
			const QStyleOptionViewItem &option,
			const QModelIndex &index) const;
  void setEditorData(QWidget *editor,
		     const QModelIndex &index) const;
  void setModelData(QWidget *editor,
		    QAbstractItemModel *model,
		    const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor,
			    const QStyleOptionViewItem &option,
			    const QModelIndex &index) const;
};

// -- ServicesModelOperatorDelegate
class ServicesModelOperatorDelegate : public QItemDelegate
{
  Q_OBJECT

public:
  ServicesModelOperatorDelegate(QObject *parent = 0);
  QWidget *createEditor(QWidget *parent,
			const QStyleOptionViewItem &option,
			const QModelIndex &index) const;
  void setEditorData(QWidget *editor,
		     const QModelIndex &index) const;
  void setModelData(QWidget *editor,
		    QAbstractItemModel *model,
		    const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor,
			    const QStyleOptionViewItem &option,
			    const QModelIndex &index) const;
};

// -- ServicesDialog -----------------------------------------------
class ServicesDialog : public QDialog
{
  Q_OBJECT

public:
  ServicesDialog(QWidget *parent = 0);
  ~ServicesDialog();

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
  ServicesModel *relModel;
  //QSqlRelationalTableModel *relModel;
  QSortFilterProxyModel *proxyModel;
};

#endif
