#ifndef DetectorConstruction_HH
#define DetectorConstruction_HH

#include "globals.hh"

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;



class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

    virtual void ConstructSDandField();
    
  private:
    
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
  
    // data members
    //
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
                                      // magnetic field messenger
    G4bool  fCheckOverlaps;
    G4int   NofLayers;
};

#endif
