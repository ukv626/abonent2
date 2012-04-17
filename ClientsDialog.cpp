#include <QtGui>
#include <QtSql>

#include "ClientsDialog.h"
#include "FinallyDialog.h"
#include "CorrectionsDialog.h"
#include "SqlManager.h"

ClientsModel::ClientsModel(QObject *parent)
  : QSqlRelationalTableModel(parent)
{
  setTable("tb_clients");
  // setRelation(Type, QSqlRelation("tb_clientTypes", "uid", "text"));
  //relModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  select();
  
  setHeaderData(Text, Qt::Horizontal, trUtf8("Наименование"));
  setHeaderData(Address, Qt::Horizontal, trUtf8("Адрес"));
  setHeaderData(Tel, Qt::Horizontal, trUtf8("Телефон"));
  setHeaderData(KPeni, Qt::Horizontal, trUtf8("k пени "));
  setHeaderData(IsActive, Qt::Horizontal, trUtf8("Выст.\nсчета"));
  setHeaderData(IsChecked, Qt::Horizontal, trUtf8("Отмечен"));
  // setHeaderData(Type, Qt::Horizontal, trUtf8("Тип"));
}


QVariant ClientsModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlRelationalTableModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == KPeni)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else if(index.column() == IsActive ||
	    index.column() == IsChecked) {
      if(value.toInt() == 0)
	return trUtf8("Нет");
      else
	return trUtf8("Да");
    }

    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == KPeni ||
	 index.column() == IsActive ||
	 index.column() == IsChecked)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}




// -- ClientsDialog -----------------------------------------------
ClientsDialog::ClientsDialog(QWidget *parent)
  : QDialog(parent)
{
  findLabel = new QLabel(trUtf8("&Поиск"));
  findEdit = new QLineEdit;
  findLabel->setBuddy(findEdit);

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(findLabel);
  topLayout->addWidget(findEdit);
  
  relModel = new ClientsModel;
  proxyModel = new QSortFilterProxyModel;
  // proxyModel->setDynamicSortFilter(false);
  proxyModel->setSourceModel(relModel);
  proxyModel->setFilterKeyColumn(-1);
  proxyModel->sort(ClientsModel::Text, Qt::AscendingOrder);

  connect(findEdit, SIGNAL(textChanged(QString)),
  	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);

  tableView = new QTableView;
  tableView->setModel(proxyModel);

  // tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setColumnHidden(ClientsModel::Id, true);
  tableView->setColumnHidden(ClientsModel::Type, true);
  //tableView->resizeRowsToContents();

  newAction = new QAction(trUtf8("Добавить"), this);
  connect(newAction, SIGNAL(triggered()), this, SLOT(insert()));

  finallyAction = new QAction(trUtf8("История начислений"), this);
  connect(finallyAction, SIGNAL(triggered()), this, SLOT(showFinally()));

  calcAction = new QAction(trUtf8("РАССЧИТАТЬ (+счет)"), this);
  connect(calcAction, SIGNAL(triggered()), this, SLOT(calc()));

  reportAction = new QAction(trUtf8("СЧЕТ"), this);
  connect(reportAction, SIGNAL(triggered()), this, SLOT(report()));

  updateActions();

  tableView->addAction(newAction);
  tableView->addAction(finallyAction);
  tableView->addAction(calcAction);
  tableView->addAction(reportAction);
  
  tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  tableView->setAlternatingRowColors(true);
  
  // tableView->sortByColumn(ClientsModel::Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
  // tableView->setColumnWidth(Clients_CatNum, 160);
  // tableView->setColumnWidth(Clients_Text, 250);
  // tableView->setColumnWidth(Clients_Place, 80);
  // tableView->setColumnWidth(Clients_Brandname, 130);
  // tableView->horizontalHeader()->setStretchLastSection(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(tableView);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Клиенты"));
  setFixedWidth(tableView->horizontalHeader()->length()+50);
  setFixedHeight(380);
}


ClientsDialog::~ClientsDialog()
{
  delete proxyModel;
  delete relModel;
}

void ClientsDialog::updateActions()
{
  if(proxyModel->rowCount()>0) {
    finallyAction->setEnabled(true);
    calcAction->setEnabled(true);
    reportAction->setEnabled(true);
  }
  else {
    finallyAction->setEnabled(false);
    calcAction->setEnabled(false);
    reportAction->setEnabled(false);
  }
}


void ClientsDialog::filterRegExpChanged()
{
  QRegExp regExp(findEdit->text());
  proxyModel->setFilterRegExp(regExp);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  updateActions();
}



void ClientsDialog::insert()
{
  // QItemSelectionModel *selection = tableView->selectionModel();
  // int row = selection->selectedIndexes().first().row();

  QAbstractItemModel *model = tableView->model();
  // QModelIndex index = model->index(0, ClientsModel::Text);
  model->insertRow(model->rowCount());
  // model->setData(model->index(row +1, ClientsModel::Type), 0);
  // model->submit();

  // tableView->setCurrentIndex(index);
  updateActions();
}



/*
void ClientsDialog::showStoragesDialog()
{
  StoragesDialog dialog;
  dialog.exec();
}
*/

void ClientsDialog::showFinally()
{
  QAbstractItemModel *model = tableView->model();
  quint32 id = model->data(model->index(tableView->currentIndex().row(), 0)).toInt();
  QString text = model->data(model->index(tableView->currentIndex().row(),
				       ClientsModel::Text)).toString();

  FinallyDialog dialog(id, text, this);
  dialog.exec();
}


void ClientsDialog::calc()
{
  QAbstractItemModel *model = tableView->model();
  quint32 id = model->data(model->index(tableView->currentIndex().row(), 0)).toInt();
  QString text = model->data(model->index(tableView->currentIndex().row(),
				       ClientsModel::Text)).toString();

  QString error;
  if(ClientsDialog::calculate(id, true, &error))
    QMessageBox::information(this, trUtf8("Информация"),
                     trUtf8("Рассчет окончен!!"), QMessageBox::Ok);
  else
    QMessageBox::warning(this, trUtf8("Ошибка"),
                    trUtf8("Рассчет не был произведен!!<br><br>[%1]").arg(error),
                    QMessageBox::Ok);
}

void ClientsDialog::report()
{
  QAbstractItemModel *model = tableView->model();
  quint32 id = model->data(model->index(tableView->currentIndex().row(), 0)).toInt();
  QString text = model->data(model->index(tableView->currentIndex().row(),
				       ClientsModel::Text)).toString();

  QString error;
  if(ClientsDialog::calculate(id, false, &error))
    QMessageBox::information(this, trUtf8("Информация"),
                    trUtf8("Счет подготовлен!!"), QMessageBox::Ok);
  else
    QMessageBox::warning(this, trUtf8("Ошибка"),
                    trUtf8("Счет НЕ подготовлен!!<br><br>[%1]").arg(error),
                    QMessageBox::Ok);

}


// bool ClientsDialog::getPays(quint32 clientId, const QDate &date, double *sum)
// {
//   bool result = true;
//   QSqlQuery query;
//   query.prepare(" SELECT SUM(p.msum)"
// 		" FROM tb_pays p"
// 		" ,tb_abonents a"
// 		" WHERE p.telA=a.telA"
// 		" AND a.clientID=:clientId"
// 		" AND EXTRACT(YEAR_MONTH FROM p.pdate)=EXTRACT(YEAR_MONTH FROM :pdate)");
//   query.bindValue(":clientId", clientId);
//   query.bindValue(":pdate", date);
//   if(!query.exec())
//     result = false;

//   if(query.next())
//     *sum = query.value(0).toDouble();

//   return result;
// }

// bool ClientsDialog::getCorrections(quint32 clientId,
// 				   const QDate &date, double *sum)
// {
//   bool result = true;
//   QSqlQuery query;
//   query.prepare(" SELECT SUM(c.msum)"
// 		" FROM tb_corrections c"
// 		" WHERE c.clientID=:clientId"
// 		" AND EXTRACT(YEAR_MONTH FROM c.date_)=EXTRACT(YEAR_MONTH FROM :pdate)");
//   query.bindValue(":clientId", clientId);
//   query.bindValue(":pdate", date);
//   if(!query.exec())
//     result = false;

//   if(query.next())
//     *sum = query.value(0).toDouble();

//   return result;
// }

// bool ClientsDialog::getLastSum(quint32 clientId, double *sum)
// {
//   bool result = true;
//   QSqlQuery query(" SELECT DISTINCT date_"
// 		  " FROM tb_summary ORDER BY 1 DESC"
// 		  " LIMIT 1,1");
//   if(!query.next())
//     return false;
  
//   QDate prevDate = query.value(0).toDate();
//   query.prepare(" SELECT asum-psum+corrections+peni+previous"
// 		" FROM tb_finally"
// 		" WHERE clientID=:clientID"
// 		" AND DATE_FORMAT(date_,'%Y-%m-%d')=DATE_FORMAT(:date_,'%Y-%m-%d')");
//   query.bindValue(":clientId", clientId);
//   query.bindValue(":date", prevDate);
//   if(!query.exec())
//     result = false;

//   if(query.next())
//     *sum = query.value(0).toDouble();

//   return result;
// }

// bool ClientsDialog::AddFinally(quint32 clientId, const QDate &date,
// 			       double previous, double aSum,
// 			       double pSum, double corrections, double peni)
// {
//   bool result = true;
//   QSqlQuery query;
//   query.prepare("DELETE FROM tb_finally WHERE date_=:date_ AND clientID=:clientID");
//   query.bindValue(":clientID", clientId);
//   query.bindValue(":date_", date);
//   if(!query.exec())
//     return false;


//   query.prepare("INSERT INTO tb_finally(clientID, date_ , asum, psum, corrections, peni, previous) VALUES(:clientID, :date_ , :asum, :psum, :corrections, :peni, :previous)");
//   query.bindValue(":clientID", clientId);
//   query.bindValue(":date_", date);                               
//   query.bindValue(":asum", aSum);
//   query.bindValue(":psum", pSum);
//   query.bindValue(":corrections", corrections);
//   query.bindValue(":peni", peni);
//   query.bindValue(":previous", previous);
//   if(!query.exec())
//     result = false;

//   return result;
// }

bool ClientsDialog::calculate(quint32 clientId, bool needUpdateFinally,
			      QString *error)
{
  bool failed(false);
  QDate summaryLastDate, summaryPrevDate;
  
  if(!SqlManager::summaryLastDate(&summaryLastDate) ||
     !SqlManager::summaryPrevDate(&summaryPrevDate)) {
    return false;
  }

  if(needUpdateFinally && SqlManager::isMonthClosed(summaryLastDate)) {
    *error = trUtf8("Месяц закрыт!! Изменение невозможно!");
    return false;
  }

  QString paysQueryStr(" SELECT a.clientID,SUM(p.msum)"
		       " FROM tb_pays p"
		       " ,tb_abonents a"
		       " WHERE p.telA=a.telA"
		       " AND EXTRACT(YEAR_MONTH FROM p.pdate)="
		       "     EXTRACT(YEAR_MONTH FROM :date)");

  QString correctionsQueryStr(" SELECT c.clientID,SUM(c.msum)"
			      " FROM tb_corrections c"
			      " WHERE EXTRACT(YEAR_MONTH FROM c.date_)="
			      "       EXTRACT(YEAR_MONTH FROM :date)");

  QString lastSumQueryStr(" SELECT clientID,asum-psum+corrections+peni+previous"
			  " FROM tb_finally"
			  " WHERE DATE_FORMAT(date_,'%Y-%m-%d')="
			  "       DATE_FORMAT(:date,'%Y-%m-%d')");

  QString finallyQueryStr(" DELETE FROM tb_finally"
			  " WHERE date_=:date");

  enum clients { ClientId, ClientText, ClientTel, ClientKPeni, ClientIsActive };
  enum abonents { ATelA, AText, AOperator, ATPlan, AF1, AF2, AF3, AF4, AF5, AF6,
		  AF7, AF8, AF9, AF10, AF11 };

  QString abonentsQueryStr(" SELECT a.telA"
			   " ,a.text"
			   " ,o.text "
			   " ,tp.text"
			   " ,s.f1+IFNULL(sc.f1,0) AS F1"
			   " ,s.f2+IFNULL(sc.f2,0) AS F2"
			   " ,s.f3+IFNULL(sc.f3,0) AS F3"
			   " ,s.f4+IFNULL(sc.f4,0) AS F4"
			   " ,s.f5+IFNULL(sc.f5,0) AS F5"
			   " ,s.f6+IFNULL(sc.f6,0) AS F6"
			   " ,sf.f1+IFNULL(sfc.f1,0) AS F7" 
			   " ,sf.f2+IFNULL(sfc.f2,0) AS F8" 
			   " ,sf.f3+IFNULL(sfc.f3,0) AS F9" 
			   " ,sf.f4+IFNULL(sfc.f4,0) AS F10"
			   " ,s.f1+IFNULL(sc.f1,0)+"
			   "  s.f2+IFNULL(sc.f2,0)+"
			   "   s.f3+IFNULL(sc.f3,0)+"
			   "   s.f4+IFNULL(sc.f4,0)+"
			   "   s.f5+IFNULL(sc.f5,0)+"
			   "   s.f6+IFNULL(sc.f6,0)+"
			   "   sf.f1+IFNULL(sfc.f1,0)+"
			   "   sf.f2+IFNULL(sfc.f2,0)+"
			   "   sf.f3+IFNULL(sfc.f3,0)+"
			   "   sf.f4+IFNULL(sfc.f4,0) AS F11"
			   " FROM tb_abonents a"
			   " ,tb_abonentTypes at"
			   " ,tb_clients cl"
			   " ,tb_operators o"
			   " ,tb_summaryFixP sf" 
			   " ,tb_tplans tp" 
			   " ,tb_summaryP s" 
			   " LEFT JOIN tb_summaryC sc ON"
			   "           (sc.date_=s.date_ AND sc.telA=s.telA)" 
			   " LEFT JOIN tb_summaryFixC sfc ON"
			   "           (sfc.date_=s.date_ AND sfc.telA=s.telA)" 
			   " WHERE a.telA=sf.telA"
			   " AND a.clientID=cl.uid"
			   " AND a.telA=s.telA"
			   " AND a.operatorID=o.uid"
			   " AND a.typeID=at.uid"
			   " AND at.text NOT LIKE '%(!)'"
			   " AND sf.date_=s.date_" 
			   " AND tp.uid=a.tplanID" 
			   " AND s.date_=:date"
			   " AND a.clientID=:clientId"
			   " ORDER BY 1");

  QString clientsQueryStr(" SELECT uid,text,tel,peni_k,isActive"
  			  " FROM tb_clients");

  // если расчет по одному клиенту
  if(clientId>0) {
    paysQueryStr.append(tr(" AND a.clientID=%1").arg(clientId));
    correctionsQueryStr.append(tr(" AND c.clientID=%1").arg(clientId));
    lastSumQueryStr.append(tr(" AND clientID=%1").arg(clientId));
    finallyQueryStr.append(tr(" AND clientID=%1").arg(clientId));
    
    clientsQueryStr.append(tr(" WHERE uid=%1").arg(clientId));
  }
  paysQueryStr.append(" GROUP BY 1");
  correctionsQueryStr.append(" GROUP BY 1");

  QSqlQuery query;
  // get pays
  query.prepare(paysQueryStr);
  query.bindValue(":date", summaryLastDate);
  if(!query.exec()) {
    *error = query.lastError().text();
    return false;
  }
  
  QHash<quint32, double> hashPays;
  while(query.next())
    hashPays[query.value(0).toInt()] = query.value(1).toDouble();

  // get corrections
  query.prepare(correctionsQueryStr);
  query.bindValue(":date", summaryLastDate);
  if(!query.exec()) {
    *error = query.lastError().text();
    return false;
  }
  
  QHash<quint32, double> hashCorrections;
  while(query.next())
    hashCorrections[query.value(0).toInt()] = query.value(1).toDouble();

  // get lastSum
  query.prepare(lastSumQueryStr);
  query.bindValue(":date", summaryPrevDate);
  if(!query.exec()) {
    *error = query.lastError().text();
    return false;
  }
  
  QHash<quint32, double> hashLastSum;
  while(query.next())
    hashLastSum[query.value(0).toInt()] = query.value(1).toDouble();

  
  QSqlQuery clientsQuery(clientsQueryStr);
  // QSqlDatabase db = QSqlDatabase::database();
  // db.transaction();

  // create dir
  QDir dir("./txt/" + summaryLastDate.toString("yyyy-MM-dd"));
  if(!dir.exists())
    QDir("./txt").mkdir(summaryLastDate.toString("yyyy-MM-dd"));

  
  QString iFinallyQuery;
  while(clientsQuery.next()) {
    quint32 clientIdCur = clientsQuery.value(ClientId).toInt();

    // get abonents
    QSqlQuery abonentsQuery;
    abonentsQuery.prepare(abonentsQueryStr);
    abonentsQuery.bindValue(":clientId", clientIdCur);
    abonentsQuery.bindValue(":date", summaryLastDate);
    if(!abonentsQuery.exec()) {
      failed = true;
      *error = abonentsQuery.lastError().text();
      break;
    }

    double fl[11],total[11];
    for(int i=0; i<11; ++i) total[i] = 0;

    QStringList list;

    list << "-----------------------------------------------------------------------------------------------------------------------------------------------------------\r\n"
	 << trUtf8("| Телефон  | Абонент    | Тарифный план              |местные |междугор|   sms  | gprs   |роуминг |роуминг |абонентс|дополнит|тарифные|разовые |  ВСЕГО   |\r\n")
	 << trUtf8("|          |            |                            |вызовы  |вызовы  |        |        |национал|междунар|плата   | услуги |  опции |начис-я |          |\r\n")
	 << "-----------------------------------------------------------------------------------------------------------------------------------------------------------\r\n";

    while(abonentsQuery.next()) {
      for(int i=0; i<11; ++i) {
	fl[i] = abonentsQuery.value(AF1 + i).toDouble();
	total[i] += fl[i];
      }

      list << tr("|%1|%2|%3|%4|%5|%6|%7|%8|%9|%10|%11|%12|%13|%14|\r\n")
	.arg(abonentsQuery.value(ATelA).toString())
	.arg(abonentsQuery.value(AText).toString().left(12), -12)
	.arg(abonentsQuery.value(ATPlan).toString().left(28), -28)
	.arg(fl[0], 8, 'f', 2)
	.arg(fl[1], 8, 'f', 2)
	.arg(fl[2], 8, 'f', 2)
	.arg(fl[3], 8, 'f', 2)
	.arg(fl[4], 8, 'f', 2)
	.arg(fl[5], 8, 'f', 2)
	.arg(fl[6], 8, 'f', 2)
	.arg(fl[7], 8, 'f', 2)
	.arg(fl[8], 8, 'f', 2)
	.arg(fl[9], 8, 'f', 2)
	.arg(fl[10], 10, 'f', 2);
    } // while(abonentsQuery.next())
      
    list << "-----------------------------------------------------------------------------------------------------------------------------------------------------------\r\n"
	 << tr("|%1|%2|%3|%4|%5|%6|%7|%8|%9|%10|%11|%12|\r\n")
      .arg("", 52)
      .arg(total[0], 8, 'f', 2)
      .arg(total[1], 8, 'f', 2)
      .arg(total[2], 8, 'f', 2)
      .arg(total[3], 8, 'f', 2)
      .arg(total[4], 8, 'f', 2)
      .arg(total[5], 8, 'f', 2)
      .arg(total[6], 8, 'f', 2)
      .arg(total[7], 8, 'f', 2)
      .arg(total[8], 8, 'f', 2)
      .arg(total[9], 8, 'f', 2)
      .arg(total[10], 10, 'f', 2);

    double pSum(0), cSum(0), lastSum(0), peni(0), finallySum(0);
    
    pSum = hashPays.value(clientIdCur);
    cSum = hashCorrections.value(clientIdCur);
    lastSum = hashLastSum.value(clientIdCur);

    if((lastSum-pSum)>0)
      peni = (lastSum-pSum)*clientsQuery.value(ClientKPeni).toDouble();

    finallySum = total[10] + lastSum - pSum + peni + cSum;

    list << trUtf8("\n\tСумма предыдущего счета            %1\r\n").arg(lastSum, 10, 'f', 2)
	 << trUtf8("\tСумма платежей в предыдущем месяце %1\r\n").arg(pSum, 10, 'f', 2)
	 << trUtf8("\tЗадолженность предыдущего периода  %1\r\n").arg(lastSum-pSum, 10, 'f', 2)
	 << trUtf8("\tПени на просроченную задолженость  %1\r\n").arg(peni, 10, 'f', 2)
	 << trUtf8("\tКорректировки предыдущего периода  %1\r\n").arg(cSum, 10, 'f', 2)
	 << trUtf8("\tСумма последнего счета             %1\r\n").arg(total[10], 10, 'f', 2)
	 << trUtf8("\n\tИТОГО к оплате                     %1\r\n").arg(finallySum, 10, 'f' ,2);

    iFinallyQuery += tr("(%1,'%2',%3,%4,%5,%6,%7),")
      .arg(clientIdCur)
      .arg(summaryLastDate.toString("yyyy-MM-dd"))
      .arg(total[10], 0, 'f', 2)
      .arg(pSum, 0, 'f', 2)
      .arg(cSum, 0, 'f', 2)
      .arg(peni, 0, 'f', 2)
      .arg(lastSum, 0, 'f', 2);
    
    // Если клиенту нужно выставлять счет
    if(clientsQuery.value(ClientIsActive).toBool()) {
      // удалим старый счет
      QStringList filters(QStringList() << tr("%1*.txt")
	  .arg(clientsQuery.value(ClientText).toString().replace(" ","_")));

      foreach(QString file, dir.entryList(filters, QDir::Files))
	QFile::remove(dir.path() + "/" + file);
      

      QFile file(tr("%1/%2_%3_%4_%5.txt")
		 .arg(dir.path())
		 .arg(clientsQuery.value(ClientText).toString().replace(" ","_"))
		 .arg(clientsQuery.value(ClientTel).toString())
		 .arg(summaryLastDate.toString("MMMM.yyyy"))
		 .arg(finallySum, 0, 'f', 2));
      if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
	QTextStream fout(&file);
	fout.setCodec("Windows-1251");
	foreach(QString line, list)
	  fout << line;
	file.close();
      } // if(file.open())
    }
  } // while(clientsQuery.next())

  // если надо, не было ошибок и есть что добавлять в таблицу tb_finally
  if(needUpdateFinally && \
     !failed &&	!iFinallyQuery.isEmpty()) {
    // удаляем старые записи из tb_finally
    query.prepare(finallyQueryStr);
    query.bindValue(":date", summaryLastDate);
    if(query.exec()) {
      // добавляем новые
      iFinallyQuery.insert(0, "INSERT INTO tb_finally(clientID, date_ , asum, psum, corrections, peni, previous) VALUES ");
      if(iFinallyQuery.endsWith(QChar(',')))
	iFinallyQuery.remove(iFinallyQuery.size() - 1, 1);
    
      query.prepare(iFinallyQuery);
      if(!query.exec())
	failed = true;
    }
    else
      failed = true;
  }

  *error = query.lastError().text();
  return !failed;
}
