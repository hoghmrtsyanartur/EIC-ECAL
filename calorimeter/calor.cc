#include "DetectorConstruction.hh"
#include "Action.hh"

#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//#include "AddOptics.hh"

#include "G4SystemOfUnits.hh"

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " Calorimeter [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}
int main(int argc,char** argv)
{

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheSeed(time(NULL));
  // Evaluate arguments
  //
  if ( argc > 7 ) {
    PrintUsage();
    return 1;
  }
  
  G4String macro;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }  
  
  G4UIExecutive* ui = 0;
  if ( ! macro.size() ) {
    ui = new G4UIExecutive(argc, argv, session);
  }

  auto* runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
#ifdef G4MULTITHREADED
  if ( nThreads > 0 ) { 
    runManager->SetNumberOfThreads(nThreads);
  }  
#endif

  auto detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  auto physicsList = new FTFP_BERT;

  /*
  physicsList->SetCutValue(3.5*cm, "gamma");
  physicsList->SetCutValue(0.085*mm, "e-");
  physicsList->SetCutValue(0.085*mm, "e+");
  */
  //physicsList->RegisterPhysics(new npsAddOptics("Cerenkov & Scintillation") );

  runManager->SetUserInitialization(physicsList);
    
  auto actionInitialization = new Action();
  runManager->SetUserInitialization(actionInitialization);
  
  // Initialize visualization
  auto visManager = new G4VisExecutive;

  visManager->Initialize();

  auto UImanager = G4UImanager::GetUIpointer();

  if ( macro.size() ) {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  else  {  
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }


  delete visManager;
  delete runManager;
}