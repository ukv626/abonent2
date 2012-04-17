#include <QtGui>
#include <QtSql>

#include "SummaryCDialog.h"
#include "SqlManager.h"

SummaryCModel::SummaryCModel(const QString &telA, QObject *parent)
  : QSqlTableModel(parent)
{
  setTable("tb_summaryC");
  setFilter(QString("telA=%1").arg(telA));
  select();
  
  setHeaderData(TelA, Qt::Horizontal, trUtf8("Телефон"));
  setHeaderData(Date, Qt::Horizontal, trUtf8("Дата"));
  setHeaderData(Text, Qt::Horizontal, trUtf8("Примечание"));
}


QVariant SummaryCModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlTableModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == F1 ||
	index.column() == F2 ||
	index.column() == F3 ||
	index.column() == F4 ||
	index.column() == F5 ||
	index.column() == F6)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else if (index.column() == Date)
      return value.toDateTime().toString("dd.MM.yyyy");
    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == F1 ||
	index.column() == F2 ||
	index.column() == F3 ||
	index.column() == F4 ||
	index.column() == F5 ||
	index.column() == F6)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}


// -- SummaryCDialog -----------------------------------------------
SummaryCDialog::SummaryCDialog(const QString &telA, QWidget *parent)
  : QDialog(parent), telA_(telA)
{
  relModel = new SummaryCModel(telA);

  tableView = new QTableView;
  tableView->setModel(relModel);

  // tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setColumnHidden(SummaryCModel::TelA, true);
  // tableView->setColumnHidden(SummaryCModel::Type, true);
  //tableView->resizeRowsToContents();

  newAction = new QAction(trUtf8("Добавить"), this);
  connect(newAction, SIGNAL(triggered()), this, SLOT(insert()));

  removeAction = new QAction(trUtf8("Удалить"), this);
  connect(removeAction, SIGNAL(triggered()), this, SLOT(remove()));

  updateActions();

  // QDate summaryLastDate;
  // SqlManager::summaryLastDate(&summaryLastDate);
  // if(SqlManager::isMonthClosed(summaryLastDate)) {
  //   newAction->setEnabled(false);
  //   removeAction->setEnabled(false);
  // }

  tableView->addAction(newAction);
  tableView->addAction(removeAction);
  
  tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  tableView->setAlternatingRowColors(true);
  
  // tableView->sortByColumn(SummaryCModel::Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();

  tableView->setColumnWidth(SummaryCModel::F1, 70);
  tableView->setColumnWidth(SummaryCModel::F2, 70);
  tableView->setColumnWidth(SummaryCModel::F3, 70);
  tableView->setColumnWidth(SummaryCModel::F4, 70);
  tableView->setColumnWidth(SummaryCModel::F5, 70);
  tableView->setColumnWidth(SummaryCModel::F6, 70);
  tableView->setColumnWidth(SummaryCModel::Text, 120);
  tableView->setColumnWidth(SummaryCModel::Date, 90);

  // tableView->setColumnWidth(SummaryC_CatNum, 160);
  // tableView->setColumnWidth(SummaryC_Text, 250);
  // tableView->setColumnWidth(SummaryC_Place, 80);
  // tableView->setColumnWidth(SummaryC_Brandname, 130);
  // tableView->horizontalHeader()->setStretchLastSection(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Корректировки начислений [ %1 ]").arg(telA));
  setFixedWidth(tableView->horizontalHeader()->length()+50);
  setFixedHeight(380);
}


SummaryCDialog::~SummaryCDialog()
{
  delete relModel;
}

void SummaryCDialog::updateActions()
{
  if(relModel->rowCount()>0) {
    removeAction->setEnabled(true);
  }
  else {
    removeAction->setEnabled(false);
  }
}


void SummaryCDialog::insert()
{
  QAbstractItemModel *model = tableView->model();
  int row = model->rowCount();
  model->insertRow(row);
  model->setData(model->index(row, SummaryCModel::TelA), telA_);

  QSqlQuery query;
  query.prepare("SELECT MAX(date_) FROM tb_summary");
  query.exec();
  if(query.next())
    model->setData(model->index(row, SummaryCModel::Date), query.value(0).toDate());

  if(model->submit())
    updateActions();
  else {
    model->removeRows(row, 1);

    QMessageBox::warning(this, trUtf8("Ошибка"),
                         trUtf8("Невозможно добавить корректировку!"),
                         QMessageBox::Ok);
  }
}


void SummaryCDialog::remove()
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
			 trUtf8("Корректировка не удалена!!"),  QMessageBox::Ok);
}

