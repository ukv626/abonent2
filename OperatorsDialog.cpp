#include <QtGui>
#include <QtSql>

#include "OperatorsDialog.h"


OperatorsModel::OperatorsModel(QObject *parent)
  : QSqlTableModel(parent)
{
  setTable("tb_operators");
  select();
  
  setHeaderData(Text, Qt::Horizontal, trUtf8("Наименование"));
}



// -- OperatorsDialog -----------------------------------------------
OperatorsDialog::OperatorsDialog(QWidget *parent)
  : QDialog(parent)
{
  findLabel = new QLabel(trUtf8("&Поиск"));
  findEdit = new QLineEdit;
  findLabel->setBuddy(findEdit);

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(findLabel);
  topLayout->addWidget(findEdit);
  
  relModel = new OperatorsModel;
  proxyModel = new QSortFilterProxyModel;
  proxyModel->setSourceModel(relModel);
  proxyModel->setFilterKeyColumn(-1);
  proxyModel->sort(OperatorsModel::Text, Qt::AscendingOrder);

  connect(findEdit, SIGNAL(textChanged(QString)),
  	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);

  tableView = new QTableView;
  tableView->setModel(proxyModel);

  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setColumnHidden(OperatorsModel::Id, true);

  QAction *newRowAction = new QAction(trUtf8("Добавить"), this);
  connect(newRowAction, SIGNAL(triggered()), this, SLOT(newRow()));


  tableView->addAction(newRowAction);
  
  tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  tableView->setAlternatingRowColors(true);
  
  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
  tableView->setColumnWidth(OperatorsModel::Text, 250);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(tableView);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Операторы"));
  setFixedWidth(tableView->horizontalHeader()->length()+50);
  setFixedHeight(380);
}


OperatorsDialog::~OperatorsDialog()
{
  delete proxyModel;
  delete relModel;
}

void OperatorsDialog::filterRegExpChanged()
{
  QRegExp regExp(findEdit->text());
  proxyModel->setFilterRegExp(regExp);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
}


void OperatorsDialog::newRow()
{
  // QItemSelectionModel *selection = tableView->selectionModel();
  // int row = selection->selectedIndexes().first().row();

  QAbstractItemModel *model = tableView->model();
  // QModelIndex index = model->index(0, OperatorsModel::Text);
  model->insertRow(model->rowCount());
  // model->setData(model->index(row +1, OperatorsModel::Type), 0);
  // model->submit();

  // tableView->setCurrentIndex(index);
}



