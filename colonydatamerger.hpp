#ifndef COLONYDATAMERGER_HPP
#define COLONYDATAMERGER_HPP

#include <QtGlobal>
#include <QMainWindow>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QVariant>
#include <QVariantList>
#include <QDir>
#include <QStringBuilder>

#include <libmtp.h>
#include <colony.hpp>

#include "mobiledevice.hpp"
#include "consoledialog.hpp"

namespace Ui {
class ColonyDataMerger;
}

class ColonyDataMerger : public QMainWindow
{
    Q_OBJECT

public:
    explicit ColonyDataMerger(QWidget *parent = 0);
    ~ColonyDataMerger();

    /**
      Convert a QVariantMap of parsed JSON into a list of colonies
      */
    static QList<Colony *> *jsonToColonyList(QVariantMap json);

    /**
      Parse JSON text into a QVariantMap of parsed JSON
      */
    static QVariantMap parseJson(QString jsonText);

private slots:

    void on_uploadButton_clicked();

    void on_downloadButton_clicked();

    void on_mergeButton_clicked();

private:
    Ui::ColonyDataMerger *ui;

    /**
      Merge two lists of colonies into one list that contains the most recently update data for each colony.

      If a colony is in one list but nont the other, it will be added to the final list.
      If a colony is in both lists, the colony with the more recent modified date will be added to the final list.
      If one of those colonies has a null modified date, the one with a non-null modified date is used
      If those two colonies have modified dates that are both null or equal, the colony from list 1 is used.

      This method does not allocate any new colonies; each colony in the returned list must
      have existed in one of the parameter lists.

      This method does not delete either of the parameter lists or any of its elements. This
      should be done manually after calling this method if desired.
      */
    static QList<Colony *> *mergeColonyLists(QList<Colony *> *list1, QList<Colony *> *list2);

    /**
      Select one colony of two as the one to to be used in the final list.
      If both colonies' modified dates are valid, returns the colony with the more recent date
      If one date is valid, returns the one with a valid date
      If neither date is valid, returns colony1.
      */
    static Colony *chooseColony(Colony *colony1, Colony *colony2);

    /**
      Show the console dialog
      */
    void showConsoleDialog();

    /**
      Convert a list of colonies into CSV
      */
    static QString toCSV(QList<Colony *> *list);
};

//This global (non-class) stuff is necessary. It handles routing of debug messages to the console.
namespace {
    ConsoleDialog *activeDialog = NULL;
}
//Global error/debug message handler
static void handleConsoleMessage(QtMsgType type, const char *msg) {
    if(activeDialog) {
        activeDialog->appendOutput(type, msg);
    }
    else {
        //Just print it to the standard error stream
        fprintf(stderr, "%s\n", msg);
    }
}

#endif // COLONYDATAMERGER_HPP
