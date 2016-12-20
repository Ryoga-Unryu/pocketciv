#include "MoveTribesInstruction.hpp"

#include "Instruction/MainPhaseInstruction.hpp"
#include "Instruction/DialogInstruction.hpp"
#include "Instruction/EndOfEraInstruction.hpp"

MoveTribesInstruction::MoveTribesInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel), moveTribeDialog(NULL)
{}

MoveTribesInstruction::~MoveTribesInstruction()
{
    this->moveTribeDialog->deleteLater();
}

void MoveTribesInstruction::initInstruction()
{
    this->boardModel->printMessage("POPULATION MOVEMENT:");
    this->boardModel->printMessage("Move tribes from one to another region.");
    if(this->boardModel->hasAdvanceAquired(AdvanceModel::EQUESTRIAN))
    {
        this->boardModel->printMessage(" ");
        this->boardModel->printMessage("Advance (EQUESTRIAN):");
        this->boardModel->printMessage("Every tribe can be moved once into any");
        this->boardModel->printMessage("region not seperated by a SEA or FRONTIER.");
        this->boardModel->printMessage(" ");
    }
    else
    {
        this->boardModel->printMessage(" ");
        this->boardModel->printMessage("Every tribe can be moved once into an adjacent region.");
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("When you are done, press Done...");
    this->boardModel->printMessage(" ");
}

Instruction *MoveTribesInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);
        if(regionModel == NULL)
        {
            return this;
        }

        QList<RegionModel *> selectedRegions = this->boardModel->getSelectedRegions().values();

        if(selectedRegions.isEmpty())
        {

            if(regionModel->getTribes() > 0)
            {
                this->boardModel->setSelectRegion(regionModel->getRegion(), true);
            }
        }
        else if(selectedRegions.size() == 1)
        {
            if(selectedRegions.contains(regionModel))
            {
                this->boardModel->setSelectRegion(regionModel->getRegion(), false);
            }
            else
            {
                RegionModel *fromRegion = selectedRegions.first();
                RegionModel *toRegion = this->boardModel->refRegionModel(x,y);

                if(this->boardModel->canMoveTribes(fromRegion->getRegion(), toRegion->getRegion()))
                {
                    this->boardModel->setSelectRegion(toRegion->getRegion(), true);
                    this->moveTribeDialog = new MoveTribeDialog(
                                this->boardModel,
                                QString("of %1 Tribes from region %2 to region %3.")
                                .arg(QString::number(fromRegion->getTribes()))
                                .arg(fromRegion->getRegion())
                                .arg(toRegion->getRegion()),
                                fromRegion->getTribes(),
                                fromRegion,
                                toRegion);
                    return new DialogInstruction(
                                this->boardModel,
                                this,
                                this->moveTribeDialog);

                }
            }
        }
    }

    return this;
}

Instruction *MoveTribesInstruction::triggerDone()
{
    const Event *event = this->boardModel->drawOriginalCard()->getEvent(this->boardModel->getEra());
    Instruction *main = new MainPhaseInstruction(this->boardModel);

    if(event == NULL)
    {
        this->boardModel->printMessage("Nothing occured...");
        this->boardModel->printMessage(" ");
        main->initInstruction();
        return main;
    }

    this->boardModel->mergeAllMovedTribes();
    this->boardModel->unselectAllRegions();
    this->boardModel->printMessage(" ");
    Instruction *eventInstruction = event->happen(main);
    eventInstruction->initInstruction();
    Instruction *next;

    if(this->boardModel->isEndOfEra())
    {
        next = new EndOfEraInstruction(this->boardModel, eventInstruction);
        next->initInstruction();
    }
    else
    {
        next = eventInstruction;
    }

    return next;
}
