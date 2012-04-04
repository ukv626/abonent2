#include <QtSql>

#include "SqlManager.h"
#include <QDebug>

SqlManager::SqlManager()
{
  qDebug() << "SqlManager()\n";
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
  query.bindValue("date", date);
  if(query.exec())
    if(query.next())
      result = true;

  return result;
}


