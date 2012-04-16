#ifndef ABONENTDIALOG_H
#define ABONENTDIALOG_H

#include <QDialog>
#include <QHash>

class QLabel;
class QLineEdit;
class QDoubleSpinBox;
class QDateTimeEdit;
class QComboBox;
class QCheckBox;
class QDataWidgetMapper;
class QSqlRelationalTableModel;
class QSqlTableModel;
class QDialogButtonBox;

class AbonentDialog : public QDialog
{
    Q_OBJECT

public:
  AbonentDialog(qint32 id, quint8 userId, QWidget *parent = 0);
  ~AbonentDialog();
  double getAmount();
  enum { Id, Client, Abonent, TelA, TPlan, AbonPay, Operator, PBalance, Limit, Type };
    
private slots:
    void tplanChanged();
    void OkButtonPushed();

private:
    QSqlRelationalTableModel *tableModel;
    QSqlTableModel *clientModel;
    QSqlTableModel *tplanModel;
    QSqlTableModel *operatorModel;
    QSqlTableModel *typeModel;
    QDataWidgetMapper *mapper;

    QLabel *clientLabel;
    QLabel *abonentLabel;
    QLabel *telLabel;
    QLabel *tplanLabel;
    QLabel *abonPayLabel;
    QLabel *operatorLabel;
    QLabel *pBalanceLabel;
    QLabel *limitLabel;
    QLabel *typeLabel;

    QComboBox *clientComboBox;
    QLineEdit *abonentEdit;
    QLineEdit *telEdit;
    QComboBox *tplanComboBox;
    QDoubleSpinBox *abonPaySpinBox;
    QComboBox *operatorComboBox;
    QDoubleSpinBox *pBalanceSpinBox;
    QDoubleSpinBox *limitSpinBox;
    QComboBox *typeComboBox;
    
    QDialogButtonBox *buttonBox;

    QHash<QString, double> hashTplans;
    quint8 userId_;
    /* qint32 id_; */
    /* qint32 detailId_; */
    /* double qty_; */
    /* double nr_; */
};

#endif
