#ifndef PAYDIALOG_H
#define PAYDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QDoubleSpinBox;
class QDateEdit;
class QDataWidgetMapper;
class QSqlTableModel;
class QDialogButtonBox;

class PayDialog : public QDialog
{
    Q_OBJECT

public:
  PayDialog(qint32 id, quint8 userId_, QWidget *parent = 0);
  ~PayDialog();
  double getAmount();
  enum { Id, TelA, Date, PDate, Summa, Prim, UserId, IsManual };
    
private slots:
    void OkButtonPushed();

private:
    QSqlTableModel *tableModel;
    QDataWidgetMapper *mapper;

    QLabel *telLabel;
    QLabel *pdateLabel;
    QLabel *summaLabel;
    QLabel *primLabel;

    QLineEdit *telEdit;
    QDateEdit *pdateEdit;
    QDoubleSpinBox *summaSpinBox;
    QLineEdit *primEdit;
    
    QDialogButtonBox *buttonBox;
    
    quint8 userId_;
};

#endif
