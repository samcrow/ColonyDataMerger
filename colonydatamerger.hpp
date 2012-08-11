#ifndef COLONYDATAMERGER_HPP
#define COLONYDATAMERGER_HPP

#include <QMainWindow>
#include <QDebug>
#include <QDateTime>

#include <libmtp.h>
#include "../libcolonyutils/colony.hpp"

#include "mobiledevice.hpp"

namespace Ui {
class ColonyDataMerger;
}

class ColonyDataMerger : public QMainWindow
{
    Q_OBJECT

public:
    explicit ColonyDataMerger(QWidget *parent = 0);
    ~ColonyDataMerger();

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
};

#endif // COLONYDATAMERGER_HPP
