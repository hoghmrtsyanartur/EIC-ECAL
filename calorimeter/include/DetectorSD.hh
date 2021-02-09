#ifndef DetectorSD_h
#define DetectorSD_h 1

#include "G4VSensitiveDetector.hh"

#include "DetectorHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class DetectorSD : public G4VSensitiveDetector
{
  public:
    DetectorSD(const G4String& name, const G4String& hitsCollectionName,G4int nofCells);
    virtual ~DetectorSD();
  

    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    CalorHitsCollection* fHitsCollection;
    G4int  fNofCells;
};



#endif