#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>
class QDate;

class SqlManager : public QObject {
     Q_OBJECT

 public:
  SqlManager();
  static bool summaryLastDate(QDate *date);
  static bool summaryPrevDate(QDate *date);
  static bool summaryFixLastDate(QDate *date);
  static bool finallyLastDate(QDate *date);
  static bool isMonthClosed(const QDate &date);
  static bool addEmptyServices(const QDate &date);
  static bool updateFreeServices(const QDate &date);
  static bool truncateServicesHistoryR();  
  static bool removeServicesHistoryR2RD(const QDate &date);
  
 /* private: */
 /*  ~SqlManager(); */
};

#endif
