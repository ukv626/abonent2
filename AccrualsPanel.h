#ifndef ACCRUALSPANEL_H
#define ACCRUALSPANEL_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QSqlQueryModel>

class QTableView;
class QSqlRelationalTableModel;

class AccrualsModel : public QSqlQueryModel {
  Q_OBJECT
    public:
  enum { Date, F1, F2, F3, F4, F5, F6, TPlan, F7, F8, F9, F10, F11 };

  AccrualsModel(QObject *parent = 0);
  void refresh(const QString &telA);
     
  QVariant data(const QModelIndex &index,
  		int role = Qt::DisplayRole) const;
     
  /* bool select(); */
};



// -- AccrualsPanel -----------------------------------------------
class AccrualsPanel : public QWidget
{
    Q_OBJECT

public:
    AccrualsPanel(QWidget *parent = 0);
    ~AccrualsPanel();
    void refresh(const QString &telA);
    int getHeight();

private:
    QTableView *tableView_;
    AccrualsModel *relModel_;
    //QSqlRelationalTableModel *relModel;
    /* QSortFilterProxyModel *proxyModel; */
};

#endif
