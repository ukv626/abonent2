#ifndef DATEINPUTDIALOG_H
#define DATEINPUTDIALOG_H

#include <QDate>
#include <QDialog>

class QLabel;
class QDateEdit;
class QDialogButtonBox;
class QDate;

class DateInputDialog : public QDialog
{
    Q_OBJECT

public:
  DateInputDialog(QWidget *parent);
  QDate getDate();
    
private:
  QLabel *dateLabel;
  QDateEdit *dateEdit;

  QDialogButtonBox *buttonBox;
};

#endif
