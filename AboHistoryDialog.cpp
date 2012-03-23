#include <QtGui>
#include <QtSql>

#include "AboHistoryDialog.h"
#include "PayDialog.h"


AboHistoryModel::AboHistoryModel(const QString &telA, QObject *parent)
  : QSqlRelationalTableModel(parent)
{
  setTable("tb_abonentsHistory");
  setRelation(User, QSqlRelation("tb_users", "uid", "uname"));
  setFilter(QString("telA=%1").arg(telA));
  select();

  setHeaderData(Client, Qt::Horizontal, trUtf8("Клиент"));
  setHeaderData(Abonent, Qt::Horizontal, trUtf8("Абонент"));
  setHeaderData(TelA, Qt::Horizontal, trUtf8("Телефон"));
  setHeaderData(TPlan, Qt::Horizontal, trUtf8("Тарифный план"));
  setHeaderData(AbonPay, Qt::Horizontal, trUtf8("а/плата"));
  setHeaderData(PBalance, Qt::Horizontal, trUtf8("П.Баланс"));
  setHeaderData(Limit, Qt::Horizontal, trUtf8("Лимит"));
  setHeaderData(Type, Qt::Horizontal, trUtf8("Тип"));
  setHeaderData(Date, Qt::Horizontal, trUtf8("Действ. до"));
  setHeaderData(User, Qt::Horizontal, trUtf8("Сотрудник"));
}

QVariant AboHistoryModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlQueryModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == AbonPay ||
	index.column() == PBalance ||
	index.column() == Limit)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else if (index.column() == Date)
	return value.toDateTime().toString("dd.MM.yyyy hh:mm");
    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == AbonPay ||
	index.column() == PBalance ||
	index.column() == Limit)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}

// -- AboHistoryDialog -----------------------------------------------
AboHistoryDialog::AboHistoryDialog(const QString &telA, QWidget *parent)
  : QDialog(parent)
{
  tableModel_ = new AboHistoryModel(telA, this);
  tableView_ = new QTableView(this);
  tableView_->setModel(tableModel_);

  tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  tableView_->setColumnHidden(AboHistoryModel::Id, true);
  tableView_->setColumnHidden(AboHistoryModel::TelA, true);
  
  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  // tableView_->setColumnWidth(AboHistoryQueryModel::Client, 180);
  tableView_->setAlternatingRowColors(true);
  tableView_->selectRow(0);
  

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(tableView_);
  setLayout(layout);

  setWindowTitle(trUtf8("История изменений"));
  setFixedWidth(tableView_->horizontalHeader()->length()+50);
  setFixedHeight(500);
}

AboHistoryDialog::~AboHistoryDialog()
{
  delete tableModel_;
}


