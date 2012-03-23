#include <QtGui>
#include <QtSql>
#include <QDate>

#include "ServicesPanel.h"

ServicesByTelModel::ServicesByTelModel(QObject *parent)
  : QSqlQueryModel(parent)
{
  // refresh("9219001800", "2012-02-29");
}

void ServicesByTelModel::refresh(const QString &telA, const QString &date)
{
  QSqlQuery query;

  query.prepare(" SELECT s.text,sh.costR,sh.cost"
                " FROM tb_servicesHistoryRD sh"
		" ,tb_services s"
                " WHERE sh.serviceID=s.uid"
                " AND sh.telA=:telA"
                " AND DATE_FORMAT(sh.date_,'%Y-%m-%d')=:date");
  query.bindValue(":telA", telA);
  query.bindValue(":date", date);
  query.exec();

  setQuery(query);

  setHeaderData(Text, Qt::Horizontal, trUtf8("Наименование"));
  setHeaderData(CostR, Qt::Horizontal, trUtf8("CostR"));
  setHeaderData(Cost, Qt::Horizontal, trUtf8("Cost"));

}

// bool ServicesByTelModel::select()
// {
//   bool result = QSqlRelationalTableModel::select();
//   if(result) {
//     while (canFetchMore()) fetchMore();
//     return true;
//   }
//   else return false;
// }

QVariant ServicesByTelModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlQueryModel::data(index, role);
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

// -- ServicesPanel -----------------------------------------------
ServicesPanel::ServicesPanel(QWidget *parent)
  : QWidget(parent)
{
  relModel_ = new ServicesByTelModel;
  
  // proxyModel = new QSortFilterProxyModel;
  // // proxyModel->setDynamicSortFilter(false);
  // proxyModel->setSourceModel(relModel);
  // proxyModel->setFilterKeyColumn(-1);
  // //proxyModel->sort(Services_Text, Qt::AscendingOrder);

  // connect(findEdit, SIGNAL(textChanged(QString)),
  // 	  this, SLOT(filterRegExpChanged()), Qt::UniqueConnection);

  tableView_ = new QTableView;
  tableView_->setModel(relModel_);

  tableView_->setItemDelegate(new QSqlRelationalDelegate(tableView_));
  tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);

  tableView_->resizeRowsToContents();

  // QAction *newRowAction = new QAction(trUtf8("Новая"), this);
  // connect(newRowAction, SIGNAL(triggered()), this, SLOT(newRow()));

  // QAction *copyRowAction = new QAction(trUtf8("Копия"), this);
  // connect(copyRowAction, SIGNAL(triggered()), this, SLOT(copyRow()));

  // QAction *showMovesAction = new QAction(trUtf8("Движение.."), this);
  // connect(showMovesAction, SIGNAL(triggered()), this, SLOT(showMovesDialog()));

  // tableView->addAction(newRowAction);
  // tableView->addAction(copyRowAction);
  // tableView->addAction(showMovesAction);
  
  // tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  // tableView_->setAlternatingRowColors(true);
  
  // tableView->sortByColumn(Services_Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  
  // tableView_->setColumnWidth(Services_Brandname, 130);
  // tableView_->horizontalHeader()->setStretchLastSection(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(1, 1, 1, 1);
  mainLayout->addWidget(tableView_);
  setLayout(mainLayout);
}


ServicesPanel::~ServicesPanel()
{
  delete relModel_;
}

void ServicesPanel::refresh(const QString &telA, const QString &date)
{
  relModel_->refresh(telA, date);
  tableView_->setColumnWidth(ServicesByTelModel::Text, 180);
  tableView_->setColumnWidth(ServicesByTelModel::CostR, 60);
  tableView_->setColumnWidth(ServicesByTelModel::Cost, 60);
}
