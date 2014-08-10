#include "DialogInstruction.hpp"

DialogInstruction::DialogInstruction(BoardModel *boardModel, Instruction *interruptedInstruction, QDialog *instructionDialog, QObject *parent)
    : Instruction(parent), boardModel(boardModel), interruptedInstruction(interruptedInstruction), instructionDialog(instructionDialog)
{
    this->interruptedInstruction->setKeepInstruction(true);
    connect(this->instructionDialog, SIGNAL(accepted()), this, SLOT(closeDialog()));
    connect(this->instructionDialog, SIGNAL(rejected()), this, SLOT(closeDialog()));
    this->instructionDialog->show();
    this->boardModel->disableButtons();
}

DialogInstruction::~DialogInstruction()
{
    this->instructionDialog->deleteLater();
}

Instruction *DialogInstruction::getFollowingInstruction()
{
    this->boardModel->enableDoneButton();
    this->interruptedInstruction->setKeepInstruction(false);
    return this->interruptedInstruction;
}

void DialogInstruction::closeDialog()
{
    this->boardModel->sendDialogClosed();
    return;
}

