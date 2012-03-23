#include <QtGui>
#include <QtSql>

#include "AbonentTypesDialog.h"


AbonentTypesModel::AbonentTypesModel(QObject *parent)
  : QSqlTableModel(parent)
{
  setTable("tb_abonentTypes");
  select();
  
  setHeaderData(Text, Qt::Horizontal, trUtf8("Наименование"));
  // setHeaderData(Address, Qt::Horizontal, trUtf8("Адрес"));
  // setHeaderData(Tel, Qt::Horizontal, trUtf8("Телефон"));
  // setHeaderData(KPeni, Qt::Horizontal, trUtf8("k пени "));
  // setHeaderData(Type, Qt::Horizontal, trUtf8("Тип"));
}


QVariant AbonentTypesModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlTableModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == K1 ||
	index.column() == K2 ||
	index.column() == K3 ||
	index.column() == K4 ||
	index.column() == K5 ||
	index.column() == K6)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else
      return value;

  case Qt::TextAlignmentRole: // Выравнивание
    if(index.column() == K1 ||
       index.column() == K2 ||
       index.column() == K3 ||
       index.column() == K4 ||
       index.column() == K5 ||
       index.column() == K6)
      return double(Qt::AlignRight | Qt::AlignVCenter);
    else
      return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}




// -- AbonentTypesDialog -----------------------------------------------
AbonentTypesDialog::AbonentTypesDialog(QWidget *parent)
  : QDialog(parent)
{
  findLabel = new QLabel(trUtf8("&Поиск"));
  findEdit = new QLineEdit;
  findLabel->setBuddy(findEdit);

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(findLabel);
  topLayout->addWidget(findEdit);
  
  relModel = new AbonentTypesModel;
  proxyModel = new QSortFilterProxyModel;
  // proxyModel->setDynamicSortFilter(false);
  proxyModel->setSourceModel(relModel);
  proxyModel->setFilterKeyColumn(-1);
  proxyModel->sort(AbonentTypesModel::Text, Qt::AscendingOrder);

  connect(findEdit, SIGNAL(textChanged(QString)),
  	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);

  tableView = new QTableView;
  tableView->setModel(proxyModel);

  // tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setColumnHidden(AbonentTypesModel::Id, true);
  //tableView->resizeRowsToContents();

  QAction *newRowAction = new QAction(trUtf8("Добавить"), this);
  connect(newRowAction, SIGNAL(triggered()), this, SLOT(newRow()));

  // QAction *showMovesAction = new QAction(trUtf8("Движение.."), this);
  // connect(showMovesAction, SIGNAL(triggered()), this, SLOT(showMovesDialog()));

  tableView->addAction(newRowAction);
  // tableView->addAction(copyRowAction);
  // tableView->addAction(showMovesAction);
  
  tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  tableView->setAlternatingRowColors(true);
  
  // tableView->sortByColumn(AbonentTypesModel::Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
  // tableView->setColumnWidth(AbonentTypes_CatNum, 160);
  // tableView->setColumnWidth(AbonentTypes_Text, 250);
  // tableView->setColumnWidth(AbonentTypes_Place, 80);
  // tableView->setColumnWidth(AbonentTypes_Brandname, 130);
  // tableView->horizontalHeader()->setStretchLastSection(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(tableView);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Типы абонентов"));
  setFixedWidth(tableView->horizontalHeader()->length()+50);
  setFixedHeight(380);
}


AbonentTypesDialog::~AbonentTypesDialog()
{
  delete proxyModel;
  delete relModel;
}

void AbonentTypesDialog::filterRegExpChanged()
{
  QRegExp regExp(findEdit->text());
  proxyModel->setFilterRegExp(regExp);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
}



void AbonentTypesDialog::newRow()
{
  // QItemSelectionModel *selection = tableView->selectionModel();
  // int row = selection->selectedIndexes().first().row();

  QAbstractItemModel *model = tableView->model();
  // QModelIndex index = model->index(0, AbonentTypesModel::Text);
  model->insertRow(model->rowCount());
  // model->setData(model->index(row +1, AbonentTypesModel::Type), 0);
  // model->submit();

  // tableView->setCurrentIndex(index);
}



