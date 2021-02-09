#include "Event.hh"
#include "DetectorSD.hh"
#include "DetectorHit.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

Event::Event()
 : G4UserEventAction(),
   fCrystHCID(-1)
{}
Event::~Event()
{}

CalorHitsCollection* 
Event::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection 
    = static_cast<CalorHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("Event::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    
void Event::PrintEventStatistics(G4double CrystalEdep, G4double CrystalTrackLength) const
{
  G4cout
     << "   Crystal: total energy: " 
     << std::setw(7) << G4BestUnit(CrystalEdep, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(CrystalTrackLength, "Length")
     << G4endl;
}
void Event::BeginOfEventAction(const G4Event* /*event */)
{}
void Event::EndOfEventAction(const G4Event* event)
{  
  // Get hits collections IDs (only once)
  if ( fCrystHCID == -1 ) {
    fCrystHCID
      = G4SDManager::GetSDMpointer()->GetCollectionID("CrystalHitsCollection");
    
  }
   auto CrystalHC = GetHitsCollection(fCrystHCID, event);

   auto CrystalHit = (*CrystalHC)[CrystalHC->entries()-1];

  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     

    PrintEventStatistics(CrystalHit->GetEdep(), CrystalHit->GetTrackLength());
  }

  auto analysisManager = G4AnalysisManager::Instance();


  analysisManager->FillH1(0, CrystalHit->GetEdep());
  analysisManager->FillH1(1, CrystalHit->GetTrackLength());

  analysisManager->FillNtupleDColumn(0, CrystalHit->GetEdep());
  analysisManager->FillNtupleDColumn(1, CrystalHit->GetTrackLength());
  analysisManager->AddNtupleRow(); 


}