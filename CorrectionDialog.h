#ifndef CORRECTIONDIALOG_H
#define CORRECTIONDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QComboBox;
class QDoubleSpinBox;
class QDateEdit;
class QDataWidgetMapper;
class QSqlRelationalTableModel;
class QSqlTableModel;
class QDialogButtonBox;

class CorrectionDialog : public QDialog
{
    Q_OBJECT

public:
  CorrectionDialog(qint32 id, quint8 userId_, QWidget *parent = 0);
  ~CorrectionDialog();
  double getAmount();
  enum { Id, Client, TelA, Date, Summa, Prim, UserId };
    
private slots:
    void OkButtonPushed();

private:
    QSqlRelationalTableModel *tableModel;
    QSqlTableModel *clientModel;
    QDataWidgetMapper *mapper;

    QLabel *clientLabel;
    QLabel *telLabel;
    QLabel *dateLabel;
    QLabel *summaLabel;
    QLabel *primLabel;

    QComboBox *clientComboBox;
    QLineEdit *telEdit;
    QDateEdit *dateEdit;
    QDoubleSpinBox *summaSpinBox;
    QLineEdit *primEdit;
    
    QDialogButtonBox *buttonBox;
    
    quint8 userId_;
};

#endif
