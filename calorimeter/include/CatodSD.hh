#ifndef CatodSD_h
#define CatodSD_h 1

#include "G4VSensitiveDetector.hh"

#include "CatodHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class CatodSD : public G4VSensitiveDetector
{
  public:
    CatodSD(G4String name);
    virtual ~CatodSD();
  

    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* ROhist);

  private:
    CalorHitsCollection* fHitsCollection;
    G4int fHCID;
};



#endif