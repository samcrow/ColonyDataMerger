#include "consoledialog.hpp"
#include "ui_consoledialog.h"

ConsoleDialog::ConsoleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConsoleDialog)
{
    ui->setupUi(this);

    //Hook stuff up so that the dialog will close when the close button is pressed
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void ConsoleDialog::appendOutput(QtMsgType /*type*/, const char *msg) {
    //Print it to stderr too.
    fprintf(stderr, "%s\n", msg);
    ui->text->appendPlainText(msg);
}

ConsoleDialog::~ConsoleDialog()
{
    delete ui;
}
