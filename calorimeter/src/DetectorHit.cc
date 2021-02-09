#include "DetectorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<DetectorHit>* CalorHitAllocator = 0;

DetectorHit::DetectorHit()
 : G4VHit(),
   fEdep(0.),
   fTrackLength(0.)
{}

DetectorHit::~DetectorHit() {}

DetectorHit::DetectorHit(const DetectorHit& right)
  : G4VHit()
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
}
const DetectorHit& DetectorHit::operator=(const DetectorHit& right)
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DetectorHit::operator==(const DetectorHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorHit::Print()
{
  G4cout
     << "Edep: " 
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " track length: " 
     << std::setw(7) << G4BestUnit( fTrackLength,"Length")
     << G4endl;
}