#include <QLabel>
#include <QDialogButtonBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QVBoxLayout>

#include "DateInputDialog.h"

DateInputDialog::DateInputDialog(QWidget *parent)
  : QDialog(parent)
{
  dateEdit = new QDateEdit(QDate::currentDate());
  dateEdit->setCalendarPopup(true);
  dateEdit->setDisplayFormat("dd.MM.yyyy");
  dateLabel = new QLabel(trUtf8("Дата"));
  dateLabel->setBuddy(dateEdit);

  
  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  QGridLayout *topLayout = new QGridLayout;
  topLayout->addWidget(dateLabel, 1, 0);
  topLayout->addWidget(dateEdit, 1, 1);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Выберите дату"));
}

QDate DateInputDialog::getDate()
{
  return dateEdit->date();
}
