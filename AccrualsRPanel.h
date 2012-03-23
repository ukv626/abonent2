#ifndef ACCRUALSRPANEL_H
#define ACCRUALSRPANEL_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QSqlQueryModel>

class QTableView;
class QSqlRelationalTableModel;

class AccrualsRModel : public QSqlQueryModel {
  Q_OBJECT
    public:
  enum { Date, F1, F2, F3, F4, F5, F6, TPlan, F7, F8, F9, F10, F11 };

  AccrualsRModel(QObject *parent = 0);
  void refresh(const QString &telA, const QString &date);
     
  QVariant data(const QModelIndex &index,
  		int role = Qt::DisplayRole) const;
     
  /* bool select(); */
};



// -- AccrualsRPanel -----------------------------------------------
class AccrualsRPanel : public QWidget
{
    Q_OBJECT

public:
    AccrualsRPanel(QWidget *parent = 0);
    ~AccrualsRPanel();
    void refresh(const QString &telA, const QString &date);


private:
    QTableView *tableView_;
    AccrualsRModel *relModel_;
    //QSqlRelationalTableModel *relModel;
    /* QSortFilterProxyModel *proxyModel; */
};

#endif
