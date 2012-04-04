#include <QtGui>
#include <QtSql>
#include <QDate>

#include "AccrualsPanel.h"

AccrualsModel::AccrualsModel(QObject *parent)
  : QSqlQueryModel(parent)
{
}

void AccrualsModel::refresh(const QString &telA)
{
  QSqlQuery query;
  query.prepare(" SELECT sf.date_"
		" ,s.f1+IFNULL(sc.f1,0) AS F1"
		" ,s.f2+IFNULL(sc.f2,0) AS F2"
		" ,s.f3+IFNULL(sc.f3,0) AS F3"
		" ,s.f4+IFNULL(sc.f4,0) AS F4"
		" ,s.f5+IFNULL(sc.f5,0) AS F5"
		" ,s.f6+IFNULL(sc.f6,0) AS F6"
		" ,tp.text"
		" ,sf.f1+IFNULL(sfc.f1,0) AS F7" 
		" ,sf.f2+IFNULL(sfc.f2,0) AS F8" 
		" ,sf.f3+IFNULL(sfc.f3,0) AS F9" 
		" ,sf.f4+IFNULL(sfc.f4,0) AS F10"
		" ,s.f1+IFNULL(sc.f1,0)+"
		"  s.f2+IFNULL(sc.f2,0)+"
		"   s.f3+IFNULL(sc.f3,0)+"
		"   s.f4+IFNULL(sc.f4,0)+"
		"   s.f5+IFNULL(sc.f5,0)+"
		"   s.f6+IFNULL(sc.f6,0)+"
		"   sf.f1+IFNULL(sfc.f1,0)+"
		"   sf.f2+IFNULL(sfc.f2,0)+"
		"   sf.f3+IFNULL(sfc.f3,0)+"
		"   sf.f4+IFNULL(sfc.f4,0) AS F11"
		" FROM tb_abonents a" 
		" ,tb_summaryFixP sf" 
		" ,tb_tplans tp" 
		" ,tb_summaryP s" 
		" LEFT JOIN tb_summaryC sc ON (sc.date_=s.date_ AND sc.telA=s.telA)" 
		" LEFT JOIN tb_summaryFixC sfc ON (sfc.date_=s.date_ AND sfc.telA=s.telA)" 
		" WHERE a.telA=sf.telA" 
		" AND a.telA=s.telA" 
		" AND sf.date_=s.date_" 
		" AND tp.uid=a.tplanID" 
		" AND a.telA=:telA"
		" ORDER BY 1 DESC");

  query.bindValue(":telA", telA);
  query.exec();

  setQuery(query);
  setHeaderData(Date, Qt::Horizontal, trUtf8("Дата"));
  setHeaderData(F1, Qt::Horizontal, trUtf8("Исх.\nместные"));
  setHeaderData(F2, Qt::Horizontal, trUtf8("Исх.\nМЗ,МГ"));
  setHeaderData(F3, Qt::Horizontal, trUtf8("SMS,\nMMS"));
  setHeaderData(F4, Qt::Horizontal, trUtf8("Пакетн.\nПД"));
  setHeaderData(F5, Qt::Horizontal, trUtf8("За\nпредел"));
  setHeaderData(F6, Qt::Horizontal, trUtf8("Роуминг"));
  setHeaderData(F7, Qt::Horizontal, trUtf8("Осн.\nпакет"));
  setHeaderData(F8, Qt::Horizontal, trUtf8("Доп.\nуслуги"));
  setHeaderData(F9, Qt::Horizontal, trUtf8("Тар.\nопции"));
  setHeaderData(F10, Qt::Horizontal, trUtf8("Разовые"));
  setHeaderData(F11, Qt::Horizontal, trUtf8("ВСЕГО"));
  setHeaderData(TPlan, Qt::Horizontal, trUtf8("ТП"));
}


QVariant AccrualsModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlQueryModel::data(index, role);
  switch (role) {
  case Qt::DisplayRole:
     if (index.column() == Date)
      return value.toDate().toString("dd.MM.yyyy");
     else if (index.column() == F1 ||
	index.column() == F2 ||
	index.column() == F3 ||
	index.column() == F4 ||
	index.column() == F5 ||
	index.column() == F6 ||
	index.column() == F7 ||
	index.column() == F8 ||
	index.column() == F9 ||
	index.column() == F10 ||
	index.column() == F11)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else
      return value;

    case Qt::TextAlignmentRole: // Выравнивание
      if(index.column() == F1 ||
	index.column() == F2 ||
	index.column() == F3 ||
	index.column() == F4 ||
	index.column() == F5 ||
	index.column() == F6 ||
	index.column() == F7 ||
	index.column() == F8 ||
	index.column() == F9 ||
	index.column() == F10 ||
	index.column() == F11)
	return double(Qt::AlignRight | Qt::AlignVCenter);
      else
	return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}

// -- AccrualsPanel -----------------------------------------------
AccrualsPanel::AccrualsPanel(QWidget *parent)
  : QWidget(parent)
{
  relModel_ = new AccrualsModel;
  
  // proxyModel = new QSortFilterProxyModel;
  // // proxyModel->setDynamicSortFilter(false);
  // proxyModel->setSourceModel(relModel);
  // proxyModel->setFilterKeyColumn(-1);
  // //proxyModel->sort(Accruals_Text, Qt::AscendingOrder);

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
  
  // tableView->sortByColumn(Accruals_Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  
  // tableView_->setColumnWidth(Accruals_Brandname, 130);
  // tableView_->horizontalHeader()->setStretchLastSection(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(1, 1, 1, 1);
  mainLayout->addWidget(tableView_);
  setLayout(mainLayout);
}


AccrualsPanel::~AccrualsPanel()
{
  delete relModel_;
}

void AccrualsPanel::refresh(const QString &telA)
{
  relModel_->refresh(telA);
  tableView_->setColumnWidth(AccrualsModel::Date, 80);
  tableView_->setColumnWidth(AccrualsModel::F1, 70);
  tableView_->setColumnWidth(AccrualsModel::F2, 70);
  tableView_->setColumnWidth(AccrualsModel::F3, 70);
  tableView_->setColumnWidth(AccrualsModel::F4, 70);
  tableView_->setColumnWidth(AccrualsModel::F5, 70);
  tableView_->setColumnWidth(AccrualsModel::F6, 70);
  tableView_->setColumnWidth(AccrualsModel::TPlan, 180);
  tableView_->setColumnWidth(AccrualsModel::F7, 70);
  tableView_->setColumnWidth(AccrualsModel::F8, 70);
  tableView_->setColumnWidth(AccrualsModel::F9, 70);
  tableView_->setColumnWidth(AccrualsModel::F10, 70);
  tableView_->setColumnWidth(AccrualsModel::F11, 80);
}
