#include <QtGui>
#include <QtSql>

#include "CorrectionsDialog.h"

CorrectionsModel::CorrectionsModel(quint32 clientId, QObject *parent)
  : QSqlTableModel(parent)
{
  setTable("tb_corrections");
  setFilter(QString("clientID=%1").arg(clientId));
  select();
  
  setHeaderData(TelA, Qt::Horizontal, trUtf8("Телефон"));
  setHeaderData(Date, Qt::Horizontal, trUtf8("Дата"));
  setHeaderData(Sum, Qt::Horizontal, trUtf8("Сумма"));
  setHeaderData(Text, Qt::Horizontal, trUtf8("Примечание"));
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


// -- CorrectionsDialog -----------------------------------------------
CorrectionsDialog::CorrectionsDialog(quint32 clientId, const QString &caption,
				     QWidget *parent)
  : QDialog(parent), clientId_(clientId)
{
  relModel = new CorrectionsModel(clientId);

  tableView = new QTableView;
  tableView->setModel(relModel);

  // tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setColumnHidden(CorrectionsModel::Id, true);
    tableView->setColumnHidden(CorrectionsModel::ClientId, true);
  // tableView->setColumnHidden(CorrectionsModel::Type, true);
  //tableView->resizeRowsToContents();

  QAction *newRowAction = new QAction(trUtf8("Добавить"), this);
  connect(newRowAction, SIGNAL(triggered()), this, SLOT(newRow()));

  QAction *removeRowAction = new QAction(trUtf8("Удалить"), this);
  connect(removeRowAction, SIGNAL(triggered()), this, SLOT(removeRow()));

  tableView->addAction(newRowAction);
  tableView->addAction(removeRowAction);
  
  tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  tableView->setAlternatingRowColors(true);
  
  // tableView->sortByColumn(CorrectionsModel::Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();

  tableView->setColumnWidth(CorrectionsModel::Sum, 70);
  tableView->setColumnWidth(CorrectionsModel::Text, 180);
  tableView->setColumnWidth(CorrectionsModel::Date, 90);

  // tableView->setColumnWidth(Corrections_CatNum, 160);
  // tableView->setColumnWidth(Corrections_Text, 250);
  // tableView->setColumnWidth(Corrections_Place, 80);
  // tableView->setColumnWidth(Corrections_Brandname, 130);
  // tableView->horizontalHeader()->setStretchLastSection(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Корректировки [ %1 ]").arg(caption));
  setFixedWidth(tableView->horizontalHeader()->length()+50);
  setFixedHeight(380);
}


CorrectionsDialog::~CorrectionsDialog()
{
  delete relModel;
}


void CorrectionsDialog::newRow()
{
  QAbstractItemModel *model = tableView->model();
  int row = model->rowCount();
  model->insertRow(row);
  model->setData(model->index(row, CorrectionsModel::ClientId), clientId_);

  QSqlQuery query;
  query.prepare("SELECT MAX(date_) FROM tb_summary");
  query.exec();
  if(query.next())
    model->setData(model->index(row, CorrectionsModel::Date), query.value(0).toDate());

  if(!model->submit()) {
    model->removeRows(row, 1);

    QMessageBox::warning(this, trUtf8("Ошибка"),
                         trUtf8("Невозможно добавить корректировку!"),
                         QMessageBox::Ok);
  }

}


void CorrectionsDialog::removeRow()
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
  if(!model->submit())
    QMessageBox::warning(this, trUtf8("Ошибка"), trUtf8("Корректировка не удалена!!"),
                         QMessageBox::Ok);
}

