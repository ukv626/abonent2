#include <QtGui>
#include <QtSql>

#include "CorrectionDialog.h"

CorrectionDialog::CorrectionDialog(qint32 id, quint8 userId, QWidget *parent)
  : QDialog(parent), userId_(userId) 
{
  clientComboBox = new QComboBox;
  clientLabel = new QLabel(trUtf8("Клиент"));
  clientLabel->setBuddy(clientComboBox);

  telEdit = new QLineEdit;
  telLabel = new QLabel(trUtf8("Телефон"));
  telLabel->setBuddy(telEdit);

  dateLabel = new QLabel(trUtf8("&Дата"));
  dateEdit = new QDateEdit(QDate::currentDate());
  dateEdit->setCalendarPopup(true);
  dateEdit->setDisplayFormat("dd.MM.yyyy");
  dateLabel->setBuddy(dateEdit);

  summaSpinBox = new QDoubleSpinBox;
  summaSpinBox->setRange(-999999999, 999999999);
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
  topLayout->addWidget(clientLabel, 0, 0);
  topLayout->addWidget(clientComboBox, 0, 1);
  topLayout->addWidget(telLabel, 1, 0);
  topLayout->addWidget(telEdit, 1, 1);
  topLayout->addWidget(dateLabel, 2, 0);
  topLayout->addWidget(dateEdit, 2, 1);
  topLayout->addWidget(summaLabel, 3, 0);
  topLayout->addWidget(summaSpinBox, 3, 1);
  topLayout->addWidget(primLabel, 4, 0);
  topLayout->addWidget(primEdit, 4, 1);

  tableModel = new QSqlRelationalTableModel(this);
  tableModel->setTable("tb_corrections");
  tableModel->setRelation(Client, QSqlRelation("tb_clients", "uid", "text"));
  tableModel->setFilter(QString("tb_corrections.uid=%1").arg(id));
  tableModel->select();
  tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  
  clientModel = tableModel->relationModel(Client);
  clientModel->sort(clientModel->fieldIndex("text"), Qt::AscendingOrder);
  clientComboBox->setModel(clientModel);
  clientComboBox->setModelColumn(clientModel->fieldIndex("text"));
  
  mapper = new QDataWidgetMapper(this);
  mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
  mapper->setModel(tableModel);
  mapper->setItemDelegate(new QSqlRelationalDelegate(this));
  mapper->addMapping(clientComboBox, Client);
  mapper->addMapping(telEdit, TelA);
  mapper->addMapping(dateEdit, Date);
  mapper->addMapping(summaSpinBox, Summa);
  mapper->addMapping(primEdit, Prim);
  
  if(id == -1) {
    tableModel->insertRow(0);
    tableModel->setData(tableModel->index(0, UserId), userId_);
    // tableModel->setData(tableModel->index(0, IsManual), 1);
    setWindowTitle(trUtf8("Новая корректировка"));
  }
  else
    setWindowTitle(trUtf8("Редактирование корректировки"));
    
  mapper->toFirst();

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);
}


CorrectionDialog::~CorrectionDialog()
{
  delete tableModel;
}

void CorrectionDialog::OkButtonPushed()
{
  if(mapper->submit())  {
    if(tableModel->submitAll())
      accept();
  }
}

