#ifndef SERVICESPANEL_H
#define SERVICESPANEL_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QSqlQueryModel>

class QTableView;
class QSqlRelationalTableModel;

class ServicesByTelModel : public QSqlQueryModel {
  Q_OBJECT
    public:
  enum { Text, CostR, Cost };

  ServicesByTelModel(QObject *parent = 0);
  void refresh(const QString &telA);
     
  QVariant data(const QModelIndex &index,
		int role = Qt::DisplayRole) const;
     
};



// -- ServicesPanel -----------------------------------------------
class ServicesPanel : public QWidget
{
    Q_OBJECT

public:
    ServicesPanel(QWidget *parent = 0);
    ~ServicesPanel();
    void refresh(const QString &telA);


private:
    QTableView *tableView_;
    ServicesByTelModel *relModel_;
    //QSqlRelationalTableModel *relModel;
    /* QSortFilterProxyModel *proxyModel; */
};

#endif
