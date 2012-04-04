#include <QtGui>
#include <QtSql>

#include "mainwindow.h"
#include "AbonentsWindow.h"
#include "ClientsDialog.h"
#include "AbonentTypesDialog.h"
#include "TPlansDialog.h"
#include "ServicesDialog.h"
#include "PaysDialog.h"
#include "DateInputDialog.h"


MainWindow::MainWindow(int userGroup)
  : userGroup_(userGroup)
{
    abonentsWindow = new AbonentsWindow;
    setCentralWidget(abonentsWindow);

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    // createStatusBar();

    //readSettings();

    //findDialog = 0;

    //setWindowIcon(QIcon(":/images/icon.png"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  writeSettings();
  event->accept();
}

/*
void MainWindow::find()
{
    if (!findDialog) {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(findNext(const QString &,
                                            Qt::CaseSensitivity)),
                spreadsheet, SLOT(findNext(const QString &,
                                           Qt::CaseSensitivity)));
        connect(findDialog, SIGNAL(findPrevious(const QString &,
                                                Qt::CaseSensitivity)),
                spreadsheet, SLOT(findPrevious(const QString &,
                                               Qt::CaseSensitivity)));
    }

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}
*/



void MainWindow::loadServices()
{
  QFileDialog fileDialog(this);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  if(!fileDialog.exec())
    return;

  DateInputDialog dialog(this);
  if(dialog.exec() != QDialog::Accepted)
    return;

  QString filename = fileDialog.selectedFiles()[0];
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::critical(0, trUtf8("Ошибка"),
	trUtf8("Ошибка при открытии файла %1").arg(filename));
    return;
  }

  QSqlQuery query("SELECT uid,UPPER(text) FROM tb_services ORDER BY 1");
  if(!query.exec()) {
    QMessageBox::critical(0, trUtf8("Ошибка"),
	trUtf8("Ошибка при чтении записей из таблицы tb_services !!"));
    return;
  }
  
  QHash<QString, int> hash;
  while(query.next())
    hash[query.value(1).toString()] = query.value(0).toInt();

  QString line, iQuery;
  QStringList list;
  int i = 0;
  double sum = 0, costR;
  
  QRegExp re("[0-9]{10}");
  QTextStream in(&file);
  
  while(!in.atEnd()) {
    line = in.readLine();
    list = line.split(QChar(';'));

    // если строка начинается с телефона
    if(re.exactMatch(list[0])) {
      // наименование состоит из двух столбцов
      QString text = list[1] + list[2];
      costR = list[3].replace(',','.').toDouble(); 

      // если в хеш-таблице есть запись с таким ключем
      if(hash.contains(text.toUpper())) {
	iQuery += tr("(NULL,%1,%2,%3,%3),").arg(list[0]).arg(hash.value(text.toUpper())).arg(costR);
	i++;
	sum += costR;
      }
      // если нет - добавляем запись в БД и в хеш-таблицу
      else {
	query.prepare(" INSERT INTO tb_services"
		      " VALUES(NULL,:text,:costR,0,:prim,0,0)");
	query.bindValue(":text", text);
	query.bindValue(":costR", costR);
	query.bindValue(":prim", list[4]);
	// если добавили новую услугу в БД
	if(query.exec()) {
	  // добавляем эту услугу в хеш-таблицу
	  hash[text.toUpper()] = query.lastInsertId().toInt();
	  iQuery += tr("(NULL,%1,%2,%3,%3),").arg(list[0])
	    .arg(hash.value(text.toUpper())).arg(costR);
	  i++;
	  sum += costR;
	}
	// если ошибка при записи в БД - пишем в файл
	else {
	  QFile tmpFile("servLoad.err");
	  if(tmpFile.open(QIODevice::Append | QIODevice::Text)) {
	    QTextStream out(&tmpFile);
	    out << line << "\n";
	    tmpFile.close();
	  }
	  // если ошибка и при записи в файл
	  else
	    QMessageBox::critical(0, trUtf8("Ошибка при загрузке"),line);
	}
      } // if(hash.contains())
    }
  } // while(!in.atEnd())
  if(!iQuery.isEmpty()) {
    // добавляем услуги во временную таблицу
    iQuery.insert(0, "INSERT INTO tb_servicesHistoryR VALUES ");
    if(iQuery.endsWith(QChar(',')))
      iQuery.remove(iQuery.size() - 1, 1);
    
    query.prepare(iQuery);
    if(!query.exec())
      QMessageBox::critical(0, trUtf8("Ошибка"),
			    trUtf8("Ошибка при добавлении записей в таблицу tb_servicesHistoryR !! [%1]").arg(query.lastError().text()));
    else {
      // переносим услуги в осн. таблицу tb_servicesHistoryRD
      // и удаляем из временной
      query.prepare(" INSERT INTO tb_servicesHistoryRD"
		    " SELECT 0,sh.telA,:date_,sh.serviceID"
		    " ,SUM(sh.costR),SUM(sh.cost) "
		    " FROM tb_services s,tb_servicesHistoryR sh"
		    " WHERE sh.serviceID=s.uid"
		    " GROUP BY 1,2,3,4");
      //" DELETE FROM tb_servicesHistoryR");
      query.bindValue(":date_", dialog.getDate());
      if(!query.exec())
	QMessageBox::critical(0, trUtf8("Ошибка"),
	  trUtf8("Ошибка при добавлении записей в таблицу tb_servicesHistoryRD !! [%1]").arg(query.lastError().text()));
      else {
	// обнуляем стоимость для "бесплатных" услуги
	query.prepare(" UPDATE tb_servicesHistoryRD"
		      " SET cost=0"
		      " WHERE date_=:date"
		      " AND serviceID IN (SELECT uid FROM tb_services WHERE ptype=1)");
	query.bindValue(":date", dialog.getDate());
	if(query.exec())
	  QMessageBox::information(0, trUtf8("Информация"),
		trUtf8("Добавлено %1 услуг на сумму %2").arg(i).arg(sum, 0, 'f', 2));
	else
	  QMessageBox::critical(0, trUtf8("Ошибка"),
	  trUtf8("Услуги загружены. Но \"бесплатные\" не обнулены!!\n[%1]").arg(query.lastError().text()));

      }
    }
  } // if(!iQuery.isEmpty())
}

void MainWindow::loadPays()
{
  QFileDialog fileDialog(this);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  if(!fileDialog.exec())
    return;

  QString filename = fileDialog.selectedFiles()[0];
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::critical(0, trUtf8("Ошибка"),
	trUtf8("Ошибка при открытии файла %1").arg(filename));
    return;
  }

  QString line, iQuery;
  int i = 0;
  double totalSum = 0;
  
  QRegExp reDate("[0-3][0-9].[0-1][0-9].[0-9]{4}");
  QRegExp reTel("[0-9]{10}");
  QTextStream in(&file);
  
  while(!in.atEnd()) {
    line = in.readLine();

    // если нашли дату
    if(reDate.exactMatch(line)) {
      // после даты 5 строк (1-сумма, 2-примечание1, 3-примечание2, 4-телефон,
      // 5-неограничен)  или 4 (1-сумма, 2-примечание1, 3-телефон, 4-неограничен)
      QString date = QDate::fromString(line,"dd.MM.yyyy").toString("yyyy-MM-dd");
      QString sum = in.readLine().split(' ')[0];
      QString place = in.readLine();
      QString text = in.readLine();
      QString telA, other;

      // если 3 строка - номер телефона, значит всего 4
      if(reTel.exactMatch(text)) {
	telA = text;
	text = "";
	// последняя (незначимая для нас) строка
	other = in.readLine();
      }
      // если еще не нашли номер телефона
      else {
	// считываем четвертую строку (она - телефон или незначимая)
	telA = in.readLine();

	// если нашли телефон - считываем последнюю(незначимую) строку
	if(reTel.exactMatch(telA))
	  other = in.readLine();
	// если не нашли - запрашиваем телефон у пользователя
	else{
	  bool ok = false;
	  while(!ok || telA.isEmpty())
	    telA = QInputDialog::getText(this, trUtf8("Введите номер телефона"),
			trUtf8("Платеж от %1<br>%2").arg(date).arg(text),
			QLineEdit::Normal, "", &ok);
	}
      }

      if(!text.isEmpty())
	place.insert(0, text + " ");

      i++;
      totalSum += sum.toDouble();
      iQuery += tr("('%1','%2',%3,'%4',2,0),").arg(telA).arg(date).arg(sum).arg(place);
    }
  }

  if(!iQuery.isEmpty()) {
    iQuery.insert(0, "INSERT INTO tb_pays(telA,pdate,msum,text,userID,isMan) VALUES ");
    if(iQuery.endsWith(QChar(',')))
      iQuery.remove(iQuery.size() - 1, 1);

    QSqlQuery query;
    query.prepare(iQuery);
    if(!query.exec())
      QMessageBox::critical(0, trUtf8("Ошибка"),
    	trUtf8("Ошибка при добавлении записей в таблицу tb_pays !! [%1]").arg(query.lastError().text()));
    else
      QMessageBox::information(0, trUtf8("Информация"),
    	trUtf8("Добавлено %1 платежей на сумму %2").arg(i).arg(totalSum, 0, 'f', 2));
  } // if(!iQuery.isEmpty())
  
}


void MainWindow::clients()
{
  ClientsDialog dialog(this);
  dialog.exec();
}

void MainWindow::abonentTypes()
{
  AbonentTypesDialog dialog(this);
  dialog.exec();
}


void MainWindow::tplans()
{
  TPlansDialog dialog(this);
  dialog.exec();
}

void MainWindow::services()
{
  ServicesDialog dialog(this);
  dialog.exec();
}

void MainWindow::pays()
{
  PaysDialog dialog(this);
  dialog.exec();
}

void MainWindow::calc4abonents()
{
  abonentsWindow->calculate("");
}

void MainWindow::calc4clients()
{
  if(ClientsDialog::calculate(0))
    QMessageBox::information(this, trUtf8("Информация"),
                         trUtf8("Рассчет окончен!!"),
                         QMessageBox::Ok);
  else
    QMessageBox::warning(this, trUtf8("Ошибка"),
                         trUtf8("Рассчет не был произведен!!"),
                         QMessageBox::Ok);
}


void MainWindow::about()
{
    // QMessageBox::about(this, tr("About Spreadsheet"),
    //         tr("<h2>Spreadsheet 1.1</h2>"
    //            "<p>Copyright &copy; 2008 Software Inc."
    //            "<p>Spreadsheet is a small application that "
    //            "demonstrates QAction, QMainWindow, QMenuBar, "
    //            "QStatusBar, QTableWidget, QToolBar, and many other "
    //            "Qt classes."));
}


// void MainWindow::updateStatusBar()
// {
//   locationLabel->setText(tr(""));
//   formulaLabel->setText(tr(""));
// }

/*
void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}
*/


void MainWindow::createActions()
{
    loadServicesAction = new QAction(trUtf8("Загрузка услуг"), this);
    connect(loadServicesAction, SIGNAL(triggered()), this, SLOT(loadServices()));

    loadPaysAction = new QAction(trUtf8("Загрузка платежей"), this);
    connect(loadPaysAction, SIGNAL(triggered()), this, SLOT(loadPays()));

    exitAction = new QAction(trUtf8("Выход"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(trUtf8("Выйти из программы"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    clientsAction = new QAction(trUtf8("Клиенты"), this);
    // clientsAction->setStatusTip(trUtf8("Информация по клиентам"));
    connect(clientsAction, SIGNAL(triggered()), this, SLOT(clients()));

    abonentTypesAction = new QAction(trUtf8("Типы абонентов"), this);
    // abonentTypesAction->setStatusTip(trUtf8("Информация по клиентам"));
    connect(abonentTypesAction, SIGNAL(triggered()), this, SLOT(abonentTypes()));

    tplansAction = new QAction(trUtf8("Тарифные планы"), this);
    // tplansAction->setStatusTip(trUtf8("Информация по клиентам"));
    connect(tplansAction, SIGNAL(triggered()), this, SLOT(tplans()));

    servicesAction = new QAction(trUtf8("Услуги"), this);
    // tplansAction->setStatusTip(trUtf8("Информация по клиентам"));
    connect(servicesAction, SIGNAL(triggered()), this, SLOT(services()));

    paysAction = new QAction(trUtf8("Платежи"), this);
    paysAction->setStatusTip(trUtf8("Информация по платежам"));
    connect(paysAction, SIGNAL(triggered()), this, SLOT(pays()));

    // monthsAction = new QAction(trUtf8("Архив"), this);
    // monthsAction->setStatusTip(trUtf8("Информация по закрытым месяцам"));
    // connect(monthsAction, SIGNAL(triggered()), this, SLOT(months()));

    calc4abonentsAction = new QAction(trUtf8("Рассчитать по всем абонентам"), this);
    // calc4abonentsAction->setStatusTip(trUtf8("Информация по платежам"));
    connect(calc4abonentsAction, SIGNAL(triggered()), this, SLOT(calc4abonents()));

    calc4clientsAction = new QAction(trUtf8("Рассчитать по всем клиентам"), this);
    // calc4abonentsAction->setStatusTip(trUtf8("Информация по платежам"));
    connect(calc4clientsAction, SIGNAL(triggered()), this, SLOT(calc4clients()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(trUtf8("&Файл"));
    //fileMenu->addSeparator();
    fileMenu->addAction(loadServicesAction);
    fileMenu->addAction(loadPaysAction);
    fileMenu->addAction(exitAction);

    refMenu = menuBar()->addMenu(trUtf8("&Справочники"));
    refMenu->addAction(clientsAction);
    refMenu->addAction(abonentTypesAction);
    refMenu->addAction(tplansAction);
    refMenu->addAction(servicesAction);
    refMenu->addAction(paysAction);
    // refMenu->addAction(monthsAction);


    //editMenu->addSeparator();
    
    

    toolsMenu = menuBar()->addMenu(trUtf8("&Инструменты"));
    toolsMenu->addAction(calc4abonentsAction);
    toolsMenu->addAction(calc4clientsAction);
    // toolsMenu->addAction(dealsAction);
    // toolsMenu->addAction(storagesAction);
    // toolsMenu->addAction(paysAction);
    //toolsMenu->addAction(recalculateAction);
    //toolsMenu->addAction(sortAction);

    reportsMenu = menuBar()->addMenu(trUtf8("&Отчеты"));
    //optionsMenu->addAction(showGridAction);
    //optionsMenu->addAction(autoRecalcAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(trUtf8("&Помощь"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createContextMenu()
{
    // spreadsheet->addAction(cutAction);
    // spreadsheet->addAction(copyAction);
    // spreadsheet->addAction(pasteAction);
    // spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    // fileToolBar = addToolBar(tr("&File"));
    // fileToolBar->addAction(newAction);
    // fileToolBar->addAction(openAction);
    // fileToolBar->addAction(saveAction);

    // editToolBar = addToolBar(tr("&Edit"));
    // editToolBar->addAction(cutAction);
    // editToolBar->addAction(copyAction);
    // editToolBar->addAction(pasteAction);
    // editToolBar->addSeparator();
    // editToolBar->addAction(findAction);
    // editToolBar->addAction(goToCellAction);
}

// void MainWindow::createStatusBar()
// {
//     locationLabel = new QLabel(" W999 ");
//     locationLabel->setAlignment(Qt::AlignHCenter);
//     locationLabel->setMinimumSize(locationLabel->sizeHint());

//     formulaLabel = new QLabel;
//     formulaLabel->setIndent(3);

//     statusBar()->addWidget(locationLabel);
//     statusBar()->addWidget(formulaLabel, 1);

//     // connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)),
//     //         this, SLOT(updateStatusBar()));
//     // connect(spreadsheet, SIGNAL(modified()),
//     //         this, SLOT(spreadsheetModified()));

//     updateStatusBar();
// }

void MainWindow::readSettings()
{
    // QSettings settings("Software Inc.", "Spreadsheet");

    // restoreGeometry(settings.value("geometry").toByteArray());

    // recentFiles = settings.value("recentFiles").toStringList();
    // updateRecentFileActions();

    // bool showGrid = settings.value("showGrid", true).toBool();
    // showGridAction->setChecked(showGrid);

    // bool autoRecalc = settings.value("autoRecalc", true).toBool();
    // autoRecalcAction->setChecked(autoRecalc);
}

void MainWindow::writeSettings()
{
    // QSettings settings("Software Inc.", "Spreadsheet");

    // settings.setValue("geometry", saveGeometry());
    // settings.setValue("recentFiles", recentFiles);
    // settings.setValue("showGrid", showGridAction->isChecked());
    // settings.setValue("autoRecalc", autoRecalcAction->isChecked());
}

