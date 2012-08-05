#ifndef COLONYDATAMERGER_HPP
#define COLONYDATAMERGER_HPP

#include <QMainWindow>
#include <QDebug>

#include <libmtp.h>

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
};

#endif // COLONYDATAMERGER_HPP
