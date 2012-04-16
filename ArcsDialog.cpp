#include <QtGui>
#include <QtSql>

#include "ArcsDialog.h"
#include "SqlManager.h"


ArcsModel::ArcsModel(QObject *parent)
  : QSqlTableModel(parent)
{
  setTable("tb_arcs");
  select();
  
  setHeaderData(Date, Qt::Horizontal, trUtf8("Дата"));
  setHeaderData(IsClosed, Qt::Horizontal, trUtf8("Закрыт"));
}

QVariant ArcsModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlTableModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
    if (index.column() == Date)
      return value.toDateTime().toString("yyyy-MM-dd");
    else if(index.column() == IsClosed) {
      if(value.toInt() == 0)
	return trUtf8("Нет");
      else
	return trUtf8("Да");
    }
    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == IsClosed)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);


  }
  return value;
}




// -- ArcsDialog -----------------------------------------------
ArcsDialog::ArcsDialog(QWidget *parent)
  : QDialog(parent)
{
  findLabel = new QLabel(trUtf8("&Поиск"));
  findEdit = new QLineEdit;
  findLabel->setBuddy(findEdit);

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(findLabel);
  topLayout->addWidget(findEdit);
  
  relModel = new ArcsModel;
  proxyModel = new QSortFilterProxyModel;
  proxyModel->setSourceModel(relModel);
  proxyModel->setFilterKeyColumn(-1);
  proxyModel->sort(ArcsModel::Date, Qt::AscendingOrder);

  connect(findEdit, SIGNAL(textChanged(QString)),
  	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);

  tableView = new QTableView;
  tableView->setModel(proxyModel);

  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  // tableView->setColumnHidden(ArcsModel::Id, true);

  QAction *newRowAction = new QAction(trUtf8("Добавить"), this);
  connect(newRowAction, SIGNAL(triggered()), this, SLOT(newRow()));


  tableView->addAction(newRowAction);
  
  tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  tableView->setAlternatingRowColors(true);
  
  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
  tableView->setColumnWidth(ArcsModel::Date, 90);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(tableView);
  setLayout(mainLayout);

  setWindowTitle(trUtf8("Архив"));
  setFixedWidth(tableView->horizontalHeader()->length()+50);
  setFixedHeight(380);
}


ArcsDialog::~ArcsDialog()
{
  delete proxyModel;
  delete relModel;
}

void ArcsDialog::filterRegExpChanged()
{
  QRegExp regExp(findEdit->text());
  proxyModel->setFilterRegExp(regExp);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
}


void ArcsDialog::newRow()
{
  // QItemSelectionModel *selection = tableView->selectionModel();
  // int row = selection->selectedIndexes().first().row();

  QAbstractItemModel *model = tableView->model();
  // QModelIndex index = model->index(0, ArcsModel::Text);
  int row = model->rowCount();
  model->insertRow(row);
  QDate date;
  SqlManager::finallyLastDate(&date);
  model->setData(model->index(row, ArcsModel::Date), date);
  if(!model->submit())
    model->removeRows(row,1);

  // tableView->setCurrentIndex(index);
}



