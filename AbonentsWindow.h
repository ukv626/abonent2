#ifndef ABONENTSWINDOW_H
#define ABONENTSWINDOW_H

#include <QDate>
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
  enum { Uid, ClientId, Client, TelA, Balance, ASum, ASumR, DSum,
	 TPlan, AbonPay, PBalance, Limit, AType };
  
  AbonentsQueryModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
  		int role = Qt::DisplayRole) const;
  void refresh(const QString &atype);
  QDate finallyLastDate();
    
private:
  QDate finallyLastDate_;
};


// -- AbonentsWindow -----------------------------------------------
class AbonentsWindow : public QWidget
{
    Q_OBJECT

public:
  AbonentsWindow(quint8 userId, quint8 userGr, QWidget *parent = 0);
  ~AbonentsWindow();
  void calculate(const QString &telA);

public slots:
  void refresh();

private slots:
  void filterRegExpChanged();
  void rowChanged(const QModelIndex &, const QModelIndex &);
    
  void insert();
  void edit();
  void remove();
  void comments();
  void history();
  void summaryC();
  void summaryFixC();
  void calculateMe();
  
private:
  void updateActions();
  void showConnectionFailed();
  bool calculate1(const QDate& date, const QString &telA);
  bool calculate2(const QDate& date, const QString &telA);
    
  QLabel *findLabel_;
  QLabel *typeLabel_;
  QLabel *countLabel_;
  QLineEdit *findEdit_;
  QComboBox *typeComboBox_;

  QTableView *tableView_;
  AbonentsQueryModel *tableModel_;
  QSortFilterProxyModel *proxyModel_;
  ServicesPanel  *servicesPanel_;
  AccrualsPanel  *accrualsPanel_;
  AccrualsRPanel *accrualsRPanel_;

  QAction *newAction;
  QAction *editAction;
  QAction *commentsAction;
  QAction *removeAction;
  QAction *historyAction;
  QAction *summaryCAction;
  QAction *summaryFixCAction;
  QAction *calcAction;

  quint8 userId_, userGr_;
};

#endif
