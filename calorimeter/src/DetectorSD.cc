#include "DetectorSD.hh"
#include "DetectorHit.hh"
#include "Connstants.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"

#include "G4ProcessManager.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4ParticleDefinition.hh"
#include "G4Proton.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Track.hh"
#include "Connstants.hh"

DetectorSD::DetectorSD(G4String name)
:G4VSensitiveDetector(name),
 fHitsCollection(nullptr),fHCID(-1)
{
	collectionName.insert("Coll");
}
DetectorSD::~DetectorSD()
{}

void DetectorSD::Initialize(G4HCofThisEvent* hce)
{
	fHitsCollection = new CalorHitsCollection(SensitiveDetectorName, collectionName[0]);
	if (fHCID<0) { 
	auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection ); 
}

   // for (auto column=0; column<kNofColumns; column++ ) {
  //  	for(auto row=0;row<kNofRows;row++){
   		fHitsCollection->insert(new DetectorHit());
   //	    }
 //   }
}
G4bool DetectorSD::ProcessHits(G4Step* step,G4TouchableHistory* /*history*/)
{
	auto edep = step->GetTotalEnergyDeposit();

	if(edep ==0. ) return true;

	auto touchable = (step->GetPreStepPoint()->GetTouchable());

	auto rowNo = touchable->GetCopyNumber(0);
	auto columnNo = touchable->GetCopyNumber(1);
	auto hitID = kNofRows*columnNo+rowNo;
	auto hit = (*fHitsCollection)[hitID];

	if(!hit){
		G4ExceptionDescription msg;
    msg << "Cannot access hit " << hitID; 
    G4Exception("DetectorSD::ProcessHits()",
      "MyCode0004", FatalException, msg);
	}

	if(hit->GetColumnID()<0){
		hit->SetColumnID(columnNo);
   		hit->SetRowID(rowNo);
	}
    
    hit->AddEdep(edep);
      
    G4OpBoundaryProcessStatus boundaryStatus=Undefined;
    static G4OpBoundaryProcess* boundary=NULL;
  //find the boundary process only once
     if(!boundary){
    G4ProcessManager* pm 
      = step->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i=0;i<nprocesses;i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary"){
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        break;
      }
    }
  }
    G4ParticleDefinition* particleType = step->GetTrack()->GetDefinition();
	if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){
 	   if(step->GetTrack()->GetParentID()>0){
 	   hit->AddNpe();
  	   }
    }
  	return true;
}
