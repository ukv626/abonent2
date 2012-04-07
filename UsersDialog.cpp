#include <QLabel>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>
#include <QMessageBox>

#include "UsersDialog.h"

UsersDialog::UsersDialog(QWidget *parent)
  : QDialog(parent), userId_(255), userGr_(255)
{
  userCombo = new QComboBox;
  userLabel = new QLabel(trUtf8("Пользователь"));
  userLabel->setBuddy(userCombo);

  QSqlQuery query("SELECT uname FROM tb_users ORDER BY 1");
  QStringList users;
  while(query.next()) {
    users << query.value(0).toString();
  }
  userCombo->addItems(users);
  
  passEdit = new QLineEdit;
  passLabel = new QLabel(trUtf8("Пароль"));
  passEdit->setEchoMode(QLineEdit::Password);
  passLabel->setBuddy(passEdit);
  
  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  QGridLayout *topLayout = new QGridLayout;
  topLayout->addWidget(userLabel, 1, 0);
  topLayout->addWidget(userCombo, 1, 1);
  topLayout->addWidget(passLabel, 2, 0);
  topLayout->addWidget(passEdit, 2, 1);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Абонент2: Авторизация"));
}

void UsersDialog::ok()
{
  QCryptographicHash hasher(QCryptographicHash::Md5);
  hasher.addData(passEdit->text().toAscii());
  // qDebug() << passEdit->text() << "|" << hasher.result().toHex();

  QSqlQuery query;
  query.prepare("SELECT uid,upass,ugroup FROM tb_users where uname=:uname");
  query.bindValue(":uname",userCombo->currentText());
  if(!query.exec()) {
    QMessageBox::critical(0, trUtf8("Ошибка"),
		  trUtf8("Нет связи с сервером !! [%1] ").arg(query.lastError().text()));
    return;
  }

  if(query.next()) {
    if(hasher.result().toHex() == query.value(1).toByteArray()) {
      accept();
      userId_ = query.value(0).toInt();
      userGr_ = query.value(2).toInt();
    }
    else {
      passEdit->setText("");
      passEdit->setFocus();
    }
  }
}
