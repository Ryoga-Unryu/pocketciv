#include "TradeInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Instruction/StealingInstruction.hpp"

TradeInstruction::TradeInstruction(BoardModel *boardModel, Instruction *nextInstruction, QString what)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), what(what), step(0), goldGain(0)
{
    this->nextInstruction->setKeepInstruction(true);
}

void TradeInstruction::initInstruction()
{
    this->boardModel->printMessage(QString("%1 TRADE:").arg(this->what));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *TradeInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;
        this->goldGain = this->boardModel->drawCard()->getGoldNuggets();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->step = 2;
        this->boardModel->printMessage(QString("When trading with %1, you gained %2 gold.").arg(this->what).arg(this->goldGain));
        this->boardModel->gainGold(this->goldGain);
        this->boardModel->printMessage(QString("The %1 TRADE has ended.").arg(this->what));
        this->boardModel->printMessage(" ");
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CULTURE_OF_THIEVES) && this->step == 2)
    {
        this->step = 3;
        this->boardModel->printMessage("Advance (CULTURE OF THIEVES):");
        this->boardModel->printMessage(QString("Do you want to try and steal gold from %1?")
                                      .arg(this->what));

        this->boardModel->setDoneButton(false);
        this->stealingDecision.setWindowTitle("Advance (CULTURE OF THIEVES)");
        this->stealingDecision.setText("Do you want to try and steal gold from %1?");
        this->stealingDecision.addButton(QMessageBox::Yes);
        this->stealingDecision.addButton(QMessageBox::No);
        connect(&this->stealingDecision, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(stealingDecided(StandardButton)));
        this->stealingDecision.show();
    }
    else
    {
        this->step = -1;
    }

    if(this->step == -1)
    {
        this->boardModel->unsetActiveRegion();
        this->nextInstruction->setKeepInstruction(false);
        this->nextInstruction->initInstruction();
        return this->nextInstruction;
    }

    return this;
}

void TradeInstruction::stealingDecided(QAbstractButton *button)
{
    this->boardModel->setDoneButton(true);
    this->boardModel->setDoneText("Done");
    this->step = -1;

    QMessageBox::ButtonRole buttonRole =  this->stealingDecision.buttonRole(button);

    switch(buttonRole)
    {
        case QMessageBox::YesRole:
            this->boardModel->printMessage(QString("Begin stealing from %1...")
                                          .arg(this->what));
            this->nextInstruction = new StealingInstruction(this->boardModel, this->what, this->nextInstruction);
            break;
        default:
            this->boardModel->printMessage("You decided not to steal any gold.");
            break;
    }

    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to Continue...");
    this->boardModel->printMessage(" ");
}
