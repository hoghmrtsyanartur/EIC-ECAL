#include "Run.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "Run.hh"
#include "Event.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericAnalysisManager.hh"

using G4AnalysisManager = G4GenericAnalysisManager;

Run::Run(Event* event)
 : G4UserRunAction(),
 fEvent(event)
{ 
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);

  analysisManager->SetNtupleMerging(true);
  analysisManager->SetFileName("Call");

  if(fEvent){
    analysisManager->CreateNtuple("Call", "Hits");
    analysisManager->CreateNtupleDColumn("Total"); // column Id = 0
    analysisManager->CreateNtupleDColumn("photonner");
    analysisManager->FinishNtuple();
  }
}

Run::~Run()
{
  delete G4AnalysisManager::Instance();  
}
void Run::BeginOfRunAction(const G4Run* /*run*/)
{ 
 
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->OpenFile();
}
void Run::EndOfRunAction(const G4Run* /*run*/)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  
}