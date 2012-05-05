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
  query.prepare("SELECT isClosed FROM tb_arcs WHERE date_=:date");
  query.bindValue(":date", date);
  if(query.exec())
    if(query.next())
      result = query.value(0).toInt();

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

bool SqlManager::servicesHistory2summary(const QDate& date)
{
  bool result = false;
  
  QSqlQuery query;
  QString queryString = " DELETE FROM tb_summary WHERE date_=:date"    
    " AND LEFT(telA,3) IN ('911','981')";

  query.prepare(queryString);
  query.bindValue(":date", date);
  if(query.exec()) {
    queryString = " INSERT INTO tb_summary "
      " SELECT sh.telA"
      " ,SUM(CASE WHEN s.typeID=5 THEN sh.costR ELSE 0 END)"
      " ,SUM(CASE WHEN s.typeID=6 THEN sh.costR ELSE 0 END)"
      " ,SUM(CASE WHEN s.typeID=7 THEN sh.costR ELSE 0 END)"
      " ,SUM(CASE WHEN s.typeID=8 THEN sh.costR ELSE 0 END)"
      " ,SUM(CASE WHEN s.typeID=9 THEN sh.costR ELSE 0 END)"
      " ,SUM(CASE WHEN s.typeID=10 THEN sh.costR ELSE 0 END)"
      " ,sh.date_"
      " FROM tb_servicesHistoryRD sh"
      " ,tb_services s"
      " WHERE 1=1"
      " AND sh.serviceID=s.uid"
      " AND sh.date_=:date"
      " AND LEFT(sh.telA,3) IN ('911','981')";

    queryString += " GROUP BY sh.telA,sh.date_";

    query.prepare(queryString);
    query.bindValue(":date", date);
    if(query.exec())
      	  result = true;
    else
      QMessageBox::critical(0, trUtf8("Ошибка"),
	trUtf8("Ошибка при переносе записей из tb_servicesHistoryRD в tb_summary !! [%1]").arg(query.lastError().text()));

  }

  return result;
}

bool SqlManager::servicesHistory2summaryFix(const QDate& date)
{
  bool result = false;
  
  QSqlQuery query;
  QString queryString = " DELETE FROM tb_summaryFix WHERE date_=:date"    
    " AND LEFT(telA,3) IN ('911','981')";

  query.prepare(queryString);
  query.bindValue(":date", date);
  if(query.exec()) {
    queryString = " INSERT INTO tb_summaryFix "
      " SELECT sh.telA"
      " ,''"
      " ,SUM(CASE WHEN s.typeID=1 THEN sh.costR ELSE 0 END)"
      " ,SUM(CASE WHEN s.typeID=2 or s.typeID=0 THEN sh.costR ELSE 0 END)"
      " ,SUM(CASE WHEN s.typeID=3 THEN sh.costR ELSE 0 END)"
      " ,SUM(CASE WHEN s.typeID=4 THEN sh.costR ELSE 0 END)"
      " ,sh.date_"
      " FROM tb_servicesHistoryRD sh"
      " ,tb_services s"
      " WHERE 1=1"
      " AND sh.serviceID=s.uid"
      " AND sh.date_=:date"
      " AND LEFT(sh.telA,3) IN ('911','981')";

    queryString += " GROUP BY sh.telA,2,sh.date_";

    query.prepare(queryString);
    query.bindValue(":date", date);
    if(query.exec())
      	  result = true;
    else
      QMessageBox::critical(0, trUtf8("Ошибка"),
	trUtf8("Ошибка при переносе записей из tb_servicesHistoryRD в tb_summaryFix !! [%1]").arg(query.lastError().text()));
  }

  return result;
}


