#include <QtGui>
#include <QtSql>

#include "AbonentsWindow.h"
#include "AbonentDialog.h"
#include "ServicesPanel.h"
#include "AccrualsPanel.h"
#include "AccrualsRPanel.h"
#include "AboHistoryDialog.h"
#include "SummaryCDialog.h"
#include "SummaryFixCDialog.h"

#include "SqlManager.h"

AbonentsQueryModel::AbonentsQueryModel(QObject *parent)
  : QSqlQueryModel(parent), finallyLastDate_(1900, 1, 1)
{
  if(SqlManager::finallyLastDate(&finallyLastDate_))
    refresh("%(!)");
}

QVariant AbonentsQueryModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlQueryModel::data(index, role);
  switch (role) {
      case Qt::FontRole:  
    if(index.column() == ASum) {
      const QAbstractItemModel *model = index.model();
      double asum = model->data(model->index(index.row(), ASum)).toDouble();
      double asumr= model->data(model->index(index.row(), ASumR)).toDouble();
      if(asum < asumr )
	return qVariantFromValue(QFont("default", 10, QFont::Bold));
      
      else
	return value;
    }
    else
      return value;

  case Qt::DisplayRole:
    if (index.column() == Balance ||
	index.column() == ASum ||
	index.column() == ASumR ||
	index.column() == AbonPay ||
	index.column() == PBalance ||
	index.column() == Limit)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == Balance ||
	 index.column() == ASum ||
	 index.column() == ASumR ||
	 index.column() == AbonPay ||
	 index.column() == PBalance ||
	 index.column() == Limit)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}

QDate AbonentsQueryModel::finallyLastDate()
{
  return finallyLastDate_;
}

void AbonentsQueryModel::refresh(const QString &atype)
{
  QSqlQuery query;
  query.prepare(" SELECT a.uid"
		" ,a.clientID"
		" ,c.text AS Client"
		" ,a.telA"
		" ,f.asum-f.psum+f.corrections+f.peni+f.previous AS Balance"
		" ,(SELECT s.f1+IFNULL(sc.f1,0)+"
		"          s.f2+IFNULL(sc.f2,0)+"
		"          s.f3+IFNULL(sc.f3,0)+"
		"          s.f4+IFNULL(sc.f4,0)+"
		"          s.f5+IFNULL(sc.f5,0)+"
		"          s.f6+IFNULL(sc.f6,0)+"
		"          sf.f1+IFNULL(sfc.f1,0)+"
		"          sf.f2+IFNULL(sfc.f2,0)+"
		"          sf.f3+IFNULL(sfc.f3,0)+"
		"          sf.f4+IFNULL(sfc.f4,0)"
		"   FROM tb_summaryFixP sf" 
		"   ,tb_summaryP s" 
		"   LEFT JOIN tb_summaryC sc ON (sc.date_=s.date_ AND sc.telA=s.telA)" 
		"   LEFT JOIN tb_summaryFixC sfc ON (sfc.date_=s.date_ AND sfc.telA=s.telA)" 
		"   WHERE a.telA=sf.telA" 
		"   AND a.telA=s.telA" 
		"   AND sf.date_=s.date_" 
		"   AND sf.date_=f.date_) AS ASum"

		" ,(SELECT s.f1+s.f2+s.f3+s.f4+s.f5+s.f6 +"
		"   sf.f1+sf.f2+sf.f3+sf.f4"
		"   FROM tb_summaryFix sf"
		"   ,tb_summary s"
		"   WHERE a.telA=sf.telA"
		"   AND a.telA=s.telA"
		"   AND sf.date_=s.date_"
		"   AND sf.date_=f.date_) AS ASumR"
		  
		" ,tp.text AS tplan"
		" ,a.abonPay"
		" ,a.pbalance"
		" ,a.limit_"
		" ,at.text AS atype"
		" FROM tb_abonents a"
		" ,tb_clients c"
		" ,tb_abonentTypes at"
		" ,tb_tplans tp"
		" ,tb_finally f"
		" WHERE a.clientID=c.uid"
		" AND a.typeID=at.uid"
		" AND a.tplanID=tp.uid"
		" AND c.uid=f.clientID"
		" AND f.date_=:date"
		" AND at.text NOT LIKE :atype"
		" ORDER BY 3,4");
  query.bindValue(":date", finallyLastDate_);
  query.bindValue(":atype", atype);
  query.exec();

  setQuery(query);

  setHeaderData(Client, Qt::Horizontal, trUtf8("Клиент"));
  setHeaderData(TelA, Qt::Horizontal, trUtf8("Телефон"));
  setHeaderData(Balance, Qt::Horizontal, finallyLastDate_);
  setHeaderData(ASum, Qt::Horizontal, trUtf8("Начисл."));
  setHeaderData(ASumR, Qt::Horizontal, trUtf8("Реально"));
  setHeaderData(TPlan, Qt::Horizontal, trUtf8("Тарифный план"));
  setHeaderData(AbonPay, Qt::Horizontal, trUtf8("а/плата"));
  setHeaderData(PBalance, Qt::Horizontal, trUtf8("П.Баланс"));
  setHeaderData(Limit, Qt::Horizontal, trUtf8("Лимит"));
  setHeaderData(AType, Qt::Horizontal, trUtf8("Тип"));
}


// -- AbonentsWindow -----------------------------------------------
AbonentsWindow::AbonentsWindow(QWidget *parent)
  : QWidget(parent)
{
  findLabel_ = new QLabel(trUtf8("&Поиск"));
  findEdit_ = new QLineEdit;
  findLabel_->setBuddy(findEdit_);

  typeLabel_ = new QLabel(trUtf8("&Показать"));
  typeComboBox_ = new QComboBox;
  typeLabel_->setBuddy(findEdit_);

  countLabel_ = new QLabel;

  // QSqlQuery query("SELECT uid,text FROM tb_abonentTypes ORDER BY 1");
  QStringList items(QStringList() << trUtf8("Действующие") << trUtf8("Все"));
  // while(query.next()) {
  //   items << query.value(1).toString();
  // }
  typeComboBox_->addItems(items);

  connect(typeComboBox_, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(typeChanged()), Qt::UniqueConnection);
    
  QHBoxLayout *leftTopLayout = new QHBoxLayout;
  leftTopLayout->addWidget(findLabel_);
  leftTopLayout->addWidget(findEdit_);
  leftTopLayout->addWidget(typeLabel_);
  leftTopLayout->addWidget(typeComboBox_);
  leftTopLayout->addWidget(countLabel_);

  tableView_ = new QTableView(this);
  tableModel_ = new AbonentsQueryModel(this);

  proxyModel_ = new QSortFilterProxyModel;
  // proxyModel->setDynamicSortFilter(false);
  proxyModel_->setSourceModel(tableModel_);
  proxyModel_->setFilterKeyColumn(-1);
  //proxyModel->sort(Details_Text, Qt::AscendingOrder);

  connect(findEdit_, SIGNAL(textChanged(QString)),
  	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);
  
  tableView_->setModel(proxyModel_);
  tableView_->setItemDelegate(new QSqlRelationalDelegate(tableView_));

  tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  tableView_->setColumnHidden(AbonentsQueryModel::Uid, true);
  tableView_->setColumnHidden(AbonentsQueryModel::ClientId, true);
  tableView_->setColumnHidden(AbonentsQueryModel::PBalance, true);
  tableView_->setColumnHidden(AbonentsQueryModel::Limit, true);
  
  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  tableView_->setColumnWidth(AbonentsQueryModel::Client, 180);
  tableView_->setAlternatingRowColors(true);
  
  servicesPanel_ = new ServicesPanel;
  servicesPanel_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  servicesPanel_->setFixedWidth(320);

  accrualsPanel_ = new AccrualsPanel;
  accrualsRPanel_ = new AccrualsRPanel;
  // accrualsRPanel_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  accrualsPanel_->setFixedHeight(135);
  accrualsRPanel_->setFixedHeight(95);
  
  connect(tableView_->selectionModel(),
          SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
          SLOT(rowChanged(const QModelIndex &, const QModelIndex &)));

  // tableView_->setSortingEnabled(true);
  tableView_->selectRow(0);
  
  QAction *newAction = new QAction(trUtf8("Добавить.."), this);
  newAction->setShortcut(tr("Ins"));
  connect(newAction, SIGNAL(triggered()), this, SLOT(insert()));
  
  QAction *editAction = new QAction(trUtf8("Редактировать.."), this);
  connect(editAction, SIGNAL(triggered()), this, SLOT(edit()));

  // QAction *removeAction = new QAction(trUtf8("Удалить"), this);
  // connect(removeAction, SIGNAL(triggered()), this, SLOT(remove()));

  QAction *historyAction = new QAction(trUtf8("История изменений"), this);
  connect(historyAction, SIGNAL(triggered()), this, SLOT(history()));

  QAction *summaryCAction = new QAction(trUtf8("Корректировки начислений"), this);
  connect(summaryCAction, SIGNAL(triggered()), this, SLOT(summaryC()));

  QAction *summaryFixCAction = new QAction(trUtf8("Корректировки фикс. начислений"), this);
  connect(summaryFixCAction, SIGNAL(triggered()), this, SLOT(summaryFixC()));

  QAction *calcAction = new QAction(trUtf8("РАССЧИТАТЬ"), this);
  connect(calcAction, SIGNAL(triggered()), this, SLOT(calculateMe()));


  tableView_->addAction(newAction);
  tableView_->addAction(editAction);
  // tableView_->addAction(removeAction);
  tableView_->addAction(historyAction);
  tableView_->addAction(summaryCAction);
  tableView_->addAction(summaryFixCAction);
  tableView_->addAction(calcAction);
  
  tableView_->setContextMenuPolicy(Qt::ActionsContextMenu);
  countLabel_->setText(tr("<B>%1</B>").arg(proxyModel_->rowCount()));
  

  QVBoxLayout *leftLayout = new QVBoxLayout;
  leftLayout->addLayout(leftTopLayout);
  leftLayout->addWidget(tableView_);

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addLayout(leftLayout);
  topLayout->addWidget(servicesPanel_);

  QVBoxLayout *layout = new QVBoxLayout;
  // layout->setContentsMargins(1, 1, 1, 1);
  layout->addLayout(topLayout);
  layout->addWidget(accrualsPanel_);
  layout->addWidget(accrualsRPanel_);
  
  setLayout(layout);
 
  setWindowTitle(trUtf8("Абоненты"));
  // setFixedWidth(tableView_->horizontalHeader()->length()+50);
}

AbonentsWindow::~AbonentsWindow()
{
  delete tableModel_;
  delete servicesPanel_;
  delete accrualsRPanel_;
}


void AbonentsWindow::filterRegExpChanged()
{
  QRegExp regExp(findEdit_->text());
  proxyModel_->setFilterRegExp(regExp);
  proxyModel_->setFilterCaseSensitivity(Qt::CaseInsensitive);
  // qDebug() << "qwe";
}


void AbonentsWindow::typeChanged()
{
  // QRegExp regExp(proxyModel_->filterRegExp().pattern() +
  // 		 tr("&^%1$").arg(typeComboBox_->currentText()));
  // qDebug() << regExp.pattern();
  // proxyModel_->setFilterRegExp(regExp);

  int i = typeComboBox_->currentIndex();
  if(i == 0)
    tableModel_->refresh("%(!)");
  else
    tableModel_->refresh("");

  countLabel_->setText(tr("<B>%1</B>").arg(proxyModel_->rowCount()));
}


void AbonentsWindow::rowChanged(const QModelIndex &current,
                                const QModelIndex & /* previous */)
{
  if(current.row()<0)
    return;

  QAbstractItemModel *model = tableView_->model();
  QString telA = model->data(model->index(current.row(), AbonentsQueryModel::TelA)).toString();

  servicesPanel_->refresh(telA, tableModel_->finallyLastDate().toString("yyyy-MM-dd"));
  accrualsPanel_->refresh(telA);
  accrualsRPanel_->refresh(telA);
}

void AbonentsWindow::insert()
{
  AbonentDialog dialog(-1, this);
  if(dialog.exec() == QDialog::Accepted)
    typeChanged();

}

void AbonentsWindow::edit()
{
  QAbstractItemModel *model = tableView_->model();
  if(model->rowCount()>0) {

     QItemSelectionModel *selection = tableView_->selectionModel();
     int curRow = selection->selectedIndexes().first().row();
    int id = model->data(model->index(tableView_->currentIndex().row(), 0)).toInt();
    
    AbonentDialog dialog(id, this);
    if(dialog.exec() == QDialog::Accepted) {
      typeChanged();
      tableView_->selectRow(curRow);
    }
  }
}

void AbonentsWindow::remove()
{
  QAbstractItemModel *model = tableView_->model();
  if(model->rowCount()>0) {

    quint32 id = model->data(model->index(tableView_->currentIndex().row(), 0)).toInt();
    QString atext = model->data(model->index(tableView_->currentIndex().row(),
					     AbonentsQueryModel::TelA)).toString();

    int r = QMessageBox::warning(this, trUtf8("Подтверждение"),
		trUtf8("Действительно удалить абонента %1?").arg(atext),
				 QMessageBox::Yes, QMessageBox::No |
				 QMessageBox::Default | QMessageBox::Escape);
  
    if (r == QMessageBox::No)
      return;

    qDebug() << id;
  }
  // QAbstractItemModel *model = tableView_->model();
  // qDebug() << tableModel_->removeRows(row, 1);
  // if(!tableModel_->submitAll())
  //   QMessageBox::warning(this, trUtf8("Ошибка"),
  // 			 trUtf8("Операция не выполнена!!"), QMessageBox::Ok);

}

void AbonentsWindow::history()
{
  QAbstractItemModel *model = tableView_->model();
  if(model->rowCount()>0) {
     QString telA = model->data(model->index(tableView_->currentIndex().row(),
				       AbonentsQueryModel::TelA)).toString();
    
    AboHistoryDialog dialog(telA, this);
    dialog.exec();
  }
}

void AbonentsWindow::summaryC()
{
  QAbstractItemModel *model = tableView_->model();
  if(model->rowCount()>0) {
     QString telA = model->data(model->index(tableView_->currentIndex().row(),
				       AbonentsQueryModel::TelA)).toString();
    
    SummaryCDialog dialog(telA, this);
    dialog.exec();
  }
}

void AbonentsWindow::summaryFixC()
{
  QAbstractItemModel *model = tableView_->model();
  if(model->rowCount()>0) {
     QString telA = model->data(model->index(tableView_->currentIndex().row(),
				       AbonentsQueryModel::TelA)).toString();
    
    SummaryFixCDialog dialog(telA, this);
    dialog.exec();
  }
}

void AbonentsWindow::showConnectionFailed()
{
  QMessageBox::warning(this, trUtf8("Ошибка"),
		       trUtf8("Нет связи с БД!!"),
		       QMessageBox::Ok);
}

bool AbonentsWindow::calculate1(const QDate& date, const QString &telA)
{
  bool result = false;
  QSqlQuery query;
  QString queryString = " DELETE FROM tb_summaryP WHERE date_=:date_";
  if(!telA.isEmpty())
    queryString += tr(" AND telA=%1").arg(telA);

  query.prepare(queryString);
  query.bindValue(":date_", date);
  if(query.exec()) {
    queryString = " INSERT INTO tb_summaryP "
      " SELECT s.telA"
      " ,CASE WHEN at.k1<0 THEN s.f1*at.k1*(-1)"
      "  ELSE at.k1 END AS F1"
      " ,CASE WHEN at.k2<0 THEN s.f2*at.k2*(-1)"
      "  ELSE at.k2 END AS F2"
      " ,CASE WHEN at.k3<0 THEN s.f3*at.k3*(-1)"
      "  ELSE at.k3 END AS F3"
      " ,CASE WHEN at.k4<0 THEN s.f4*at.k4*(-1)"
      "  ELSE at.k4 END AS F4"
      " ,CASE WHEN at.k5<0 THEN s.f5*at.k5*(-1)"
      "  ELSE at.k5 END AS F5"
      " ,CASE WHEN at.k6<0 THEN s.f6*at.k6*(-1)"
      "  ELSE at.k6 END AS F6"
      " ,s.date_"
      " FROM tb_abonents a"
      " ,tb_abonentTypes at"
      " ,tb_summary s"
      " WHERE a.telA=s.telA"
      " AND a.typeID=at.uid"
      " AND s.date_=:date";

    if(!telA.isEmpty())
      queryString += tr(" AND a.telA=%1").arg(telA);
    
    query.prepare(queryString);
    query.bindValue(":date", date);
    if(query.exec())
      result = true;
  }

  return result;
}

bool AbonentsWindow::calculate2(const QDate& date, const QString &telA)
{
  bool result = false;
  
  QSqlQuery query;
  QString queryString = " DELETE FROM tb_summaryFixP WHERE date_=:date";
  if(!telA.isEmpty())
    queryString += tr(" AND telA=%1").arg(telA);

  query.prepare(queryString);
  query.bindValue(":date", date);
  if(query.exec()) {
    queryString = " INSERT INTO tb_summaryFixP "
      " SELECT sh.date_"
      " ,sh.telA"
      " ,a.tplanID"
      " ,SUM(CASE WHEN s.typeID=1 THEN sh.cost ELSE 0 END)+a.abonPay"
      " ,SUM(CASE WHEN s.typeID=2 or s.typeID=0 THEN sh.cost ELSE 0 END)"
      " ,SUM(CASE WHEN s.typeID=3 THEN sh.cost ELSE 0 END)"
      " ,SUM(CASE WHEN s.typeID=4 THEN sh.cost ELSE 0 END)"
      " FROM tb_servicesHistoryRD sh"
      " ,tb_services s"
      " ,tb_abonents a"
      " WHERE 1=1"
      " AND sh.telA=a.telA"
      " AND sh.serviceID=s.uid"
      " AND sh.date_=:date";

    if(!telA.isEmpty())
      queryString += tr(" AND a.telA=%1").arg(telA);
  
    queryString += " GROUP BY 1,2,3";

    query.prepare(queryString);
    query.bindValue(":date", date);
    if(query.exec())
      result = true;
  }

  return result;
}


void AbonentsWindow::calculate(const QString &telA)
{
  QDate summaryLastDate;
  QSqlQuery query;

  if(!SqlManager::summaryLastDate(&summaryLastDate)) {
    showConnectionFailed();
    return;
  }
  
  if(SqlManager::isMonthClosed(summaryLastDate)) {
    QMessageBox::warning(this, trUtf8("Внимание"),
  			 trUtf8("Месяц закрыт!! Изменение невозможно!"),
  			 QMessageBox::Ok);
    return;
  }

  QSqlDatabase db = QSqlDatabase::database();
  db.transaction();
  if(calculate1(summaryLastDate, telA) && calculate2(summaryLastDate, telA)) {
    db.commit();
    QMessageBox::information(this, trUtf8("Информация"),
                         trUtf8("Рассчет окончен!!"),
                         QMessageBox::Ok);
  }
  else {
    db.rollback();
    QMessageBox::warning(this, trUtf8("Ошибка"),
                         trUtf8("Рассчет не был произведен!!"),
                         QMessageBox::Ok);
  }
}

void AbonentsWindow::calculateMe()
{
  QAbstractItemModel *model = tableView_->model();
  if(model->rowCount()>0) {
    QString telA = model->data(model->index(tableView_->currentIndex().row(),
					    AbonentsQueryModel::TelA)).toString();
    calculate(telA);
  }
}
