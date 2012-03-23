#include <QtGui>
#include <QtSql>

#include "PaysDialog.h"
#include "PayDialog.h"
// #include "AbonentDialog.h"


PaysQueryModel::PaysQueryModel(QObject *parent)
  : QSqlQueryModel(parent)
{
}

QVariant PaysQueryModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlQueryModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == Summa)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else if (index.column() == Date)
	return value.toDateTime().toString("dd.MM.yyyy hh:mm");
    else if (index.column() == PDate)
	return value.toDateTime().toString("dd.MM.yyyy");
    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == Summa)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}

void PaysQueryModel::refresh(const QDate &date1, const QDate &date2 )
{
  QSqlQuery query;
  query.prepare(" SELECT p.uid"
		" ,c.text"
		" ,p.telA"
		" ,p.date_"
		" ,p.pdate"
		" ,p.msum"
		" ,p.text"
		" FROM tb_pays p"
		" ,tb_abonents a"
		" ,tb_clients c"
		" WHERE p.telA=a.telA"
		" AND a.clientID=c.uid"
		" AND p.pdate>=:pdate1"
		" AND p.pdate<=:pdate2");

    query.bindValue(":pdate1", date1);
    query.bindValue(":pdate2", date2);
    query.exec();

    setQuery(query);
    setHeaderData(Client, Qt::Horizontal, trUtf8("Клиент"));
    setHeaderData(TelA, Qt::Horizontal, trUtf8("Телефон"));
    setHeaderData(Date, Qt::Horizontal, trUtf8("Проведен"));
    setHeaderData(PDate, Qt::Horizontal, trUtf8("Дата"));
    setHeaderData(Summa, Qt::Horizontal, trUtf8("Сумма"));
    setHeaderData(Prim, Qt::Horizontal, trUtf8("Примечание"));
}


// -- PaysDialog -----------------------------------------------
PaysDialog::PaysDialog(QWidget *parent)
  : QDialog(parent)
{
  findLabel_ = new QLabel(trUtf8("&Поиск"));
  findEdit_ = new QLineEdit;
  findLabel_->setBuddy(findEdit_);

  tableModel_ = new PaysQueryModel(this);

  date1Label_ = new QLabel(trUtf8("&Дата"));
  date1Edit_ = new QDateEdit;
  date1Edit_->setCalendarPopup(true);
  date1Edit_->setDisplayFormat("dd.MM.yyyy");
  date1Label_->setBuddy(date1Edit_);

  connect(date1Edit_, SIGNAL(dateChanged(const QDate &)),
          this, SLOT(date1Changed(const QDate &)), Qt::UniqueConnection);

  date2Label_ = new QLabel(trUtf8("&Дата"));
  date2Edit_ = new QDateEdit;
  date2Edit_->setCalendarPopup(true);
  date2Edit_->setDisplayFormat("dd.MM.yyyy");
  date2Label_->setBuddy(date2Edit_);

  connect(date2Edit_, SIGNAL(dateChanged(const QDate &)),
          this, SLOT(date1Changed(const QDate &)), Qt::UniqueConnection);
  
  date1Edit_->setDate(QDate::currentDate());
  date2Edit_->setDate(QDate::currentDate());
  
  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(findLabel_);
  topLayout->addWidget(findEdit_);
  topLayout->addWidget(date1Label_);
  topLayout->addWidget(date1Edit_);
  topLayout->addWidget(date2Label_);
  topLayout->addWidget(date2Edit_);

  connect(findEdit_, SIGNAL(textChanged(QString)),
  	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);

  proxyModel_ = new QSortFilterProxyModel;
  // proxyModel->setDynamicSortFilter(false);
  proxyModel_->setSourceModel(tableModel_);
  proxyModel_->setFilterKeyColumn(-1);
  proxyModel_->sort(PaysQueryModel::Client, Qt::AscendingOrder);

  tableView_ = new QTableView(this);
  tableView_->setModel(proxyModel_);
  tableView_->setItemDelegate(new QSqlRelationalDelegate(tableView_));

  tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  tableView_->setColumnHidden(PaysQueryModel::Id, true);
  
  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  // tableView_->setColumnWidth(PaysQueryModel::Client, 180);
  tableView_->setAlternatingRowColors(true);
  tableView_->selectRow(0);
  
  QAction *newAction = new QAction(trUtf8("Добавить.."), this);
  newAction->setShortcut(tr("Ins"));
  connect(newAction, SIGNAL(triggered()), this, SLOT(insert()));
  
  QAction *editAction = new QAction(trUtf8("Редактировать.."), this);
  connect(editAction, SIGNAL(triggered()), this, SLOT(edit()));

  // QAction *removeAction = new QAction(trUtf8("Удалить"), this);
  // connect(removeAction, SIGNAL(triggered()), this, SLOT(remove()));

  tableView_->addAction(newAction);
  tableView_->addAction(editAction);
  // tableView_->addAction(removeAction);
  tableView_->setContextMenuPolicy(Qt::ActionsContextMenu);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(topLayout);
  layout->addWidget(tableView_);
  setLayout(layout);


  setWindowTitle(trUtf8("Платежи"));
  setFixedWidth(tableView_->horizontalHeader()->length()+50);
  setFixedHeight(500);
}

PaysDialog::~PaysDialog()
{
  delete tableModel_;
}


void PaysDialog::filterRegExpChanged()
{
  QRegExp regExp(findEdit_->text());
  proxyModel_->setFilterRegExp(regExp);
  proxyModel_->setFilterCaseSensitivity(Qt::CaseInsensitive);
  // qDebug() << "qwe";
}


void PaysDialog::date1Changed(const QDate &date)
{
  tableModel_->refresh(date, date2Edit_->date());
}

void PaysDialog::date2Changed(const QDate &date)
{
  tableModel_->refresh(date1Edit_->date(), date);
}


void PaysDialog::insert()
{
  PayDialog dialog(-1, this);
  if(dialog.exec() == QDialog::Accepted)
    tableModel_->refresh(date1Edit_->date(), date2Edit_->date());
}

void PaysDialog::edit()
{
  QAbstractItemModel *model = tableView_->model();
  if(model->rowCount()>0) {

    QItemSelectionModel *selection = tableView_->selectionModel();
    int curRow = selection->selectedIndexes().first().row();
    int id = model->data(model->index(tableView_->currentIndex().row(), 0)).toInt();
    
    PayDialog dialog(id, this);
    if(dialog.exec() == QDialog::Accepted) {
      tableModel_->refresh(date1Edit_->date(), date2Edit_->date());
      tableView_->selectRow(curRow);
    }
  }
}

void PaysDialog::remove()
{
  // QAbstractItemModel *model = tableView_->model();
  // if(model->rowCount()>0) {

  //   int id = model->data(model->index(tableView_->currentIndex().row(), 0)).toInt();
  //   QString atext = model->data(model->index(tableView_->currentIndex().row(),
  // 					     PaysQueryModel::TelA)).toString();

  //   int r = QMessageBox::warning(this, trUtf8("Подтверждение"),
  // 		trUtf8("Действительно удалить абонента %1?").arg(atext),
  // 				 QMessageBox::Yes, QMessageBox::No |
  // 				 QMessageBox::Default | QMessageBox::Escape);
  
  //   if (r == QMessageBox::No)
  //     return;

  //   qDebug() << id;
  // }





  // QAbstractItemModel *model = tableView_->model();
  // qDebug() << tableModel_->removeRows(row, 1);
  // if(!tableModel_->submitAll())
  //   QMessageBox::warning(this, trUtf8("Ошибка"),
  // 			 trUtf8("Операция не выполнена!!"), QMessageBox::Ok);

}
