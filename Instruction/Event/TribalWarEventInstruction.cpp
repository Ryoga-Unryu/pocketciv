#include "TribalWarEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Common.hpp"

TribalWarEventInstruction::TribalWarEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0), warringTribes(0), borderingRegionsLeft(2)
{}

void TribalWarEventInstruction::initInstruction()
{
    this->boardModel->sendMessage("TRIBAL WAR:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue.");
    this->boardModel->sendMessage(" ");
    return;
}

Instruction *TribalWarEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->step != -1)
    {
        return this;
    }

    if(button == Qt::RightButton && this->borderingRegions.contains(regionModel->getRegion()) && this->borderingRegionsLeft > 0)
    {
        this->borderingRegionsLeft--;
        regionModel->decimateTribes(this->warringTribes);
        this->boardModel->sendMessage(QString("The tribes of region %1 are reduced by %2.")
                                      .arg(regionModel->getRegion())
                                      .arg(this->warringTribes));

        if(this->borderingRegionsLeft == 0)
        {
            this->boardModel->refActiveRegion()->decimateTribes(3);
            this->boardModel->sendMessage(" ");
            this->boardModel->sendMessage("Reduce 3 tribes in the active region because of the tribal war uproar.");
            return this->endEvent();
        }
    }

    return this;
}

Instruction *TribalWarEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step++;
        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->step = -1;

        RegionModel *activeRegion = this->boardModel->refActiveRegion();
        int tribes = activeRegion->getTribes();

        if(tribes == 0)
        {
            this->boardModel->sendMessage("The active region has NO tribes.");
            this->boardModel->unsetActiveRegion();
            return this->endEvent();
        }
        else
        {
            this->warringTribes = tribes*2;
            this->boardModel->sendMessage(QString("The active region has %1 warring tribes.").arg(this->warringTribes));
        }

        QMap<int, RegionModel *> possibleRegions = this->boardModel->getAdjacentRegions(activeRegion->getRegion());

        foreach(int region, possibleRegions.keys())
        {
            RegionModel *regionModel = possibleRegions.value(region);

            if(regionModel->hasTribes())
            {
                this->borderingRegions.insert(region, regionModel);
            }
        }

        this->borderingRegionsLeft = 0;

        this->boardModel->sendMessage(" ");

        if(this->borderingRegions.size() == 0)
        {
            this->boardModel->sendMessage("There are no bordering regions with tribes.");
        }
        else if(this->borderingRegions.size() == 1)
        {
            this->boardModel->sendMessage("There is only one bordering region with tribes.");
            this->borderingRegions.values().at(0)->decimateTribes(this->warringTribes);
            this->boardModel->sendMessage(QString("The tribes of region %1 are reduced by %2.")
                                          .arg(this->borderingRegions.values().at(0)->getRegion())
                                          .arg(this->warringTribes));
        }
        else if(this->borderingRegions.size() == 2)
        {
            this->boardModel->sendMessage("There are exactly two bordering region with tribes.");
            this->borderingRegions.values().at(0)->decimateTribes(this->warringTribes);
            this->boardModel->sendMessage(QString("The tribes of region %1 are reduced by %2.")
                                          .arg(this->borderingRegions.values().at(0)->getRegion())
                                          .arg(this->warringTribes));
            this->borderingRegions.values().at(1)->decimateTribes(this->warringTribes);
            this->boardModel->sendMessage(QString("The tribes of region %1 are reduced by %2.")
                                          .arg(this->borderingRegions.values().at(1)->getRegion())
                                          .arg(this->warringTribes));
        }
        else
        {
            this->boardModel->sendMessage(QString("Possible regions to wage war against are regions $1.").arg(Common::listUpRegions(possibleRegions.values())));
            this->boardModel->sendMessage("Choose two of those regions.");
            this->boardModel->sendMessage("In those regions, the tribes are reduced by the amount of warring tribes.");
            this->borderingRegionsLeft = 2;
            return this;
        }
    }

    if(this->step == -1 && this->borderingRegionsLeft > 0)
    {
        return this;
    }

    this->boardModel->refActiveRegion()->decimateTribes(3);
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Reduce 3 tribes in the active region because of the tribal war uproar.");
    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}