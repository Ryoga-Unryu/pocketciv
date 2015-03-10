#ifndef CIVILWAREVENTINSTRUCTION_HPP
#define CIVILWAREVENTINSTRUCTION_HPP

#include "Instruction/Event/EventInstruction.hpp"
#include "BoardModel.hpp"

class CivilWarEventInstruction : public EventInstruction
{
    int colleteralDamage;
    int totalTribes;
    int step;
    int cityAVLost;

    QMap<int, RegionModel *> affectedRegions;

public:
    CivilWarEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();

private:
    void reduceColleteralDamageBy(int value);
};

#endif // CIVILWAREVENTINSTRUCTION_HPP
