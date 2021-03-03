#ifndef Event_h
#define Event_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>
#include <array>

const G4int kHad = 1;
const G4int kDim = 2;

class Event : public G4UserEventAction
{
public:
  Event();
  virtual ~Event();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);
  
  std::vector<G4double>& GetHadCalEdep() { return fCalEdep[kHad]; }
private:
  G4int fCalHCID;
  G4int TunID;
  std::array<std::vector<G4double>, 1> fCalEdep;

};
                     

#endif