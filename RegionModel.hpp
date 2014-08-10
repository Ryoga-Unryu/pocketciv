#ifndef REGIONMODEL_HPP
#define REGIONMODEL_HPP

#include <QObject>

class RegionModel : public QObject
{
    Q_OBJECT
    int region;
    int tribes;
    int movedTribes;
    bool mountain;
    bool volcano;
    bool forest;
    bool farm;
    bool desert;
    bool selected;
    bool city;
    int cityAV;

public:
    RegionModel(int region = -1, QObject *parent = 0);

public:
    int getTribeSupportCount() const;

    void toggleMountain();
    void toggleForest();

    void addToMovedTribes(int movedTribes);
    void mergeMovedTribes();

    // Get-Methods
    int getRegion() const;
    int getTribes() const;
    int getMovedTribes() const;
    bool hasMountain() const;
    bool hasVolcano() const;
    bool hasForest() const;
    bool hasFarm() const;
    bool hasDesert() const;
    bool isSelected() const;
    bool hasCity() const;
    int getCityAV() const;

    // Set-Methods
    void setTribes(int tribes);
    void unsetMountain();
    void unsetForest();
    void setDesert();
    void unsetDesert();
    void setSelected(bool selected);
};

#endif // REGIONMODEL_HPP
