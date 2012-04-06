#include <QtSql>

#include "SqlManager.h"
#include <QDebug>
#include <QMessageBox>

SqlManager::SqlManager()
{
  qDebug() << "SqlManager()\n";
}

bool SqlManager::summaryPrevDate(QDate *date)
{
  bool result(true);
  QSqlQuery query(" SELECT DISTINCT date_"
		  " FROM tb_summary ORDER BY 1 DESC"
		  " LIMIT 1,1");
  if(query.next())
    *date = query.value(0).toDate();
  else
    result = false;

  return result;
}

bool SqlManager::summaryLastDate(QDate *date)
{
  bool result(true);
  QSqlQuery query("SELECT MAX(date_) FROM tb_summary");
  if(query.next())
    *date = query.value(0).toDate();
  else
    result = false;

  return result;
}


bool SqlManager::summaryFixLastDate(QDate *date)
{
  bool result(true);
  QSqlQuery query("SELECT MAX(date_) FROM tb_summaryFix");
  if(query.next())
    *date = query.value(0).toDate();
  else
    result = false;

  return result;
}


bool SqlManager::finallyLastDate(QDate *date)
{
  bool result(true);
  QSqlQuery query("SELECT MAX(date_) FROM tb_finally");
  if(query.next())
    *date = query.value(0).toDate();
  else
    result = false;

  return result;
}

bool SqlManager::isMonthClosed(const QDate &date)
{
  bool result(false);
  QSqlQuery query;
  query.prepare("SELECT 1 FROM tb_arcs WHERE date_=:date");
  query.bindValue(":date", date);
  if(query.exec())
    if(query.next())
      result = true;

  return result;
}

bool SqlManager::addEmptyServices(const QDate &date)
{
  bool result(false);
  QSqlQuery query;
  query.prepare(" INSERT INTO tb_servicesHistoryRD"
		" SELECT NULL, a.telA, :date1, 65, 0, 0"
		" FROM tb_abonents a"
		" WHERE a.telA NOT IN"
		"  (SELECT DISTINCT telA"
		"   FROM tb_servicesHistoryRD"
		"   WHERE date_=:date2)");
  query.bindValue(":date1", date);
  query.bindValue(":date2", date);
  if(query.exec())
      result = true;
  else
    qDebug() << "addEmptyServices: " << query.lastError().text();

  return result;
}

bool SqlManager::updateFreeServices(const QDate &date)
{
  bool result(false);
  QSqlQuery query;
  query.prepare(" UPDATE tb_servicesHistoryRD"
		" SET cost=0"
		" WHERE date_=:date"
		" AND serviceID IN (SELECT uid FROM tb_services WHERE ptype=1)");
  query.bindValue(":date", date);
  if(query.exec())
      result = true;
  else
    qDebug() << "updateFreeServices: " << query.lastError().text();

  return result;
}

bool SqlManager::truncateServicesHistoryR()
{
  bool result(false);
  QSqlQuery query;
  query.prepare(" DELETE FROM tb_servicesHistoryR");
  if(query.exec())
      result = true;
  else
    qDebug() << "truncateServicesHistoryR: " << query.lastError().text();

  return result;  
}

bool SqlManager::removeServicesHistoryR2RD(const QDate &date){
  bool result(false);
  QSqlQuery query;
  query.prepare(" INSERT INTO tb_servicesHistoryRD"
		" SELECT 0,sh.telA,:date_,sh.serviceID"
		" ,SUM(sh.costR),SUM(sh.cost) "
		" FROM tb_services s,tb_servicesHistoryR sh"
		" WHERE sh.serviceID=s.uid"
		" GROUP BY 1,2,3,4");

  query.bindValue(":date", date);
  if(query.exec())
    result = SqlManager::truncateServicesHistoryR();
  else
    QMessageBox::critical(0, trUtf8("Ошибка"),
	trUtf8("Ошибка при добавлении записей в таблицу tb_servicesHistoryRD !! [%1]")
			  .arg(query.lastError().text()));


  return result;  
}

