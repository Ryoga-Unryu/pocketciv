#ifndef WONDERSTABLE_HPP
#define WONDERSTABLE_HPP

#include <QTableWidget>

#include "BoardModel.hpp"
#include "WonderDescription.hpp"
#include "IntegerTableItem.hpp"

class WondersTable : public QTableWidget
{
    Q_OBJECT

signals:
    void closeTable();

private:
    BoardModel *boardModel;
    QMap<WonderModel::Wonder, int> wonders;
    WonderDescription::WonderDescriptionType wonderDescriptionType;

    QMap<WonderModel::Wonder, QTableWidgetItem *> wonderTableItems;
    QMap<WonderModel::Wonder, WonderDescription *> wonderDescriptions;

public:
    WondersTable(BoardModel *boardModel,
                 QMap<WonderModel::Wonder, int> wonders,
                 WonderDescription::WonderDescriptionType wonderDescriptionType = WonderDescription::OVERVIEW,
                 QWidget *parent = 0);

    void setCompactColumns();
    void setFullColumns();

private slots:
    void selectWonder();
    void triggerWonderBuilt(WonderModel::Wonder wonder);
};

#endif // WONDERSTABLE_HPP
