#ifndef Event_h
#define Event_h 1

#include "G4UserEventAction.hh"

#include "DetectorHit.hh"

#include "globals.hh"

class Event : public G4UserEventAction
{
public:
  Event();
  virtual ~Event();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);
    
private:
  // methods
  CalorHitsCollection* GetHitsCollection(G4int hcID,const G4Event* event) const;
  void PrintEventStatistics(G4double CrystalEdep, G4double CrystalTrackLength) const;
  
  // data members                   
  G4int  fCrystHCID;

};
                     

#endif