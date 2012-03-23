#include <QtGui>
#include <QtSql>

#include "FinallyDialog.h"


FinallyQueryModel::FinallyQueryModel(qint32 id, QObject *parent)
  : QSqlQueryModel(parent)
{
  QSqlQuery query;
  query.prepare(" SELECT f.date_"
		" ,(SELECT SUM(s.f1 + s.f2 + s.f3 + s.f4 + s.f5 + s.f6 + "
		"              sf.f1 + sf.f2 + sf.f3 + sf.f4)"
		"   FROM tb_summary s"
		"   ,tb_summaryFix sf"
		"   ,tb_abonents a"
		"   WHERE s.telA=sf.telA"
		"   AND f.clientID=a.clientID"
		"   AND s.telA=a.telA"
		"   AND s.date_=sf.date_"
		"   AND s.date_=f.date_) AS osum"
		" ,f.asum"
		" ,f.psum"
		" ,f.corrections"
		" ,f.peni"
		" ,f.previous"
		" ,f.asum - f.psum + f.corrections + f.peni + f.previous AS fsum"
		" FROM tb_finally f"
		" WHERE f.clientID=:id"
		" ORDER BY 1 DESC");

    query.bindValue(":id", id);
    query.exec();

    setQuery(query);
    setHeaderData(Date, Qt::Horizontal, trUtf8("Дата"));
    setHeaderData(OSum, Qt::Horizontal, trUtf8("Реально"));
    setHeaderData(ASum, Qt::Horizontal, trUtf8("Начисл."));
    setHeaderData(PSum, Qt::Horizontal, trUtf8("Плат.\nпр.пер."));
    setHeaderData(Correct, Qt::Horizontal, trUtf8("Корр."));
    setHeaderData(Peni, Qt::Horizontal, trUtf8("Пени"));
    setHeaderData(Prev, Qt::Horizontal, trUtf8("Пред.\nпериод"));
    setHeaderData(FSum, Qt::Horizontal, trUtf8("Итого"));
}

QVariant FinallyQueryModel::data(const QModelIndex &index, int role) const
{
  QVariant value = QSqlQueryModel::data(index, role);
  switch (role) {
  case Qt::FontRole:  
    if(index.column() == OSum) {
      // index.sibling(index.row(), OSum).isValid()) {
      const QAbstractItemModel *model = index.model();
      double osum = model->data(model->index(index.row(), OSum)).toDouble();
      double asum= model->data(model->index(index.row(), ASum)).toDouble();
      if(osum > asum )
	return qVariantFromValue(QFont("default", 10, QFont::Bold));
      
      else
	return value;
    }
    else
      return value;

      
  case Qt::DisplayRole:
    if (index.column() == OSum ||
	index.column() == ASum ||
	index.column() == PSum ||
	index.column() == Correct ||
	index.column() == Peni ||
	index.column() == Prev ||
	index.column() == FSum)
      return tr("%1").arg(value.toDouble(), 0, 'f', 2);
    else if (index.column() == Date)
      return value.toDateTime().toString("dd.MM.yyyy");
    else
      return value;

  case Qt::TextAlignmentRole: // Выравнивание
    if(index.column() == OSum ||
       index.column() == ASum ||
       index.column() == PSum ||
       index.column() == Correct ||
       index.column() == Peni ||
       index.column() == Prev ||
       index.column() == FSum)
      return double(Qt::AlignRight | Qt::AlignVCenter);
    else
      return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  return value;
}


// -- FinallyDialog -----------------------------------------------
FinallyDialog::FinallyDialog(qint32 id, const QString &caption, QWidget *parent)
  : QDialog(parent)
{
  tableModel_ = new FinallyQueryModel(id, this);
  tableView_ = new QTableView(this);
  tableView_->setModel(tableModel_);
  tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // tableView_->setColumnHidden(FinallyQueryModel::Id, true);
  
  tableView_->verticalHeader()->hide();
  tableView_->resizeColumnsToContents();
  // tableView_->setColumnWidth(FinallyQueryModel::Client, 180);
  tableView_->setAlternatingRowColors(true);
  tableView_->selectRow(0);
  
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(tableView_);
  setLayout(layout);

  setWindowTitle(trUtf8("История начислений [ %1 ]").arg(caption));
  setFixedWidth(tableView_->horizontalHeader()->length()+50);
  setFixedHeight(500);
}

FinallyDialog::~FinallyDialog()
{
  delete tableModel_;
}



