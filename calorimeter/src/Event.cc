#include "Event.hh"
#include "DetectorHit.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "g4analysis.hh"
#include "globals.hh"

using std::array;
using std::vector;

namespace {

// Utility function which finds a hit collection with the given Id
// and print warnings if not found 
G4VHitsCollection* GetHC(const G4Event* event, G4int collId) {
  auto hce = event->GetHCofThisEvent();
  if (!hce) {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl; 
      G4Exception("Event::EndOfEventAction()",
                  "B5Code001", JustWarning, msg);
      return nullptr;
  }

  auto hc = hce->GetHC(collId);
  if ( ! hc) {
    G4ExceptionDescription msg;
    msg << "Hits collection " << collId << " of this event not found." << G4endl; 
    G4Exception("Event::EndOfEventAction()",
                "B5Code001", JustWarning, msg);
  }
  return hc;  
}

}

Event::Event()
 : G4UserEventAction(),
   fCalHCID(-1),
  fCalEdep{{ vector<G4double>(25., 0.) }}
{
  G4RunManager::GetRunManager()->SetPrintProgress(1);
}
Event::~Event()
{}

void Event::BeginOfEventAction(const G4Event*)
{
   auto sdManager = G4SDManager::GetSDMpointer();
   G4String cHCName = "HadCalorimeter/HadColl";
   fCalHCID = sdManager->GetCollectionID(cHCName);
}
void Event::EndOfEventAction(const G4Event* event)
{  
  auto analysisManager = G4AnalysisManager::Instance();

  // G4int totalCalHit = 0; 
   G4double totalCalEdep = 0;
   G4double Calu = 0;
   G4double totalNpe = 0;


   auto hc = GetHC(event, fCalHCID);
   if ( ! hc ) return;

   G4double edep = 0.;
   G4double edepu = 0.;
   G4double Npe = 0.;
   for (unsigned long i = 0; i < hc->GetSize(); ++i) {
      
        auto hit = static_cast<DetectorHit*>(hc->GetHit(i));
        edep = hit->GetEdep();
        Npe = hit->GetNpe();
      if ( edep > 0. ) {
        totalCalEdep += edep;
        totalNpe += Npe;
      }
        auto arandz = static_cast<DetectorHit*>(hc->GetHit(10));
        edepu = arandz->GetEdep();
        if(edepu >0.){
          Calu +=edepu;
        }
    //  fCalEdep[i] = edep;
    }
    analysisManager->FillNtupleDColumn(0, totalCalEdep);
    analysisManager->FillNtupleDColumn(1, Calu);
    analysisManager->FillNtupleDColumn(2, totalNpe);
  analysisManager->AddNtupleRow();
}

/*
   for (G4int iDet = 0; iDet < kDim; ++iDet) {
    
    
    
    analysisManager->FillNtupleDColumn(iDet + 2, totalCalEdep[iDet]);
  }
  

*/
