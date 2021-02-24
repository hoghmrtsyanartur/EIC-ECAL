#ifndef DetectorSD_h
#define DetectorSD_h 1

#include "G4VSensitiveDetector.hh"

#include "DetectorHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class DetectorSD : public G4VSensitiveDetector
{
  public:
    DetectorSD(G4String name);
    virtual ~DetectorSD();
  

    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* ROhist);

  private:
    CalorHitsCollection* fHitsCollection;
    G4int fHCID;
};



#endif