#ifndef DetectorConstruction_HH
#define DetectorConstruction_HH

#include "globals.hh"

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;
class G4VSensitiveDetector;



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
    G4LogicalVolume* fPWO_LV;

    G4double air_gap;
    G4int refFlag;
    G4String refName;
    G4int refNumData;
    G4double* refWL;
    G4double* refReIndex;
    G4double* refImIndex;
    G4double subRefrIndex;

    G4double* refRefl;
    G4LogicalVolume* block_log;
    G4LogicalVolume* mylar_log;
    G4LogicalVolume* PMTWin_right_log;
    G4LogicalVolume* PMTHouse_log;
    G4LogicalVolume* Cathode_log;
    G4LogicalVolume* glue_log;
    G4LogicalVolume* tedlar_log;
    G4LogicalVolume* counter_log;


};

#endif
