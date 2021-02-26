#ifndef PrimeryGenerator_h
#define PrimeryGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGenerator();    
  virtual ~PrimaryGenerator();

  virtual void GeneratePrimaries(G4Event* event);

  void SetOptPhotonPolar();
  void SetOptPhotonPolar(G4double);
  
  // set methods
  void SetRandomFlag(G4bool value);

private:
  G4ParticleGun*  fParticleGun; // G4 particle gun
};

#endif