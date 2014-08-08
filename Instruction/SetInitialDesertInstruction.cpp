#include "SetInitialDesertInstruction.hpp"

#include "Instruction/PlaceInitialTribes.hpp"

SetInitialDesertInstruction::SetInitialDesertInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void SetInitialDesertInstruction::initInstruction()
{
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("No regions without mountain and/or forest found to place a desert.");
    this->boardModel->sendMessage("Place ONE desert into one region without a forest.");
    this->boardModel->sendMessage("When you are done, press Done...");
}

Instruction *SetInitialDesertInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

        if(!regionModel->hasForest())
        {
            foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
            {
                regionModel->unsetDesert();
            }

            if(regionModel != NULL)
            {
                regionModel->setDesert();
            }
        }
    }

    return this;
}

Instruction *SetInitialDesertInstruction::triggerDone()
{
    if(this->boardModel->getDesertCount() == 1)
    {
        Instruction *next = new PlaceInitialTribes(this->boardModel);
        next->initInstruction();
        return next;
    }
    else
    {
        this->boardModel->sendMessage("No desert set.");
    }

    return this;
}
