#include <QtGui>
#include <QtSql>

#include "ClientsDialog.h"
#include "FinallyDialog.h"

ClientsModel::ClientsModel(QObject *parent)
  : QSqlRelationalTableModel(parent)
{
  setTable("tb_clients");
  // setRelation(Type, QSqlRelation("tb_clientTypes", "uid", "text"));
  //relModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  select();
  
  setHeaderData(Text, Qt::Horizontal, trUtf8("Наименование"));
  setHeaderData(Address, Qt::Horizontal, trUtf8("Адрес"));
  setHeaderData(Tel, Qt::Horizontal, trUtf8("Телефон"));
  setHeaderData(KPeni, Qt::Horizontal, trUtf8("k пени "));
  // setHeaderData(Type, Qt::Horizontal, trUtf8("Тип"));
}


QVariant ClientsModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlRelationalTableModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == KPeni)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == KPeni)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}




// -- ClientsDialog -----------------------------------------------
ClientsDialog::ClientsDialog(QWidget *parent)
  : QDialog(parent)
{
  findLabel = new QLabel(trUtf8("&Поиск"));
  findEdit = new QLineEdit;
  findLabel->setBuddy(findEdit);

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(findLabel);
  topLayout->addWidget(findEdit);
  
  relModel = new ClientsModel;
  proxyModel = new QSortFilterProxyModel;
  // proxyModel->setDynamicSortFilter(false);
  proxyModel->setSourceModel(relModel);
  proxyModel->setFilterKeyColumn(-1);
  proxyModel->sort(ClientsModel::Text, Qt::AscendingOrder);

  connect(findEdit, SIGNAL(textChanged(QString)),
  	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);

  tableView = new QTableView;
  tableView->setModel(proxyModel);

  // tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setColumnHidden(ClientsModel::Id, true);
  tableView->setColumnHidden(ClientsModel::Type, true);
  //tableView->resizeRowsToContents();

  QAction *newRowAction = new QAction(trUtf8("Добавить"), this);
  connect(newRowAction, SIGNAL(triggered()), this, SLOT(newRow()));

  QAction *finallyAction = new QAction(trUtf8("История начислений"), this);
  connect(finallyAction, SIGNAL(triggered()), this, SLOT(showFinally()));

  tableView->addAction(newRowAction);
  tableView->addAction(finallyAction);
  // tableView->addAction(showMovesAction);
  
  tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  tableView->setAlternatingRowColors(true);
  
  // tableView->sortByColumn(ClientsModel::Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
  // tableView->setColumnWidth(Clients_CatNum, 160);
  // tableView->setColumnWidth(Clients_Text, 250);
  // tableView->setColumnWidth(Clients_Place, 80);
  // tableView->setColumnWidth(Clients_Brandname, 130);
  // tableView->horizontalHeader()->setStretchLastSection(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(tableView);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Клиенты"));
  setFixedWidth(tableView->horizontalHeader()->length()+50);
  setFixedHeight(380);
}


ClientsDialog::~ClientsDialog()
{
  delete proxyModel;
  delete relModel;
}

void ClientsDialog::filterRegExpChanged()
{
  QRegExp regExp(findEdit->text());
  proxyModel->setFilterRegExp(regExp);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
}



void ClientsDialog::newRow()
{
  // QItemSelectionModel *selection = tableView->selectionModel();
  // int row = selection->selectedIndexes().first().row();

  QAbstractItemModel *model = tableView->model();
  // QModelIndex index = model->index(0, ClientsModel::Text);
  model->insertRow(model->rowCount());
  // model->setData(model->index(row +1, ClientsModel::Type), 0);
  // model->submit();

  // tableView->setCurrentIndex(index);
}



/*
void ClientsDialog::showStoragesDialog()
{
  StoragesDialog dialog;
  dialog.exec();
}
*/

void ClientsDialog::showFinally()
{
  QAbstractItemModel *model = tableView->model();
  qint32 id = model->data(model->index(tableView->currentIndex().row(), 0)).toInt();
  QString text = model->data(model->index(tableView->currentIndex().row(),
				       ClientsModel::Text)).toString();

  FinallyDialog dialog(id, text, this);
  dialog.exec();
}

