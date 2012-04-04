#include <QtGui>
#include <QtSql>

#include "ServicesDialog.h"

ServicesModel::ServicesModel(QObject *parent)
  : QSqlRelationalTableModel(parent)
{
  setTable("tb_services");
  setRelation(Type, QSqlRelation("tb_serviceTypes", "uid", "text"));
  //relModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  select();

  setHeaderData(Text, Qt::Horizontal, trUtf8("Наименование"));
  setHeaderData(CostR, Qt::Horizontal, trUtf8("CostR"));
  setHeaderData(Cost, Qt::Horizontal, trUtf8("Cost"));
  setHeaderData(Type, Qt::Horizontal, trUtf8("Тип"));
  setHeaderData(PType, Qt::Horizontal, trUtf8("Беспл."));
}


QVariant ServicesModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlRelationalTableModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == CostR ||
	index.column() == Cost)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else if(index.column() == PType) {
      if(value.toInt() == 0)
	return trUtf8("Нет");
      else
	return trUtf8("Да");
    }
    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == CostR ||
	 index.column() == Cost ||
	 index.column() == PType)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}



ServicesModelTypeDelegate::ServicesModelTypeDelegate(QObject *parent)
  : QItemDelegate(parent)
{
}

   
QWidget *ServicesModelTypeDelegate::createEditor(QWidget *parent,
					 const QStyleOptionViewItem& /* option */,
					 const QModelIndex& /* index */) const
{
  QComboBox *editor = new QComboBox(parent);
  QSqlQuery query("SELECT uid,text FROM tb_serviceTypes ORDER BY 1");
  QStringList items;
  
  while(query.next()) {
    items << query.value(1).toString();
  }
  editor->addItems(items);
  editor->installEventFilter(const_cast<ServicesModelTypeDelegate*>(this));
  return editor;
}
   
void ServicesModelTypeDelegate::setEditorData(QWidget *editor,
					      const QModelIndex &index) const
{
  QString value = index.model()->data(index, Qt::EditRole).toString();
  QComboBox *de = static_cast<QComboBox*>(editor);
  int i = de->findText(value);
  de->setCurrentIndex(i);
}
   
void ServicesModelTypeDelegate::setModelData(QWidget *editor,
					     QAbstractItemModel *model,
					     const QModelIndex& index) const
{
  QComboBox *de = static_cast<QComboBox*>(editor);
  // de->interpretText();
  int value = de->currentIndex();
  model->setData(index, value);
}
   
void ServicesModelTypeDelegate::updateEditorGeometry(QWidget *editor,
					const QStyleOptionViewItem &option,
					const QModelIndex& /* index */) const
{
  editor->setGeometry(option.rect);
}

// -- ServicesDialog -----------------------------------------------
ServicesDialog::ServicesDialog(QWidget *parent)
  : QDialog(parent)
{
  findLabel = new QLabel(trUtf8("&Поиск"));
  findEdit = new QLineEdit;
  findLabel->setBuddy(findEdit);

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(findLabel);
  topLayout->addWidget(findEdit);
  
  relModel = new ServicesModel;
  proxyModel = new QSortFilterProxyModel;
  // proxyModel->setDynamicSortFilter(false);
  proxyModel->setSourceModel(relModel);
  proxyModel->setFilterKeyColumn(-1);
  proxyModel->sort(ServicesModel::Text, Qt::AscendingOrder);

  connect(findEdit, SIGNAL(textChanged(QString)),
  	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);

  tableView = new QTableView;
  tableView->setModel(proxyModel);

  tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setColumnHidden(ServicesModel::Id, true);
  tableView->setColumnHidden(ServicesModel::Prim, true);
  tableView->setColumnHidden(ServicesModel::Cost, true);
  tableView->setColumnHidden(ServicesModel::CostR, true);
  //tableView->resizeRowsToContents();
  tableView->setItemDelegateForColumn(ServicesModel::Type,
				      new ServicesModelTypeDelegate(this));

  QAction *newRowAction = new QAction(trUtf8("Добавить"), this);
  connect(newRowAction, SIGNAL(triggered()), this, SLOT(newRow()));

  // QAction *showMovesAction = new QAction(trUtf8("Движение.."), this);
  // connect(showMovesAction, SIGNAL(triggered()), this, SLOT(showMovesDialog()));

  tableView->addAction(newRowAction);
  // tableView->addAction(copyRowAction);
  // tableView->addAction(showMovesAction);
  
  tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  tableView->setAlternatingRowColors(true);
  
  // tableView->sortByColumn(ServicesModel::Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
  // tableView->setColumnWidth(Services_CatNum, 160);
  // tableView->setColumnWidth(Services_Text, 250);
  // tableView->setColumnWidth(Services_Place, 80);
  // tableView->setColumnWidth(Services_Brandname, 130);
  // tableView->horizontalHeader()->setStretchLastSection(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(tableView);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Услуги"));
  setFixedWidth(tableView->horizontalHeader()->length()+50);
  setFixedHeight(380);
}


ServicesDialog::~ServicesDialog()
{
  delete proxyModel;
  delete relModel;
}

void ServicesDialog::filterRegExpChanged()
{
  QRegExp regExp(findEdit->text());
  proxyModel->setFilterRegExp(regExp);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
}



void ServicesDialog::newRow()
{
  // QItemSelectionModel *selection = tableView->selectionModel();
  // int row = selection->selectedIndexes().first().row();

  QAbstractItemModel *model = tableView->model();
  // QModelIndex index = model->index(0, ServicesModel::Text);
  model->insertRow(model->rowCount());
  // model->setData(model->index(row +1, ServicesModel::Type), 0);
  // model->submit();

  // tableView->setCurrentIndex(index);
}



/*
void ServicesDialog::showStoragesDialog()
{
  StoragesDialog dialog;
  dialog.exec();
}
*/

// void ServicesDialog::showMovesDialog()
// {
//   QAbstractItemModel *model = tableView->model();

//   int detailId = model->data(model->index(tableView->currentIndex().row(), Services_Id)).toInt();
//   double qty = model->data(model->index(tableView->currentIndex().row(), Services_Qty)).toDouble();
//   double nr = model->data(model->index(tableView->currentIndex().row(), Services_NR)).toDouble();

//   MovesDialog dialog(detailId, qty, nr, this);
//   dialog.exec();
//   if(dialog.isDirty()) {
//     model->setData(model->index(tableView->currentIndex().row(), Services_NR), dialog.getNr());

//     QModelIndex index = model->index(tableView->currentIndex().row(), Services_Text);
//     model->submit();

//     tableView->setCurrentIndex(index);
//   }

// }

