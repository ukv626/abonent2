#include <QtGui>
#include <QtSql>

#include "SummaryCDialog.h"

SummaryCModel::SummaryCModel(const QString &telA, QObject *parent)
  : QSqlTableModel(parent)
{
  setTable("tb_summaryC");
  setFilter(QString("telA=%1").arg(telA));
  select();
  
  setHeaderData(TelA, Qt::Horizontal, trUtf8("Телефон"));
  // setHeaderData(KPeni, Qt::Horizontal, trUtf8("k пени "));
  // setHeaderData(Type, Qt::Horizontal, trUtf8("Тип"));
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
  : QDialog(parent)
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

  QAction *newRowAction = new QAction(trUtf8("Добавить"), this);
  connect(newRowAction, SIGNAL(triggered()), this, SLOT(newRow()));

  // QAction *removeAction = new QAction(trUtf8("Удалить"), this);
  // connect(removeAction, SIGNAL(triggered()), this, SLOT(remove()));

  tableView->addAction(newRowAction);
  // tableView->addAction(removeAction);
  
  tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  tableView->setAlternatingRowColors(true);
  
  // tableView->sortByColumn(SummaryCModel::Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
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


void SummaryCDialog::newRow()
{
  // QItemSelectionModel *selection = tableView->selectionModel();
  // int row = selection->selectedIndexes().first().row();

  QAbstractItemModel *model = tableView->model();
  // QModelIndex index = model->index(0, SummaryCModel::Text);
  model->insertRow(model->rowCount());
  // model->setData(model->index(row +1, SummaryCModel::Type), 0);
  // model->submit();

  // tableView->setCurrentIndex(index);
}



