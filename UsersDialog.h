#ifndef USERSDIALOG_H
#define USERSDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QComboBox;
class QDialogButtonBox;

class UsersDialog : public QDialog
{
    Q_OBJECT

public:
  UsersDialog(QWidget *parent = 0);
  int getUserId() { return userId_; }
  int getUserGr() { return userGr_; }

private slots:
  void ok();
  
private:
  QLabel *userLabel;
  QLabel *passLabel;
  QComboBox *userCombo;
  QLineEdit *passEdit;

  QDialogButtonBox *buttonBox;
  quint8 userId_, userGr_;
};

#endif
