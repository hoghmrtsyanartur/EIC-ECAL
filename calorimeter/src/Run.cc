#include "Run.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

Run::Run()
 : G4UserRunAction()
{ 
	G4RunManager::GetRunManager()->SetPrintProgress(1);

	auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;

    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true);

    analysisManager->CreateH1("ECryst","Edep in Crystals", 100, 0., 100*GeV);
    analysisManager->CreateH1("LCryst","trackL in Crystals", 100, 0., 150*m);

    analysisManager->CreateNtuple("Calorimeter", "Edep and TrackL");
    analysisManager->CreateNtupleDColumn("ECryst");
    analysisManager->CreateNtupleDColumn("LCryst");
    analysisManager->FinishNtuple();
}

Run::~Run()
{
  delete G4AnalysisManager::Instance();  
}
void Run::BeginOfRunAction(const G4Run* /*run*/)
{ 
 
  auto analysisManager = G4AnalysisManager::Instance();

  
  G4String fileName = "Calorimeter";
  analysisManager->OpenFile(fileName);
}
void Run::EndOfRunAction(const G4Run* /*run*/)
{
  
  auto analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    /*
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }*/
    
    G4cout << " ECryst : mean = " 
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;
    
    G4cout << " LCryst : mean = " 
      << G4BestUnit(analysisManager->GetH1(1)->mean(), "Length") 
      << " rms = " 
      << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Length") << G4endl;
  }

  analysisManager->Write();
  analysisManager->CloseFile();
}