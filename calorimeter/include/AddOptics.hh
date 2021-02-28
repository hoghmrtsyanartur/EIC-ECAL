#ifndef AddOptics_hh
#define AddOptics_hh

#include "G4VPhysicsConstructor.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"

class AddOptics : public G4VPhysicsConstructor {

public:

  AddOptics(G4String aS) : G4VPhysicsConstructor(aS) {};

  virtual void ConstructParticle() 
  {
    // here we do nothing - or may be optical photons?
    G4OpticalPhoton::OpticalPhotonDefinition();
  };

  virtual void ConstructProcess() 
  {
    // here we can construct and register Cherenkov and other stuff.
    // Make sure nothing is there twice, though !!!

    // photonneri arajacman processner
    theCerenkovProcess=new G4Cerenkov();
    theScintillationProcess=new G4Scintillation();

    //photonner poxazdecutyan processner
    theAbsorptionProcess=new G4OpAbsorption();
    theRayleighScattering=new G4OpRayleigh();
    theBoundaryProcess=new G4OpBoundaryProcess();

    G4ProcessManager * pManager = 0;
    pManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

    auto theParticleIterator=GetParticleIterator();
    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
      G4ParticleDefinition* particle = theParticleIterator->value();
      pManager = particle->GetProcessManager();
      if(theCerenkovProcess->IsApplicable(*particle)){
	pManager->AddProcess(theCerenkovProcess);
	pManager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
      }
      if(theScintillationProcess->IsApplicable(*particle)){
      	pManager->AddProcess(theScintillationProcess);
	pManager->SetProcessOrdering(theScintillationProcess,idxPostStep);
      }
      if (particle->GetParticleName() == "opticalphoton") {
	G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;


	pManager->AddDiscreteProcess(theAbsorptionProcess);
	pManager->AddDiscreteProcess(theRayleighScattering);
	pManager->AddDiscreteProcess(theBoundaryProcess);
  //  pmanager->AddDiscreteProcess(fMieHGScatteringProcess);
      }

    }

    theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
    theCerenkovProcess->SetTrackSecondariesFirst(true);

    theScintillationProcess->SetTrackSecondariesFirst(true);

  }

private:

  G4Cerenkov* theCerenkovProcess;
  G4Scintillation* theScintillationProcess;
  G4OpAbsorption* theAbsorptionProcess;
  G4OpRayleigh* theRayleighScattering;
  G4OpBoundaryProcess* theBoundaryProcess;

}; 

#endif
