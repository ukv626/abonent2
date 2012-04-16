#include <QtGui>
#include <QtSql>

#include "CorrectionsDialog.h"
#include "CorrectionDialog.h"
#include "SqlManager.h"

CorrectionsModel::CorrectionsModel(QObject *parent)
  : QSqlRelationalTableModel(parent)
{
}


QVariant CorrectionsModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlTableModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == Sum)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else if (index.column() == Date)
      return value.toDateTime().toString("dd.MM.yyyy");
    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == Sum)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}

void CorrectionsModel::refresh(const QDate &date1, const QDate &date2 )
{
  setTable("tb_corrections");
  setRelation(Client, QSqlRelation("tb_clients", "uid", "text"));
  setFilter(QString("tb_corrections.date_ BETWEEN '%1' AND '%2'").arg(date1.toString("yyyy-MM-dd")).arg(date2.toString("yyyy-MM-dd")));
  select();

  setHeaderData(Client, Qt::Horizontal, trUtf8("Клиент"));
  setHeaderData(TelA, Qt::Horizontal, trUtf8("Телефон"));
  setHeaderData(Date, Qt::Horizontal, trUtf8("Дата"));
  setHeaderData(Sum, Qt::Horizontal, trUtf8("Сумма"));
  setHeaderData(Text, Qt::Horizontal, trUtf8("Примечание"));
}

// -- CorrectionsDialog -----------------------------------------------
CorrectionsDialog::CorrectionsDialog(quint8 userId, QWidget *parent)
  : QDialog(parent), userId_(userId)
{
  tableModel_ = new CorrectionsModel(this);
  tableModel_->refresh(QDate::currentDate(), QDate::currentDate());
  
  proxyModel_ = new QSortFilterProxyModel;
  proxyModel_->setSourceModel(tableModel_);
  proxyModel_->setFilterKeyColumn(-1);
  proxyModel_->sort(CorrectionsModel::Date, Qt::AscendingOrder);

  tableView_ = new QTableView(this);
  tableView_->setModel(proxyModel_);
  tableView_->setItemDelegate(new QSqlRelationalDelegate(tableView_));
  tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  tableView_->setColumnHidden(CorrectionsModel::Id, true);
  
  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  tableView_->setColumnWidth(CorrectionsModel::Client, 180);
  tableView_->setColumnWidth(CorrectionsModel::TelA, 90);
  tableView_->setColumnWidth(CorrectionsModel::Date, 85);
  tableView_->setColumnWidth(CorrectionsModel::Sum, 80);
  tableView_->setColumnWidth(CorrectionsModel::Text, 250);
  // tableView_->setColumnWidth(CorrectionsModel::User, 120);

  tableView_->setAlternatingRowColors(true);
  tableView_->selectRow(0);
  
  newAction = new QAction(trUtf8("Добавить.."), this);
  newAction->setShortcut(tr("Ins"));
  connect(newAction, SIGNAL(triggered()), this, SLOT(insert()));
  
  editAction = new QAction(trUtf8("Редактировать.."), this);
  connect(editAction, SIGNAL(triggered()), this, SLOT(edit()));

  removeAction = new QAction(trUtf8("Удалить"), this);
  connect(removeAction, SIGNAL(triggered()), this, SLOT(remove()));

  updateActions();

  // QDate summaryLastDate;
  // SqlManager::summaryLastDate(&summaryLastDate);
  // if(SqlManager::isMonthClosed(summaryLastDate)) {
  //   newAction->setEnabled(false);
  //   editAction->setEnabled(false);
  //   removeAction->setEnabled(false);
  // }


  tableView_->addAction(newAction);
  tableView_->addAction(editAction);
  tableView_->addAction(removeAction);
  tableView_->setContextMenuPolicy(Qt::ActionsContextMenu);

  findLabel_ = new QLabel(trUtf8("&Поиск"));
  findEdit_ = new QLineEdit;
  findLabel_->setBuddy(findEdit_);
  connect(findEdit_, SIGNAL(textChanged(QString)),
  	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);

  date1Label_ = new QLabel(trUtf8("&Дата"));
  date1Edit_ = new QDateEdit;
  date1Edit_->setCalendarPopup(true);
  date1Edit_->setDisplayFormat("dd.MM.yyyy");
  date1Label_->setBuddy(date1Edit_);
  date1Edit_->setDate(QDate::currentDate());
  connect(date1Edit_, SIGNAL(dateChanged(const QDate &)),
          this, SLOT(date1Changed(const QDate &)), Qt::UniqueConnection);

  date2Label_ = new QLabel(trUtf8("&Дата"));
  date2Edit_ = new QDateEdit;
  date2Edit_->setCalendarPopup(true);
  date2Edit_->setDisplayFormat("dd.MM.yyyy");
  date2Label_->setBuddy(date2Edit_);
  date2Edit_->setDate(QDate::currentDate());
  connect(date2Edit_, SIGNAL(dateChanged(const QDate &)),
          this, SLOT(date2Changed(const QDate &)), Qt::UniqueConnection);
  
  
  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(findLabel_);
  topLayout->addWidget(findEdit_);
  topLayout->addWidget(date1Label_);
  topLayout->addWidget(date1Edit_);
  topLayout->addWidget(date2Label_);
  topLayout->addWidget(date2Edit_);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(topLayout);
  layout->addWidget(tableView_);
  setLayout(layout);

  setWindowTitle(trUtf8("Корректировки"));
  setFixedWidth(tableView_->horizontalHeader()->length()+50);
  setFixedHeight(500);
}


CorrectionsDialog::~CorrectionsDialog()
{
  delete tableModel_;
}


void CorrectionsDialog::filterRegExpChanged()
{
  QRegExp regExp(findEdit_->text());
  proxyModel_->setFilterRegExp(regExp);
  proxyModel_->setFilterCaseSensitivity(Qt::CaseInsensitive);
  updateActions();
}

void CorrectionsDialog::updateActions()
{
  if(proxyModel_->rowCount()>0) {
    editAction->setEnabled(true);
    removeAction->setEnabled(true);
  }
  else {
    editAction->setEnabled(false);
    removeAction->setEnabled(false);
  }
}

void CorrectionsDialog::date1Changed(const QDate &date)
{
  tableModel_->refresh(date, date2Edit_->date());
  updateActions();
}

void CorrectionsDialog::date2Changed(const QDate &date)
{
  tableModel_->refresh(date1Edit_->date(), date);
  updateActions();
}


void CorrectionsDialog::insert()
{
  CorrectionDialog dialog(-1, userId_, this);
  if(dialog.exec() == QDialog::Accepted) {
    tableModel_->refresh(date1Edit_->date(), date2Edit_->date());
    updateActions();
  }
}


void CorrectionsDialog::edit()
{
  QAbstractItemModel *model = tableView_->model();
  if(model->rowCount()>0) {

    QItemSelectionModel *selection = tableView_->selectionModel();
    int curRow = selection->selectedIndexes().first().row();
    int id = model->data(model->index(tableView_->currentIndex().row(), 0)).toInt();
    
    CorrectionDialog dialog(id, userId_, this);
    if(dialog.exec() == QDialog::Accepted) {
      tableModel_->refresh(date1Edit_->date(), date2Edit_->date());
      tableView_->selectRow(curRow);
    }
  }
}

void CorrectionsDialog::remove()
{
  QAbstractItemModel *model = tableView_->model();
  if(model->rowCount() > 0) {
    int id = model->data(model->index(tableView_->currentIndex().row(), 0)).toInt();

    int r = QMessageBox::warning(this, trUtf8("Подтверждение"),
				 trUtf8("Действительно удалить корректировку?"),
  				 QMessageBox::Yes, QMessageBox::No |
  				 QMessageBox::Default | QMessageBox::Escape);
  
    if (r == QMessageBox::No)
      return;

    QSqlQuery query;
    query.prepare("DELETE FROM tb_corrections WHERE uid=:id");
    query.bindValue(":id", id);
    if(!query.exec())
      QMessageBox::warning(this, trUtf8("Ошибка"),
			   trUtf8("Запись не удалена!!"), QMessageBox::Ok);
    else {
      tableModel_->refresh(date1Edit_->date(), date2Edit_->date());
      updateActions();
    }
  }

}


