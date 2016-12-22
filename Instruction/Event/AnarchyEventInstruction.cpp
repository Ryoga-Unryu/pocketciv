#include "AnarchyEventInstruction.hpp"

AnarchyEventInstruction::AnarchyEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{
    this->nextInstruction->setKeepInstruction(true);
}

void AnarchyEventInstruction::initInstruction()
{
    this->boardModel->printMessage("ANARCHY:");
    this->boardModel->printMessage("Press done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *AnarchyEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->step != -1)
    {
        return this;
    }

    if(regionModel && this->step == 1)
    {
        if(button == Qt::LeftButton &&
           this->boardModel->getCityAVCount() - this->boardModel->getAllSelectedCityAV() > 1 &&
           regionModel->getSelectedCityAV() < regionModel->getCityAV())
        {
            regionModel->setSelectedCityAV(regionModel->getSelectedCityAV() + 1);
        }
        else if(button == Qt::RightButton && regionModel->getSelectedCityAV() > 0)
        {
            regionModel->setSelectedCityAV(regionModel->getSelectedCityAV() - 1);
        }
    }

    return this;
}

Instruction *AnarchyEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step++;
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::LAW))
        {
            this->boardModel->printMessage("Advance (LAW):");
            this->boardModel->printMessage("In every region with a city, decimate as many tribes");
            this->boardModel->printMessage("as the City AV of that city.");
            this->boardModel->printMessage(" ");

            foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
            {
                if(regionModel->hasCity())
                {
                    int tribes = regionModel->getTribes();
                    int cityAV = regionModel->getCityAV();

                    regionModel->decimateTribes(cityAV);

                    this->boardModel->printMessage(QString("In region %1 there have been %2 tribes decimated.")
                                                  .arg(regionModel->getRegion())
                                                  .arg(tribes - regionModel->getTribes()));
                }
            }
        }
        else
        {
            int decimateTribes = 3;

            this->boardModel->printMessage(QString("In every region with a city, reduce 1 City AV and decimate 3 tribes."));
            this->boardModel->printMessage("This is repeated in every region, until the tribes are");
            this->boardModel->printMessage("lower than the City AV or the City AV is 1 in each region.");
            this->boardModel->printMessage(" ");

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::LITERACY))
            {
                decimateTribes += 2;
                this->boardModel->printMessage("Advance (LITERACY):");
                this->boardModel->printMessage("Decimate an additional 2 tribes in every region affected by ANARCHY.");
                this->boardModel->printMessage(" ");
            }

            foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
            {
                if(regionModel->hasCity() && regionModel->getCityAV() > 0)
                {
                    int cityAV = regionModel->getCityAV();
                    int tribes = regionModel->getTribes();

                    do
                    {
                        regionModel->setCityAV(regionModel->getCityAV()-1);
                        regionModel->setTribes(regionModel->getTribes()-decimateTribes);
                    }
                    while(regionModel->getCityAV() > 1 &&
                          regionModel->getCityAV() <= regionModel->getTribes());


                    int decimatedTribes = tribes-regionModel->getTribes();

                    QString tribeString = "tribe";

                    if(decimatedTribes > 1)
                    {
                        tribeString = "tribes";
                    }

                    this->boardModel->printMessage(QString("In region %1 there have been %2 City AV reduced and %3 %4 decimated.")
                                                  .arg(regionModel->getRegion())
                                                  .arg(cityAV-regionModel->getCityAV())
                                                  .arg(decimatedTribes)
                                                  .arg(tribeString));
                    this->boardModel->printMessage(" ");
                }
            }

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::MACHINING))
            {
                this->boardModel->printMessage("Advance (MACHINING):");
                this->boardModel->printMessage("Reduce 2 additional City AV from cities of your choice.");
                this->boardModel->printMessage(" ");
                int cityAVCount = this->boardModel->getCityAVCount();

                if(cityAVCount == 0)
                {
                    this->boardModel->printMessage("There is no City AV left in the empire.");
                    this->boardModel->printMessage("Therefore nothing further occurs.");
                    this->boardModel->printMessage(" ");
                }
                else if(cityAVCount == 1 || cityAVCount == 2)
                {
                    this->boardModel->printMessage(QString("There is %1 City AV left in the empire.").arg(cityAVCount));
                    this->boardModel->printMessage("Therefore all City AV has been reduced to 0.");
                    this->boardModel->printMessage(" ");

                    foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
                    {
                        regionModel->setCityAV(0);
                    }
                }
                else
                {
                    this->boardModel->printMessage("When you are done, press Done...");
                    this->boardModel->printMessage(" ");
                    return this;
                }
            }
        }
    }

    return this->endEvent();
}

