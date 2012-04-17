#include <QtGui>
#include <QtSql>

#include "SummaryFixCDialog.h"
#include "SqlManager.h"

SummaryFixCModel::SummaryFixCModel(const QString &telA, QObject *parent)
  : QSqlTableModel(parent)
{
  setTable("tb_summaryFixC");
  setFilter(QString("telA=%1").arg(telA));
  select();
  
  setHeaderData(TelA, Qt::Horizontal, trUtf8("Телефон"));
  setHeaderData(Date, Qt::Horizontal, trUtf8("Дата"));
  setHeaderData(Text, Qt::Horizontal, trUtf8("Примечание"));
}


QVariant SummaryFixCModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlTableModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == F1 ||
	index.column() == F2 ||
	index.column() == F3 ||
	index.column() == F4)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else if (index.column() == Date)
      return value.toDateTime().toString("dd.MM.yyyy");
    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == F1 ||
	 index.column() == F2 ||
	 index.column() == F3 ||
	 index.column() == F4)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}


// -- SummaryFixCDialog -----------------------------------------------
SummaryFixCDialog::SummaryFixCDialog(const QString &telA, QWidget *parent)
  : QDialog(parent), telA_(telA)
{
  relModel = new SummaryFixCModel(telA);

  tableView = new QTableView;
  tableView->setModel(relModel);

  // tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setColumnHidden(SummaryFixCModel::TelA, true);
  // tableView->setColumnHidden(SummaryFixCModel::Type, true);
  //tableView->resizeRowsToContents();

  newAction = new QAction(trUtf8("Добавить"), this);
  connect(newAction, SIGNAL(triggered()), this, SLOT(insert()));

  removeAction = new QAction(trUtf8("Удалить"), this);
  connect(removeAction, SIGNAL(triggered()), this, SLOT(remove()));

  updateActions();

  // QDate summaryLastDate;
  // SqlManager::summaryLastDate(&summaryLastDate);
  // if(SqlManager::isMonthClosed(summaryLastDate)) {
  //   newRowAction->setEnabled(false);
  //   removeRowAction->setEnabled(false);
  // }


  tableView->addAction(newAction);
  tableView->addAction(removeAction);
  
  tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  tableView->setAlternatingRowColors(true);
  
  // tableView->sortByColumn(SummaryFixCModel::Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
  tableView->setColumnWidth(SummaryFixCModel::F1, 70);
  tableView->setColumnWidth(SummaryFixCModel::F2, 70);
  tableView->setColumnWidth(SummaryFixCModel::F3, 70);
  tableView->setColumnWidth(SummaryFixCModel::F4, 70);
  tableView->setColumnWidth(SummaryFixCModel::Text, 120);
  tableView->setColumnWidth(SummaryFixCModel::Date, 90);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Корректировки фикс. начислений [ %1 ]").arg(telA));
  setFixedWidth(tableView->horizontalHeader()->length()+50);
  setFixedHeight(380);
}


SummaryFixCDialog::~SummaryFixCDialog()
{
  delete relModel;
}

void SummaryFixCDialog::updateActions()
{
  if(relModel->rowCount()>0) {
    removeAction->setEnabled(true);
  }
  else {
    removeAction->setEnabled(false);
  }
}


void SummaryFixCDialog::insert()
{
  QAbstractItemModel *model = tableView->model();
  int row = model->rowCount();
  model->insertRow(row);
  model->setData(model->index(row, SummaryFixCModel::TelA), telA_);

  QSqlQuery query;
  query.prepare("SELECT MAX(date_) FROM tb_summaryFix");
  query.exec();
  if(query.next())
    model->setData(model->index(row, SummaryFixCModel::Date), query.value(0).toDate());

  if(model->submit())
    updateActions();
  else {
    model->removeRows(row, 1);

    QMessageBox::warning(this, trUtf8("Ошибка"),
                         trUtf8("Невозможно добавить корректировку!"),
                         QMessageBox::Ok);
  }
}

void SummaryFixCDialog::remove()
{
  QItemSelectionModel *selection = tableView->selectionModel();
  if(selection->selectedIndexes().size() == 0)
    return;

  int r = QMessageBox::warning(this, trUtf8("Подтверждение"),
        trUtf8("Действительно удалить корректировку ?"),
        QMessageBox::Yes,
        QMessageBox::No | QMessageBox::Default | QMessageBox::Escape);

  if (r == QMessageBox::No)
    return;

  int row = selection->selectedIndexes().first().row();
  QAbstractItemModel *model = tableView->model();
  model->removeRows(row, 1);
  if(model->submit())
    updateActions();
  else
    QMessageBox::warning(this, trUtf8("Ошибка"),
			 trUtf8("Корректировка не удалена!!"), QMessageBox::Ok);
}


