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
  int getUserGroup() { return userGroup_; }

private slots:
  void ok();
  
private:
  QLabel *userLabel;
  QLabel *passLabel;
  QComboBox *userCombo;
  QLineEdit *passEdit;

  QDialogButtonBox *buttonBox;
  int userGroup_;
};

#endif
