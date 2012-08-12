#ifndef CONSOLEDIALOG_HPP
#define CONSOLEDIALOG_HPP

#include <QDialog>

namespace Ui {
class ConsoleDialog;
}

/**
  A dialog that displays a console with text in it
  */
class ConsoleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConsoleDialog(QWidget *parent = 0);
    ~ConsoleDialog();

    //To support debugging
    void appendOutput(QtMsgType type, const char *msg);

private:
    Ui::ConsoleDialog *ui;

public slots:

};

#endif // CONSOLEDIALOG_HPP
