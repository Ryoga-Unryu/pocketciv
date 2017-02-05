#include "WonderDialog.hpp"

#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>

WonderDialog::WonderDialog(BoardModel *boardModel, WonderDescription::WonderDescriptionType wonderDescriptionType, int region, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      isCompact(true),
      WONDERS_DIALOG_SIZE_COMPACT(789,373),
      WONDERS_DIALOG_SIZE_FULL(100, 60),
      BUTTON_TEXT_COMPACT("FULL VIEW"),
      BUTTON_TEXT_FULL("COMPCAT VIEW"),
      wonderDescriptionType(wonderDescriptionType)
{    
    if(region == -1)
    {
        wonderDescriptionType = WonderDescription::OVERVIEW;
    }

    QString windowTitleText = "UNKNOWN";
    switch(wonderDescriptionType)
    {
        case WonderDescription::OVERVIEW: windowTitleText = "Overview"; break;
        case WonderDescription::REGION_OVERVIEW: windowTitleText = QString("Region %1").arg(region); break;
        case WonderDescription::BUILD: windowTitleText = QString("Build Wonder in Region %1").arg(this->boardModel->refActiveRegion()->getRegion()); break;
        case WonderDescription::SELECTION: windowTitleText = "Selection"; break;
    }

    this->setWindowTitle(QString("Wonders (%1)").arg(windowTitleText));
    this->wonderLayout = new QGridLayout(this);

    QMap<WonderModel::Wonder, int> wonders;

    if(wonderDescriptionType == WonderDescription::REGION_OVERVIEW || wonderDescriptionType == WonderDescription::SELECTION)
    {
        wonders = this->boardModel->refRegionModel(region)->getBuiltWonders();
    }
    else
    {
        wonders = this->boardModel->getAllWonders();
    }

    this->wonderTable = new WonderTable(this->boardModel, wonders, wonderDescriptionType, this);
    connect(this->wonderTable, SIGNAL(closeTable()), this, SLOT(accept()));

    if(wonderDescriptionType == WonderDescription::SELECTION)
    {
        connect(this->wonderTable, SIGNAL(wonderSelectionChanged(WonderModel::Wonder,int)), this, SLOT(updateWonderSelection(WonderModel::Wonder,int)));
    }

    this->resizeButton = new QPushButton(this->BUTTON_TEXT_COMPACT, this);
    connect(this->resizeButton, SIGNAL(clicked()), this, SLOT(toggleSize()));

    this->submitButton = new QPushButton("Submit Selection", this);
    connect(this->submitButton, SIGNAL(clicked()), this, SLOT(submitWonders()));

    this->setCompactSize();

    this->setLayout(this->wonderLayout);
    this->wonderLayout->addWidget(this->wonderTable, 0, 0, 1,2);

    if(wonderDescriptionType == WonderDescription::SELECTION)
    {
        this->wonderLayout->addWidget(this->resizeButton, 1, 0);
        this->wonderLayout->addWidget(this->submitButton, 1, 1);
    }
    else
    {
        this->wonderLayout->addWidget(this->resizeButton, 1, 0 ,1,2);
        this->submitButton->setVisible(false);
    }
}

void WonderDialog::setCompactSize()
{
    QDesktopWidget widget;
    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    this->resizeButton->setText(this->BUTTON_TEXT_COMPACT);
    this->isCompact = true;
    this->wonderTable->setCompactColumns();
    this->setFixedSize(this->WONDERS_DIALOG_SIZE_COMPACT.width(),this->WONDERS_DIALOG_SIZE_COMPACT.height());
    this->move(mainScreenSize.center() - this->rect().center());
    return;
}

void WonderDialog::setFullSize()
{
    QDesktopWidget widget;
    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    this->resizeButton->setText(this->BUTTON_TEXT_FULL);
    this->isCompact = false;
    this->wonderTable->setFullColumns();
    this->setFixedSize(mainScreenSize.width() - this->WONDERS_DIALOG_SIZE_FULL.width(), mainScreenSize.height() - this->WONDERS_DIALOG_SIZE_FULL.height());
    this->move(mainScreenSize.center() - this->rect().center());
    return;
}

void WonderDialog::setSelectionTotal(int selectionTotal)
{
    this->wonderTable->setSelectionTotal(selectionTotal);
    this->setWindowTitle(QString("Wonders (Selection 0/%1)").arg(selectionTotal));
    this->submitButton->setEnabled(false);
    return;
}

QMap<WonderModel::Wonder, int> WonderDialog::getSelectedWonders() const
{
    return this->wonderTable->getSelectedWonders();
}

void WonderDialog::show()
{
    this->wonderTable->update();
    this->QDialog::show();
}

int WonderDialog::exec()
{
    this->wonderTable->update();
    return this->QDialog::exec();
}

void WonderDialog::toggleSize()
{
    if(this->isCompact)
    {
        this->setFullSize();
    }
    else
    {
        this->setCompactSize();
    }
}

void WonderDialog::updateWonderSelection(WonderModel::Wonder, int)
{
    QMap<WonderModel::Wonder, int> selectedWonders = this->wonderTable->getSelectedWonders();
    int wonderCountTotal = 0;

    foreach(int wonderCount, selectedWonders.values())
    {
        wonderCountTotal += wonderCount;
    }

    this->submitButton->setEnabled(wonderCountTotal == this->wonderTable->getSelectionTotal());
    this->setWindowTitle(QString("Wonders (Selection %1/%2)").arg(wonderCountTotal).arg(this->wonderTable->getSelectionTotal()));
    return;
}

void WonderDialog::submitWonders()
{
    QMessageBox::StandardButton result = QMessageBox::question(this, "Submit selected Wonders?", "Do you really want to submit the selected wonders?", QMessageBox::Yes | QMessageBox::No);

    if(result == QMessageBox::Yes)
    {
        this->close();
    }
}
