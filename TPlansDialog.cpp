#include <QtGui>
#include <QtSql>

#include "TPlansDialog.h"


TPlansModel::TPlansModel(QObject *parent)
  : QSqlTableModel(parent)
{
  setTable("tb_tplans");
  select();
  
  setHeaderData(Text, Qt::Horizontal, trUtf8("Наименование"));
  // setHeaderData(Address, Qt::Horizontal, trUtf8("Адрес"));
  // setHeaderData(Tel, Qt::Horizontal, trUtf8("Телефон"));
  // setHeaderData(KPeni, Qt::Horizontal, trUtf8("k пени "));
  // setHeaderData(Type, Qt::Horizontal, trUtf8("Тип"));
}


QVariant TPlansModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlTableModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == CostR ||
	index.column() == Cost)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else
      return value;

  case Qt::TextAlignmentRole: // Выравнивание
    if(index.column() == CostR ||
       index.column() == Cost)
      return double(Qt::AlignRight | Qt::AlignVCenter);
    else
      return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}




// -- TPlansDialog -----------------------------------------------
TPlansDialog::TPlansDialog(QWidget *parent)
  : QDialog(parent)
{
  findLabel = new QLabel(trUtf8("&Поиск"));
  findEdit = new QLineEdit;
  findLabel->setBuddy(findEdit);

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(findLabel);
  topLayout->addWidget(findEdit);
  
  relModel = new TPlansModel;
  proxyModel = new QSortFilterProxyModel;
  // proxyModel->setDynamicSortFilter(false);
  proxyModel->setSourceModel(relModel);
  proxyModel->setFilterKeyColumn(-1);
  proxyModel->sort(TPlansModel::Text, Qt::AscendingOrder);

  connect(findEdit, SIGNAL(textChanged(QString)),
  	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);

  tableView = new QTableView;
  tableView->setModel(proxyModel);

  // tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setColumnHidden(TPlansModel::Id, true);
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
  
  // tableView->sortByColumn(TPlansModel::Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
  // tableView->setColumnWidth(TPlans_CatNum, 160);
  // tableView->setColumnWidth(TPlans_Text, 250);
  // tableView->setColumnWidth(TPlans_Place, 80);
  // tableView->setColumnWidth(TPlans_Brandname, 130);
  // tableView->horizontalHeader()->setStretchLastSection(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(tableView);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Тарифные планы"));
  setFixedWidth(tableView->horizontalHeader()->length()+50);
  setFixedHeight(380);
}


TPlansDialog::~TPlansDialog()
{
  delete proxyModel;
  delete relModel;
}

void TPlansDialog::filterRegExpChanged()
{
  QRegExp regExp(findEdit->text());
  proxyModel->setFilterRegExp(regExp);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
}



void TPlansDialog::newRow()
{
  // QItemSelectionModel *selection = tableView->selectionModel();
  // int row = selection->selectedIndexes().first().row();

  QAbstractItemModel *model = tableView->model();
  // QModelIndex index = model->index(0, TPlansModel::Text);
  model->insertRow(model->rowCount());
  // model->setData(model->index(row +1, TPlansModel::Type), 0);
  // model->submit();

  // tableView->setCurrentIndex(index);
}



