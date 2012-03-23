#ifndef ABONENTSWINDOW_H
#define ABONENTSWINDOW_H

#include <QWidget>
#include <QSqlQueryModel>

class QLabel;
class QLineEdit;
class QComboBox;
class QSortFilterProxyModel;
class QTableView;
class ServicesPanel;
class AccrualsPanel;
class AccrualsRPanel;


class AbonentsQueryModel : public QSqlQueryModel
{
  Q_OBJECT
public:
  enum { Uid, ClientId, Client, TelA, Balance, ASum, ASumR,
	 TPlan, AbonPay, PBalance, Limit, AType };
  
  AbonentsQueryModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
  		int role = Qt::DisplayRole) const;
  void refresh(const QString &atype);
};


// -- AbonentsWindow -----------------------------------------------
class AbonentsWindow : public QWidget
{
    Q_OBJECT

public:
  AbonentsWindow(QWidget *parent = 0);
  ~AbonentsWindow();
  void calculate(const QString &telA);

private slots:
    void filterRegExpChanged();
    void rowChanged(const QModelIndex &, const QModelIndex &);
    void typeChanged();
    void insert();
    void edit();
    void remove();
    void history();
    void summaryC();
    void summaryFixC();
    void calculateMe();
  
private:
    QLabel *findLabel_;
    QLabel *typeLabel_;
    QLineEdit *findEdit_;
    QComboBox *typeComboBox_;

    QTableView *tableView_;
    AbonentsQueryModel *tableModel_;
    QSortFilterProxyModel *proxyModel_;
    ServicesPanel  *servicesPanel_;
    AccrualsPanel  *accrualsPanel_;
    AccrualsRPanel *accrualsRPanel_;
};

#endif
