#include <QtGui>
#include <QtSql>
#include <QDate>

#include "AccrualsRPanel.h"

AccrualsRModel::AccrualsRModel(QObject *parent)
  : QSqlQueryModel(parent)
{
}

void AccrualsRModel::refresh(const QString &telA)
{
  QSqlQuery query;
  query.prepare(" SELECT s.date_"
		" ,s.f1"
		" ,s.f2"
		" ,s.f3"
		" ,s.f4"
		" ,s.f5"
		" ,s.f6"
		" ,sf.tplan"
		" ,sf.f1 AS f7"
		" ,sf.f2 AS f8"
		" ,sf.f3 AS f9"
		" ,sf.f4 AS f10"
		" ,s.f1+s.f2+s.f3+s.f4+s.f5+s.f6 + sf.f1+sf.f2+sf.f3+sf.f4 AS f11"
		" FROM tb_summaryFix sf"
		" ,tb_summary s"
		" WHERE 1=1"
		" AND s.date_=sf.date_"
		" AND s.telA=sf.telA"
		" AND s.telA=:telA"
		" ORDER BY date_ DESC");

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


QVariant AccrualsRModel::data(const QModelIndex &index, int role) const
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

// -- AccrualsRPanel -----------------------------------------------
AccrualsRPanel::AccrualsRPanel(QWidget *parent)
  : QWidget(parent)
{
  relModel_ = new AccrualsRModel;
  
  // proxyModel = new QSortFilterProxyModel;
  // // proxyModel->setDynamicSortFilter(false);
  // proxyModel->setSourceModel(relModel);
  // proxyModel->setFilterKeyColumn(-1);
  // //proxyModel->sort(AccrualsR_Text, Qt::AscendingOrder);

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
  
  // tableView->sortByColumn(AccrualsR_Text, Qt::AscendingOrder);
  // tableView->setSortingEnabled(true);
  
  //tableView->setEditTriggers(QAbstractItemTableView::NoEditTriggers);

  tableView_->verticalHeader()->hide();
  tableView_->horizontalHeader()->hide();
  tableView_->resizeColumnsToContents();
  
  // tableView_->setColumnWidth(AccrualsR_Brandname, 130);
  // tableView_->horizontalHeader()->setStretchLastSection(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(1, 1, 1, 1);
  mainLayout->addWidget(tableView_);
  setLayout(mainLayout);

  // qDebug() << tableView_->rowHeight(0);// * relModel_->rowCount();
  // setFixedHeight(tableView_->rowHeight(1) * relModel_->rowCount());
}


AccrualsRPanel::~AccrualsRPanel()
{
  delete relModel_;
}

void AccrualsRPanel::refresh(const QString &telA)
{
  relModel_->refresh(telA);
  tableView_->setColumnWidth(AccrualsRModel::Date, 80);
  tableView_->setColumnWidth(AccrualsRModel::F1, 70);
  tableView_->setColumnWidth(AccrualsRModel::F2, 70);
  tableView_->setColumnWidth(AccrualsRModel::F3, 70);
  tableView_->setColumnWidth(AccrualsRModel::F4, 70);
  tableView_->setColumnWidth(AccrualsRModel::F5, 70);
  tableView_->setColumnWidth(AccrualsRModel::F6, 70);
  tableView_->setColumnWidth(AccrualsRModel::TPlan, 180);
  tableView_->setColumnWidth(AccrualsRModel::F7, 70);
  tableView_->setColumnWidth(AccrualsRModel::F8, 70);
  tableView_->setColumnWidth(AccrualsRModel::F9, 70);
  tableView_->setColumnWidth(AccrualsRModel::F10, 70);
  tableView_->setColumnWidth(AccrualsRModel::F11, 80);
}
