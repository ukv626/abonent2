#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QTableView>
#include <QAction>
#include <QHeaderView>
#include <QApplication>
#include <QInputDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDataWidgetMapper>
#include <QtSql>

#include "AbonentDialog.h"

AbonentDialog::AbonentDialog(qint32 id, quint8 userId, QWidget *parent)
  : QDialog(parent), userId_(userId) 
{
  clientComboBox = new QComboBox;
  clientLabel = new QLabel(trUtf8("Клиент"));
  clientLabel->setBuddy(clientComboBox);

  abonentEdit = new QLineEdit;
  abonentLabel = new QLabel(trUtf8("Абонент"));
  abonentLabel->setBuddy(abonentEdit);

  telEdit = new QLineEdit;
  telLabel = new QLabel(trUtf8("Телефон"));
  telLabel->setBuddy(telEdit);

  tplanComboBox = new QComboBox;
  tplanLabel = new QLabel(trUtf8("ТП"));
  tplanLabel->setBuddy(tplanComboBox);

  abonPaySpinBox = new QDoubleSpinBox;
  abonPaySpinBox->setRange(0, 999999999);
  abonPayLabel = new QLabel(trUtf8("Абон.плата"));
  abonPayLabel->setBuddy(abonPaySpinBox);

  operatorComboBox = new QComboBox;
  operatorLabel = new QLabel(trUtf8("Оператор"));
  operatorLabel->setBuddy(operatorComboBox);

  pBalanceSpinBox = new QDoubleSpinBox;
  pBalanceSpinBox->setRange(0, 999999999);
  pBalanceLabel = new QLabel(trUtf8("П.баланс"));
  pBalanceLabel->setBuddy(pBalanceSpinBox);

  limitSpinBox = new QDoubleSpinBox;
  limitSpinBox->setRange(0, 999999999);
  limitLabel = new QLabel(trUtf8("Лимит"));
  limitLabel->setBuddy(limitSpinBox);

  typeComboBox = new QComboBox;
  typeLabel = new QLabel(trUtf8("ТП"));
  typeLabel->setBuddy(typeComboBox);

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(OkButtonPushed()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  QGridLayout *topLayout = new QGridLayout;
  topLayout->addWidget(clientLabel, 0, 0);
  topLayout->addWidget(clientComboBox, 0, 1);
  topLayout->addWidget(abonentLabel, 1, 0);
  topLayout->addWidget(abonentEdit, 1, 1);
  topLayout->addWidget(telLabel, 2, 0);
  topLayout->addWidget(telEdit, 2, 1);
  topLayout->addWidget(tplanLabel, 3, 0);
  topLayout->addWidget(tplanComboBox, 3, 1);
  topLayout->addWidget(abonPayLabel, 4, 0);
  topLayout->addWidget(abonPaySpinBox, 4, 1);
  topLayout->addWidget(pBalanceLabel, 5, 0);
  topLayout->addWidget(pBalanceSpinBox, 5, 1);
  topLayout->addWidget(limitLabel, 6, 0);
  topLayout->addWidget(limitSpinBox, 6, 1);
  topLayout->addWidget(typeLabel, 7, 0);
  topLayout->addWidget(typeComboBox, 7, 1);
  topLayout->addWidget(operatorLabel, 8, 0);
  topLayout->addWidget(operatorComboBox, 8, 1);

  tableModel = new QSqlRelationalTableModel(this);
  tableModel->setTable("tb_abonents");
  tableModel->setRelation(Client, QSqlRelation("tb_clients", "uid", "text"));
  tableModel->setRelation(TPlan, QSqlRelation("tb_tplans", "uid", "text"));
  tableModel->setRelation(Operator, QSqlRelation("tb_operators", "uid", "text"));
  tableModel->setRelation(Type, QSqlRelation("tb_abonentTypes", "uid", "text"));
  tableModel->setFilter(QString("tb_abonents.uid=%1").arg(id));
  tableModel->select();
  tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  
  clientModel = tableModel->relationModel(Client);
  clientModel->sort(clientModel->fieldIndex("text"), Qt::AscendingOrder);
  clientComboBox->setModel(clientModel);
  clientComboBox->setModelColumn(clientModel->fieldIndex("text"));

  tplanModel = tableModel->relationModel(TPlan);
  tplanModel->sort(tplanModel->fieldIndex("text"), Qt::AscendingOrder);
  tplanComboBox->setModel(tplanModel);
  tplanComboBox->setModelColumn(tplanModel->fieldIndex("text"));
  connect(tplanComboBox, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(tplanChanged()), Qt::UniqueConnection);

  for(qint32 i=0; i < tplanModel->rowCount(); ++i) {
    QString text = tplanModel->data(tplanModel->index(i, 1)).toString();
    double costR=tplanModel->data(tplanModel->index(i, 2)).toDouble();
    hashTplans[text] = costR;
  }

  operatorModel = tableModel->relationModel(Operator);
  operatorModel->sort(operatorModel->fieldIndex("text"), Qt::AscendingOrder);
  operatorComboBox->setModel(operatorModel);
  operatorComboBox->setModelColumn(tplanModel->fieldIndex("text"));

  typeModel = tableModel->relationModel(Type);
  typeModel->sort(typeModel->fieldIndex("text"), Qt::AscendingOrder);
  typeComboBox->setModel(typeModel);
  typeComboBox->setModelColumn(typeModel->fieldIndex("text"));
  
  mapper = new QDataWidgetMapper(this);
  mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
  mapper->setModel(tableModel);
  mapper->setItemDelegate(new QSqlRelationalDelegate(this));

  mapper->addMapping(clientComboBox, Client);
  mapper->addMapping(abonentEdit, Abonent);
  mapper->addMapping(telEdit, TelA);
  mapper->addMapping(tplanComboBox, TPlan);
  mapper->addMapping(abonPaySpinBox, AbonPay);
  mapper->addMapping(pBalanceSpinBox, PBalance);
  mapper->addMapping(limitSpinBox, Limit);
  mapper->addMapping(typeComboBox, Type);
  mapper->addMapping(operatorComboBox, Operator);
  
  if(id == -1) {
    tableModel->insertRow(0);
    setWindowTitle(trUtf8("Новый абонент"));
  }
  else
    setWindowTitle(trUtf8("Редактирование абонента"));

  mapper->toFirst();

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);

  
}


AbonentDialog::~AbonentDialog()
{
  //delete queryModel;
  //delete proxyModel;
}

void AbonentDialog::tplanChanged()
{
  abonPaySpinBox->setValue(hashTplans.value(tplanComboBox->currentText()));
  //placeModel->setFilter(QString("mflag=%1").arg(typeCheckBox->isChecked()));
  //placeModel->select();
}


void AbonentDialog::OkButtonPushed()
{
  QString Client1 = tableModel->data(tableModel->index(0, Client)).toString();
  QString Abonent1 = tableModel->data(tableModel->index(0, Abonent)).toString();
  QString TelA1 = tableModel->data(tableModel->index(0, TelA)).toString();
  QString TPlan1 = tableModel->data(tableModel->index(0, TPlan)).toString();
  double AbonPay1 = tableModel->data(tableModel->index(0, AbonPay)).toDouble();
  QString Operator1 = tableModel->data(tableModel->index(0, Operator)).toString();
  double PBalance1 = tableModel->data(tableModel->index(0, PBalance)).toDouble();
  double Limit1 = tableModel->data(tableModel->index(0, Limit)).toDouble();
  QString Type1 = tableModel->data(tableModel->index(0, Type)).toString();

  if(mapper->submit())  {
    
    if(tableModel->submitAll()) {
      QSqlQuery query;
      query.prepare("INSERT INTO tb_abonentsHistory(client,text,telA,tplan,abonPay,operator,pbalance,limit_,type,userID) VALUES(:client,:text,:telA,:tplan,:abonPay,:operator,:pbalance,:limit_,:type,:userID)");

      query.bindValue(":client", Client1);
      query.bindValue(":text", Abonent1);
      query.bindValue(":telA", TelA1);
      query.bindValue(":tplan", TPlan1);
      query.bindValue(":abonPay", AbonPay1);
      query.bindValue(":operator", Operator1);
      query.bindValue(":pbalance", PBalance1);
      query.bindValue(":limit_", Limit1);
      query.bindValue(":type", Type1);
      query.bindValue(":userID", userId_);
      query.exec();

      accept();
    }
  }
}

