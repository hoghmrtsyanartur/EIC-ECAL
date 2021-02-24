#include "DetectorHit.hh"
#include "DetectorConstruction.hh"


#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<DetectorHit>* CalorHitAllocator = 0;

DetectorHit::DetectorHit()
 : G4VHit(),
   fColumnID(-1), fRowID(-1), fEdep(0.), fNpe(0.)
{}

DetectorHit::DetectorHit(G4int columnID,G4int rowID)
: G4VHit(),
  fColumnID(columnID),fRowID(rowID),fEdep(0.),fNpe(0.)
 {}

DetectorHit::~DetectorHit()
{}

DetectorHit::DetectorHit(const DetectorHit& right)
  : G4VHit(),
  fColumnID(right.fColumnID),
  fRowID(right.fRowID),
  fEdep(right.fEdep),
  fNpe(right.fNpe)
{}

const DetectorHit& DetectorHit::operator=(const DetectorHit& right)
{
  fColumnID = right.fColumnID;
  fRowID = right.fRowID;
  fEdep = right.fEdep;
  fNpe = right.fNpe;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DetectorHit::operator==(const DetectorHit& right) const
{
  return (fColumnID==right.fColumnID&&fRowID==right.fRowID);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
const std::map<G4String,G4AttDef>* DetectorHit::GetAttDefs() const
{
  G4bool isNew;
  auto store = G4AttDefStore::GetInstance("DetectorHit",isNew);

  if (isNew) {
    (*store)["HitType"] 
      = G4AttDef("HitType","Hit Type","Physics","","G4String");
    
    (*store)["Column"] 
      = G4AttDef("Column","Column ID","Physics","","G4int");
    
    (*store)["Row"] 
      = G4AttDef("Row","Row ID","Physics","","G4int");
    
    (*store)["Energy"] 
      = G4AttDef("Energy","Energy Deposited","Physics","G4BestUnit",
                 "G4double");
  }
  return store;
}
std::vector<G4AttValue>* DetectorHit::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;
  
  values
    ->push_back(G4AttValue("HitType","HadCalorimeterHit",""));
  values
    ->push_back(G4AttValue("Column",G4UIcommand::ConvertToString(fColumnID),
                           ""));
  values
    ->push_back(G4AttValue("Row",G4UIcommand::ConvertToString(fRowID),""));
  values
    ->push_back(G4AttValue("Energy",G4BestUnit(fEdep,"Energy"),""));
  
  return values;
}

void DetectorHit::Print()
{
  G4cout << "  Cell[" << fRowID << ", " << fColumnID << "] "
    << fEdep/MeV << " (MeV) " << G4endl;
}