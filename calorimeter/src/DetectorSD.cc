#include "DetectorSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

DetectorSD::DetectorSD(const G4String& name,const G4String& hitsCollectionName,G4int nofCells)
:G4VSensitiveDetector(name),
 fHitsCollection(nullptr),
 fNofCells(nofCells)
{
	collectionName.insert(hitsCollectionName);
}
DetectorSD::~DetectorSD()
{}

void DetectorSD::Initialize(G4HCofThisEvent* hce)
{
	fHitsCollection = new CalorHitsCollection(SensitiveDetectorName, collectionName[0]);

	auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    hce->AddHitsCollection( hcID, fHitsCollection ); 

    for (G4int i=0; i<fNofCells+1; i++ ) {
   		fHitsCollection->insert(new DetectorHit());
    }
}
G4bool DetectorSD::ProcessHits(G4Step* step,G4TouchableHistory* /*history*/)
{
	auto edep = step->GetTotalEnergyDeposit();

	G4double stepLength = 0.;
	if(step->GetTrack()->GetDefinition()->GetPDGCharge() !=0.){
		stepLength = step->GetStepLength();
	}

	if(edep ==0. && stepLength == 0.) return false;

	auto touchable = (step->GetPreStepPoint()->GetTouchable());

	auto layerNumber = touchable->GetReplicaNumber(1);

	auto hit = (*fHitsCollection)[layerNumber];
	if(!hit){
		G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber; 
    G4Exception("DetectorSD::ProcessHits()",
      "MyCode0004", FatalException, msg);
	}
	auto hitTotal = (*fHitsCollection)[fHitsCollection->entries()-1];
    hit->Add(edep, stepLength);
  	hitTotal->Add(edep, stepLength); 
      
  	return true;
}
void DetectorSD::EndOfEvent(G4HCofThisEvent*){
	if(verboseLevel>1){
		auto nOfHits = fHitsCollection->entries();
		G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nOfHits 
       << " hits in the tracker chambers: " << G4endl;
     for ( std::size_t i=0; i<nOfHits; ++i ) (*fHitsCollection)[i]->Print();
	}
}