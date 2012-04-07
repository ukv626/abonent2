#include <QtGui>
#include <QtSql>

#include "PayDialog.h"

PayDialog::PayDialog(qint32 id, quint8 userId, QWidget *parent)
  : QDialog(parent), userId_(userId) 
{
  telEdit = new QLineEdit;
  telLabel = new QLabel(trUtf8("Телефон"));
  telLabel->setBuddy(telEdit);

  pdateLabel = new QLabel(trUtf8("&Дата"));
  pdateEdit = new QDateEdit(QDate::currentDate());
  pdateEdit->setCalendarPopup(true);
  pdateEdit->setDisplayFormat("dd.MM.yyyy");
  pdateLabel->setBuddy(pdateEdit);

  summaSpinBox = new QDoubleSpinBox;
  summaSpinBox->setRange(0, 999999999);
  summaLabel = new QLabel(trUtf8("Сумма"));
  summaLabel->setBuddy(summaSpinBox);

  primEdit = new QLineEdit;
  primLabel = new QLabel(trUtf8("Примечание"));
  primLabel->setBuddy(primEdit);

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(OkButtonPushed()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  QGridLayout *topLayout = new QGridLayout;
  topLayout->addWidget(telLabel, 0, 0);
  topLayout->addWidget(telEdit, 0, 1);
  topLayout->addWidget(pdateLabel, 1, 0);
  topLayout->addWidget(pdateEdit, 1, 1);
  topLayout->addWidget(summaLabel, 2, 0);
  topLayout->addWidget(summaSpinBox, 2, 1);
  topLayout->addWidget(primLabel, 3, 0);
  topLayout->addWidget(primEdit, 3, 1);

  tableModel = new QSqlTableModel(this);
  tableModel->setTable("tb_pays");
  tableModel->setFilter(QString("tb_pays.uid=%1").arg(id));
  tableModel->select();
  tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  
  mapper = new QDataWidgetMapper(this);
  mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
  mapper->setModel(tableModel);
  mapper->addMapping(telEdit, TelA);
  mapper->addMapping(pdateEdit, PDate);
  mapper->addMapping(summaSpinBox, Summa);
  mapper->addMapping(primEdit, Prim);
  
  if(id == -1) {
    tableModel->insertRow(0);
    tableModel->setData(tableModel->index(0, UserId), userId_);
    tableModel->setData(tableModel->index(0, IsManual), 1);
    setWindowTitle(trUtf8("Новый платеж"));
  }
  else
    setWindowTitle(trUtf8("Редактирование платежа"));
    
  mapper->toFirst();

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);
}


PayDialog::~PayDialog()
{
  delete tableModel;
}

void PayDialog::OkButtonPushed()
{
  if(mapper->submit())  {
    if(tableModel->submitAll())
      accept();
  }
}

