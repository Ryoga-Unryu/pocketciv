#include "EventCard.hpp"

EventCard::EventCard(int redCircleNumber, int greenSquareNumber, int blueHexagonNumber, bool handshake, int goldNuggets, const QMap<int, Event *> &events, QObject *parent)
    : QObject(parent), handshake(handshake), goldNuggets(goldNuggets), events(events)
{
    this->shapeNumbers.insert(Event::RED_CIRCLE, redCircleNumber);
    this->shapeNumbers.insert(Event::GREEN_SQUARE, greenSquareNumber);
    this->shapeNumbers.insert(Event::BLUE_HEXAGON, blueHexagonNumber);
}

const Event *EventCard::getEvent(int era) const
{
    return this->events.value(era, NULL);
}

QMap<Event::NumberShapes, int> EventCard::getShapeNumbers() const
{
    return this->shapeNumbers;
}

int EventCard::getShapeNumberSum(const QMap<Event::NumberShapes, int> &shapeNumberAmounts) const
{
    int result = 0;

    foreach(Event::NumberShapes shape, shapeNumberAmounts.keys())
    {
        result += this->shapeNumbers.value(shape, 0)*shapeNumberAmounts.value(shape, 0);
    }

    return result;
}

bool EventCard::hasHandshake() const
{
    return this->handshake;
}

int EventCard::getGoldNuggets() const
{
    return this->goldNuggets;
}
