#include "DetectorConstruction.hh"
#include "DetectorSD.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"


#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4VSensitiveDetector.hh"

#include <vector>
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SubtractionSolid.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4ReplicatedSlice.hh"
#include "G4GenericMessenger.hh"
#include "Connstants.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4OpticalSurface.hh"

#include <fstream>

using namespace std;
G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0;


DetectorConstruction::DetectorConstruction()
:   G4VUserDetectorConstruction(),
    fCheckOverlaps(true),
    fPWO_LV(nullptr)
{
}

DetectorConstruction::~DetectorConstruction()
{ 
}  

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}
void DetectorConstruction::DefineMaterials()
{
  
}
G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{


     
  ifstream fin;
  fin.open("reflector.inp");

  G4String line;
  getline(fin,line); istringstream iss1(line);
  iss1 >> air_gap;
  getline(fin,line); istringstream iss2(line);
  iss2 >> refFlag;
  getline(fin,line); istringstream iss3(line);
  iss3 >> refName;
  getline(fin,line); istringstream iss4(line);
  iss4 >> refNumData;
  refWL = new G4double[refNumData];

  //For the specular reflector, read real and imaginary parts of refractive
  //index. For the diffuse reflector, read reflectivity.

  if (refFlag!=0) {
    refReIndex = new G4double[refNumData];
    refImIndex = new G4double[refNumData];
    for (G4int i=refNumData-1; i>-1; i--)
      fin >> refWL[i] >> refReIndex[i] >> refImIndex[i];
  }
  else {
    refRefl = new G4double[refNumData];
    for (G4int i=refNumData-1; i>-1; i--)
      fin >> refWL[i] >> refRefl[i];
  }
  
  //Read refractive index of substrate of reflector.
  fin >> subRefrIndex;
  fin.close();

  air_gap *= mm;

  for (G4int i=0; i<refNumData; i++) refWL[i] *= nanometer;

  //Print out parameters of reflector.
    
  G4cout << "DetectorConstruction::DetectorConstruction: input data:"
    << G4endl;
  G4cout << "   gap = " << 0.01/mm << " mm" << G4endl;
  G4cout << "   Reflector: " << refName << ", refFlag = " << refFlag << ", ";
  if (refFlag==0)
    G4cout << "diffuse reflector";
  else
    G4cout << "specular reflector";
  G4cout << "." << G4endl;

  G4cout << "   Reflector data:" << G4endl;
  for (G4int i=refNumData-1; i>-1; i--) {
    G4cout << "   " << refWL[i]/nanometer << " ";
    if (refFlag!=0)
      G4cout  << refReIndex[i] << " " << refImIndex[i];
    else
      G4cout << refRefl[i];
    G4cout << " " << i << G4endl;
  };

  G4cout << "   Substrate refr. index = " << subRefrIndex;
  if (subRefrIndex == 0.)
    G4cout << ", no substrate";
  else
    G4cout << ", substrate layer between crystal and reflector.";
  G4cout << G4endl;



  G4NistManager* man = G4NistManager::Instance();
  //man->SetVerbose(1);

  G4Element* H  = man->FindOrBuildElement("H");
  G4Element* Si = man->FindOrBuildElement("Si");
  G4Element* C  = man->FindOrBuildElement("C");
  G4Element* O  = man->FindOrBuildElement("O");
  G4Element* K  = man->FindOrBuildElement("K");
  G4Element* N  = man->FindOrBuildElement("N");
  G4Element* Cs = man->FindOrBuildElement("Cs");


  G4String name, symbol;
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density; 
  G4int ncomponents,natoms;
  G4int nelements;

  a = 16.00 * g / mole;
    G4Element *elO = new G4Element(name = "Oxygen", symbol = "O", z = 8., a);

  a = 183.84 * g / mole;
    G4Element *elW = new G4Element(name = "Tungsten", symbol = "W", z = 74., a);

  a = 207.20 * g / mole;
    G4Element *elPb = new G4Element(name = "Lead", symbol = "Pb", z = 82., a);

  a = 28.09 * g / mole;
    G4Element *elSi = new G4Element(name = "Silicone", symbol = "Si", z = 14., a);
  
  a = 137.33 * g / mole;
    G4Element *elBa = new G4Element(name = "Barium", symbol = "Ba", z = 56., a);

 /* // Vacuum
  auto Galactic = new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);*/
  auto PbWO4 = new G4Material("PbWO4",density = 8.280 * g / cm3,ncomponents = 3);
  PbWO4->AddElement(elO, natoms = 4);
  PbWO4->AddElement(elW, natoms = 1);
  PbWO4->AddElement(elPb, natoms = 1);

  auto DSBCe = new G4Material(name = "DSBCe", density = 3.8 * g / cm3, ncomponents = 3);
    DSBCe->AddElement(elO, natoms = 5);
    DSBCe->AddElement(elBa, natoms = 1);
    DSBCe->AddElement(elSi, natoms = 2);
    
G4double wlPbWO4[52] = {675.,
              670.,660.,650.,640.,630.,620.,610.,600.,590.,580.,
              570.,560.,550.,540.,530.,520.,510.,500.,490.,480.,
              470.,460.,450.,440.,430.,420.,410.,400.,390.,380.,
              370.,360.,350.,340.,330.,320.,310.,300.,290.,280.,
              270.,260.,250.,240.,230.,220.,210.,200.,190.,180.,
              175.}; 
  for (G4int i=0; i<52; i++) wlPbWO4[i] *= nanometer;

  const G4double hc = 1.239841857E-6*m*eV;
    
  G4double kphotPbWO4[52];
  for (G4int i=0; i<52; i++) kphotPbWO4[i] = hc/wlPbWO4[i];

  G4double abslength[52] = {
    1400.,
    1400.,1400.,1400.,1400.,1400.,1400.,1400.,933.3,933.3,933.3,
    933.3,933.3,933.3,933.3,933.3,933.3,700.0,700.0,622.2,560.0,
    560.0,466.6,350.0,280.0,233.3,175.0,151.3,112.0,71.79,45.52,
    29.62,17.07,10.17,6.026,3.557,2.092,1.227,0.717,0.418,0.243,
    0.140,0.081,0.047,0.027,0.016,0.009,0.005,0.003,0.002,0.001,
    0.000711281};

  for (G4int i=0; i<52; i++) {
    abslength[i] *= cm;
  };

  G4double rindPbWO4[52];
  for (G4int i=0; i<52; i++) {
    rindPbWO4[i] = 2.2;             
  };
  
  G4double wlPbWO4_sc_fast[82] = {
    630.,
    626.,622.,618.,614.,610.,606.,602.,598.,594.,590.,
    586.,582.,578.,574.,570.,566.,562.,558.,554.,550.,
    546.,542.,538.,534.,530.,526.,522.,518.,514.,510.,
    506.,502.,498.,494.,490.,486.,482.,478.,474.,470.,
    466.,462.,458.,454.,450.,446.,442.,438.,434.,430.,
    426.,422.,418.,414.,410.,406.,402.,398.,394.,390.,
    386.,382.,378.,374.,370.,366.,362.,358.,354.,350.,
    346.,342.,338.,334.,330.,326.,322.,318.,314.,310.,
    306.};

    G4double wlPbWO4_sc_slow[82];
  for (G4int i=0; i<82; i++) wlPbWO4_sc_slow[i] =  wlPbWO4_sc_fast[i] + 5.;
  
  for (G4int i=0; i<82; i++) wlPbWO4_sc_fast[i] *= nanometer;
  for (G4int i=0; i<82; i++) wlPbWO4_sc_slow[i] *= nanometer;

  G4double kphotPbWO4_sc_fast[82];
  G4double kphotPbWO4_sc_slow[82];
  for (G4int i=0; i<82; i++) kphotPbWO4_sc_fast[i] = hc/wlPbWO4_sc_fast[i];
  for (G4int i=0; i<82; i++) kphotPbWO4_sc_slow[i] = hc/wlPbWO4_sc_slow[i];

  G4double PbWO4_sc_fast[82] = {
    0.,
    0.019,0.045,0.064,0.058,0.058,0.064,0.070,0.064,0.064,0.064,
    0.070,0.070,0.090,0.077,0.096,0.122,0.109,0.141,0.134,0.154,
    0.186,0.166,0.192,0.205,0.218,0.243,0.256,0.269,0.288,0.320,
    0.358,0.390,0.416,0.429,0.467,0.512,0.544,0.589,0.627,0.640,
    0.704,0.730,0.774,0.794,0.838,0.870,0.909,0.928,0.934,0.986,
    0.979,0.998,0.992,0.986,0.973,0.941,0.902,0.870,0.819,0.787,
    0.730,0.691,0.653,0.589,0.538,0.461,0.410,0.326,0.282,0.224,
    0.173,0.102,0.070,0.051,0.013,0.000,0.000,0.000,0.000,0.000,
    0.000};

  G4double PbWO4_sc_slow[82];
  for (G4int i=0; i<82; i++) PbWO4_sc_slow[i] = PbWO4_sc_fast[i];

  //PbWO4 material properties table.
    
  G4MaterialPropertiesTable *PbWO4MPT = new G4MaterialPropertiesTable();
  
  PbWO4MPT -> AddProperty("RINDEX",kphotPbWO4,rindPbWO4,52);
  PbWO4MPT -> AddProperty("ABSLENGTH",kphotPbWO4,abslength,52);

  PbWO4MPT->AddProperty("FASTCOMPONENT",kphotPbWO4_sc_fast,PbWO4_sc_fast,82);
  PbWO4MPT->AddProperty("SLOWCOMPONENT",kphotPbWO4_sc_slow,PbWO4_sc_slow,82);
  PbWO4MPT->AddConstProperty("SCINTILLATIONYIELD", 40000*0.377/100/MeV);
  PbWO4MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
  PbWO4MPT->AddConstProperty("FASTTIMECONSTANT", 10.*ns);
  PbWO4MPT->AddConstProperty("SLOWTIMECONSTANT", 30.*ns);
  PbWO4MPT->AddConstProperty("YIELDRATIO", 0.077/(0.077+0.3));

  PbWO4->SetMaterialPropertiesTable(PbWO4MPT);

  //bazmapatkichnerr

  // Air
  // 
  G4Material* Air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  Air->AddElement(N, 70.*perCent);
  Air->AddElement(O, 30.*perCent);

  G4double rindAir[52];
  for (G4int i=0; i<52; i++) {
    rindAir[i] = 1.000293;   //Air @ STP
  };
  G4MaterialPropertiesTable *AirMPT = new G4MaterialPropertiesTable();
  AirMPT -> AddProperty("RINDEX",kphotPbWO4,rindAir,52);
  Air -> SetMaterialPropertiesTable(AirMPT);
  // Glass
  //

  density = 2.23*g/cm3;   //Borosilicate glass (wikipedia)
  auto Glass = new G4Material("Glass", density, ncomponents=2);
  Glass->AddElement(Si, 1);
  Glass->AddElement(O,  2);

  G4double rindGlass[52];
  for (G4int i=0; i<52; i++) {
    rindGlass[i] = 1.525;              //average of 1.51-1.54
  };

  G4MaterialPropertiesTable *GlassMPT = new G4MaterialPropertiesTable();
  GlassMPT -> AddProperty("RINDEX",kphotPbWO4,rindGlass,52);
  Glass -> SetMaterialPropertiesTable(GlassMPT);

  // Optical grease BC630 from Bicron
  //
  density = 1.06*g/cm3;
  auto OpticalGlue = new G4Material("Silgard", density, ncomponents=1);
  OpticalGlue->AddElement(Si, 1); //exact composition not known

  G4double rindGlue[52];
  for (G4int i=0; i<52; i++) {
    rindGlue[i] = 1.465;
  };

  G4MaterialPropertiesTable *GlueMPT = new G4MaterialPropertiesTable();
  GlueMPT -> AddProperty("RINDEX",kphotPbWO4,rindGlue,52);
  OpticalGlue -> SetMaterialPropertiesTable(GlueMPT);


  // Optical insulation
  //
  density = 1.5*g/cm3;   //approximately
  auto Polymer = new G4Material("Polymer", density, ncomponents=2);
  Polymer->AddElement(C, 1);
  Polymer->AddElement(H, 1);
  //Mylar, reflector substrate material.
  //
  
  auto Mylar = new G4Material("Mylar", density= 1.40*g/cm3, ncomponents=3);
  Mylar->AddElement(H, natoms=4);
  Mylar->AddElement(C, natoms=5);
  Mylar->AddElement(O, natoms=2);

  if (subRefrIndex != 0.) {
    
    //Mylar refractive index.
    G4double rindMylar[52];
    for (G4int i=0; i<52; i++) {
      rindMylar[i] = subRefrIndex;
    };

    G4MaterialPropertiesTable *MylarMPT = new G4MaterialPropertiesTable();
    MylarMPT -> AddProperty("RINDEX",kphotPbWO4,rindMylar,52);
    Mylar -> SetMaterialPropertiesTable(MylarMPT);
  }

  // Bialcali, the photochathode material
  //

  density = 1*g/cm3;   //Does not matter
  auto Bialcali = new G4Material("Bialcali", density, ncomponents=2);
  Bialcali->AddElement(Cs, 1);
  Bialcali->AddElement(K,  1);


   // G4double RIn = 15 * cm;
   // G4double ROut = 120 *cm;
    //G4double ROutshift = 5 * cm;
   // G4double Thickness = 20 * cm;
    //double SizeZ;
   // G4double PosZ = 0;

    //............... EMCAL Crystals modules ......................
    G4double PWO_Thickness = 50. * cm;
    G4double PWO_Width = 2. * cm;
    G4double PWO_Gap = 0.01 * mm;
   // G4double PWO_InnerR = 15. * cm;
   // G4double PWO_OuterR = 82. * cm;
   // G4double PWO_PosZ = 0;
    G4double tedlar_thick = 0.040*mm;
    G4double mylar_thick = 0.025*mm;
    G4double glue_thick = 0.035*mm;
    air_gap = 0.035*mm;

    G4double PMT_diameter = 1.86*cm;
    G4double PMTWin_thick = 1*mm;

    G4double Cathode_diam = 1.5*cm;
    G4double Cathode_thick = 0.1*mm;
  
    G4double block_x = 2*cm;
    G4double block_y = 2*cm;
    G4double block_z = 20*cm;

    G4double mylar_x = block_x + 2*air_gap + 2*mylar_thick;
    G4double mylar_y = block_y + 2*air_gap+ 2*mylar_thick;
    G4double mylar_z = block_z + 2*air_gap + 2*mylar_thick;

    G4double tedlar_x = mylar_x + 2*tedlar_thick;
    G4double tedlar_y = mylar_y + 2*tedlar_thick;
    G4double tedlar_z = mylar_z + 2*tedlar_thick;

    G4double counter_x = tedlar_x;
    G4double counter_y = tedlar_y;
    G4double counter_z = tedlar_z + 2*glue_thick +  2*PMTWin_thick;

    auto worldSizeXY = 1.5*counter_x*kNofColumns;
    auto worldSizeZ = 1.5*counter_z;

    G4GenericMessenger *Messenger = new G4GenericMessenger(this, "/EMCAL/");
            Messenger->DeclareProperty("pwoThickness", PWO_Thickness, "Thikness (z direction dimention) of PWO crystals ");
            Messenger->DeclareProperty("pwoWidth", PWO_Width, "Width (and higth) of each PWO crystal");
            Messenger->DeclareProperty("pwoGap", PWO_Gap, "Gap between PWO crystals ");



 /*   //............... EMCAL Glass modules ......................
    double Glass_Thickness = 40. * cm;
    double Glass_Width = 4. * cm;
    double Glass_Gap = 0.01 * mm;
    double Glass_InnerR = 15.*cm;
    double Glass_OuterR = 82.*cm;
    double Glass_PosZ = 0.;
    G4GenericMessenger *Messenger;

            // Set geant options
            Messenger = new G4GenericMessenger(this,"/src/");
            Messenger->DeclareProperty("pwoThickness", PWO_Thickness, "Thikness (z direction dimention) of PWO crystals ");
            Messenger->DeclareProperty("pwoWidth", PWO_Width, "Width (and higth) of each PWO crystal");
            Messenger->DeclareProperty("pwoGap", PWO_Gap, "Gap between PWO crystals ");
            Messenger->DeclareProperty("pwoInnerR", PWO_InnerR, "Inner radius or beam hole for PWO block");
            Messenger->DeclareProperty("pwoOuterR", PWO_OuterR, "Outer radius of PWO block");

            Messenger->DeclareProperty("glassThickness", Glass_Thickness, "Thikness (z direction dimention) of Glass modules");
            Messenger->DeclareProperty("glassWidth", Glass_Width, "Width (and higth) of each Glass modules");
            Messenger->DeclareProperty("glassGap", Glass_Gap, "Gap between Glass modules ");
        
*/
    

    
    //................Get Materials.................................

    
    
    

    
    if ( ! PbWO4 || ! DSBCe || ! Air ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  
    //World

    auto worldS = new G4Box("World",worldSizeXY/2,worldSizeXY/2,worldSizeZ/2);
    auto worldLV = new G4LogicalVolume(worldS,Air,"World");
    auto worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0,fCheckOverlaps);

   /* //Calorimeter
    auto CalorS = new G4Tubs("Calorimeter",RIn,ROut,Thickness/2.,0.,360* deg);
    auto CalorLV = new G4LogicalVolume(CalorS,PWO_Material,"CalorimeterLV");
    new G4PVPlacement(0,G4ThreeVector(0,0,PosZ),CalorLV,"Calorimeter",worldLV,false,0);*/

    

//    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, PWO_PosZ),PWO_LV,"anunik",CalorLV, false, fCheckOverlaps);


    //Visualization
    auto worldVisAttr = new G4VisAttributes(G4Color(1.0, 1.0, 1.0));
    worldVisAttr->SetLineWidth(1);
    worldVisAttr->SetForceSolid(false);
    worldLV->SetVisAttributes(worldVisAttr);
   /* 
    auto CalorVisAttr = new G4VisAttributes(G4Color(1.0, 1.0, 0.9));
    CalorVisAttr->SetLineWidth(1);
    CalorVisAttr->SetForceSolid(false);
    CalorLV->SetVisAttributes(CalorVisAttr);

*/
  G4Box* mylar_outer = new G4Box("Mylar_solid",mylar_x/2,mylar_y/2,mylar_z/2);
  G4Box* mylar_inner = new G4Box("Mylar_cavity",mylar_x/2-mylar_thick,mylar_y/2-mylar_thick,mylar_z/2-mylar_thick);
  G4SubtractionSolid* mylar_box = new G4SubtractionSolid("Mylar",mylar_outer, mylar_inner);

  G4Tubs*  mylar_hole = new G4Tubs("mylar_hole", 0., PMT_diameter/2,mylar_thick/2, 0.*deg, 360.*deg);

  G4RotationMatrix rot;
  G4ThreeVector z_trans_mylar_hole(0, 0, mylar_z/2 - mylar_thick/2);
  G4Transform3D trans_mylar_hole(rot, z_trans_mylar_hole);
  G4SubtractionSolid* mylar_holed = new G4SubtractionSolid("Mylar",mylar_box, mylar_hole, trans_mylar_hole);

  //Remove front wall of Mylar
  G4Box* mylar_front = new G4Box("Mylar_fr",mylar_x/2,mylar_y/2,mylar_thick/2);
  G4ThreeVector z_trans_mylar_front(0, 0, -mylar_z/2 + mylar_thick/2);
  G4Transform3D trans_mylar_front(rot, z_trans_mylar_front);

  G4SubtractionSolid* mylar_frame = new G4SubtractionSolid("Mylar_holed",mylar_holed, mylar_front, trans_mylar_front);
  mylar_log=new G4LogicalVolume(mylar_frame,Mylar,"Mylar");

  

  // Optical insulation
  //
  G4Box* tedlar_outer = new G4Box("Tedlar_solid",tedlar_x/2,tedlar_y/2,tedlar_z/2);
  G4Box* tedlar_inner = new G4Box("Tedlar_cavity",tedlar_x/2-tedlar_thick,tedlar_y/2-tedlar_thick,tedlar_z/2-tedlar_thick);
  G4SubtractionSolid* tedlar_box = new G4SubtractionSolid("Tedlar",tedlar_outer, tedlar_inner);

  // Make a hole of PMT size
  G4Tubs*  tedlar_hole = new G4Tubs("tedlar_hole",0., PMT_diameter/2, tedlar_thick/2,0.*deg, 360.*deg);

  G4ThreeVector z_trans_tedlar_hole(0, 0, tedlar_z/2 - tedlar_thick/2);
  G4Transform3D trans_tedlar_hole(rot, z_trans_tedlar_hole);
  G4SubtractionSolid* tedlar_holed = new G4SubtractionSolid("Tedlar_holed",tedlar_box, tedlar_hole, trans_tedlar_hole);

  //Remove front wall of Tedlar
  G4Box* tedlar_front = new G4Box("Tedlar_fr",tedlar_x/2,tedlar_y/2,tedlar_thick/2);
  G4ThreeVector z_trans_tedlar_front(0, 0, -tedlar_z/2 + tedlar_thick/2);
  G4Transform3D trans_tedlar_front(rot, z_trans_tedlar_front);
  G4SubtractionSolid* tedlar_frame = new G4SubtractionSolid("Tedlar",tedlar_holed, tedlar_front, trans_tedlar_front);

  tedlar_log = new G4LogicalVolume(tedlar_frame,Polymer,"Tedlar",0,0,0);
    
  G4Tubs*  PMTWin_tube = new G4Tubs("PMTWindow", 0., PMT_diameter/2, PMTWin_thick/2,0.*deg, 360.*deg);
  PMTWin_right_log = new G4LogicalVolume(PMTWin_tube,Glass, "PMTWindow");

  G4Tubs*  Cathode_tube = new G4Tubs("Cathode", 0., Cathode_diam/2, Cathode_thick/2,0.*deg, 360.*deg);
  Cathode_log = new G4LogicalVolume(Cathode_tube, Bialcali, "Cathode");

  G4Tubs*  glue_tube = new G4Tubs("glue", 0., PMT_diameter/2, glue_thick/2, 0.*deg, 360.*deg);
  glue_log = new G4LogicalVolume(glue_tube,OpticalGlue, "Glue");

  G4Box* counter_box = new G4Box("Counter",counter_x/2,counter_y/2,counter_z/2);
  counter_log = new G4LogicalVolume(counter_box,Air,"Counter",0,0,0);


  new G4PVPlacement(0,G4ThreeVector(),mylar_log,"Mylar_phys",counter_log,false,0);
  new G4PVPlacement(0,G4ThreeVector(),tedlar_log,"Mylar_phys",counter_log,false,0);

  G4double row_x = kNofRows*counter_x;
  G4double row_y = kNofColumns*counter_y;
  G4double column_x = counter_x;
 // G4double column_y = counter_y;

  auto row_s = new G4Box("row",row_x*0.5,row_y*0.5,counter_z*0.5);
  auto row_lv = new G4LogicalVolume(row_s,Air,"ROW_LV");
  new G4PVPlacement(nullptr,G4ThreeVector(),row_lv,"column_ph",worldLV,false,fCheckOverlaps);

  auto column_s = new G4Box("column",column_x*0.5,row_y*0.5,counter_z*0.5);
  auto column_lv = new G4LogicalVolume(column_s,Air,"COLUMN_LV");

  new G4ReplicatedSlice("divizion",column_lv,row_lv,kXAxis,kNofRows,counter_x,PWO_Gap/2,0);  

  new G4ReplicatedSlice("divizia",counter_log,column_lv,kYAxis,kNofColumns,counter_x,PWO_Gap/2,0);
  
  G4double x = 0.;
  G4double y = 0.;
  z = block_z/2 + glue_thick/2;
  new G4PVPlacement(0,G4ThreeVector(x,y,z),glue_log,"Glue",counter_log,false,0);

  z = block_z/2 + glue_thick + PMTWin_thick/2;

  new G4PVPlacement(0,G4ThreeVector(x,y,z),PMTWin_right_log,"PMTWindow",counter_log,false,0);
  new G4PVPlacement(0,G4ThreeVector(x,y,z),Cathode_log,"Cathode",counter_log,false,0);

  auto PMTVisAttr = new G4VisAttributes(G4Color(1.0, 0.0, 0.0));
    PMTVisAttr->SetLineWidth(1);
    PMTVisAttr->SetForceSolid(false);
    PMTWin_right_log->SetVisAttributes(PMTVisAttr);

  auto CatVisAttr = new G4VisAttributes(G4Color(0.0, 1.0, 0.0));
    CatVisAttr->SetLineWidth(1);
    CatVisAttr->SetForceSolid(false);
    Cathode_log->SetVisAttributes(CatVisAttr);
    

  auto PWO_Solid = new G4Box("Crystal",block_x*0.5,block_y*0.5,block_z*0.5);
  fPWO_LV = new G4LogicalVolume(PWO_Solid,PbWO4,"CrystalLV");
  new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), fPWO_LV,"PWO_name", counter_log, false, fCheckOverlaps);


  auto CrystalVisAttr = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
  CrystalVisAttr->SetLineWidth(1);
  CrystalVisAttr->SetForceSolid(false);
  fPWO_LV->SetVisAttributes(CrystalVisAttr);






  G4MaterialPropertiesTable* ReflectorMPT = new G4MaterialPropertiesTable();
  G4OpticalSurface* Reflector = new G4OpticalSurface("Reflector");

  G4double* refKphot;   //Momenta of optical photons in eV units.
  refKphot = new G4double[refNumData];
  for (G4int i=0; i<refNumData; i++) refKphot[i] = hc/refWL[i];

  if (refFlag != 0) {
    //Specular reflector.

    ReflectorMPT->AddProperty("REALRINDEX",refKphot,refReIndex,refNumData);
    ReflectorMPT->AddProperty("IMAGINARYRINDEX",refKphot,refImIndex,refNumData);

    Reflector -> SetType(dielectric_metal);
    Reflector -> SetFinish(polished);
    Reflector -> SetModel(glisur);
  }
  else {
    // Diffuse reflector, PTFE (Teflon).

    ReflectorMPT -> AddProperty("REFLECTIVITY",refKphot,refRefl,refNumData);

    Reflector -> SetType(dielectric_dielectric);
    Reflector -> SetModel(unified);
    Reflector -> SetFinish(groundfrontpainted);   //Purely Lambertian reflection
  }

  Reflector -> SetMaterialPropertiesTable(ReflectorMPT);

  G4cout << "===== ReflectorMPT: ============================" << G4endl;
  ReflectorMPT->DumpTable();
  Reflector->DumpInfo();

  if (subRefrIndex == 0.) {
    // Reflective front surface of Mylar.
    new G4LogicalSkinSurface("Reflector",mylar_log,Reflector);
  }
  else {
    // Reflective back surface of Mylar.
    // Tedlar borders Mylar from back. Making it reflective, makes effectively
    // Mylar back surface reflective.
    new G4LogicalSkinSurface("Reflector",tedlar_log,Reflector);
    G4cout << "   subRefrIndex = " << subRefrIndex
     << ", substarate between crystal and reflector" << G4endl;
  }
  

  //Quantum efficiency of PMT photocathode.
  //

  G4double wlCat[101] = {675.,670.,665.,660.,655.,650.,645.,640.,635.,630.,
       625.,620.,615.,610.,605.,600.,595.,590.,585.,580.,
       575.,570.,565.,560.,555.,550.,545.,540.,535.,530.,
       525.,520.,515.,510.,505.,500.,495.,490.,485.,480.,
       475.,470.,465.,460.,455.,450.,445.,440.,435.,430.,
       425.,420.,415.,410.,405.,400.,395.,390.,385.,380.,
       375.,370.,365.,360.,355.,350.,345.,340.,335.,330.,
       325.,320.,315.,310.,305.,300.,295.,290.,285.,280.,
       275.,270.,265.,260.,255.,250.,245.,240.,235.,230.,
       225.,220.,215.,210.,205.,200.,195.,190.,185.,180.,
       175.};

  for (G4int i=0; i<101; i++) {
    wlCat[i] *= nanometer;
  };

  G4double kphotCat[101];   //Momenta of optical photons in eV units.
  for (G4int i=0; i<101; i++) kphotCat[i] = hc/wlCat[i];

  // Hamamatsu R4125 quantum efficiency (bialcali photocathode, borosilicate
  // window). Taken from the Hamamatsu booklet, p.65.
  G4double effCat[101] = {
    0.0030,0.0035,0.0040,0.0046,0.0052,0.0060,0.0068,0.0077,0.0087,0.0099,
    0.0112,0.0126,0.0141,0.0159,0.0177,0.0198,0.0221,0.0245,0.0272,0.0301,
    0.0332,0.0365,0.0401,0.0440,0.0481,0.0525,0.0572,0.0621,0.0673,0.0728,
    0.0785,0.0846,0.0908,0.0973,0.1041,0.1110,0.1181,0.1255,0.1329,0.1405,
    0.1482,0.1560,0.1638,0.1716,0.1793,0.1870,0.1946,0.2020,0.2092,0.2162,
    0.2229,0.2293,0.2354,0.2411,0.2463,0.2511,0.2554,0.2592,0.2625,0.2651,
    0.2673,0.2688,0.2697,0.2700,0.2688,0.2653,0.2595,0.2517,0.2419,0.2305,
    0.2177,0.2038,0.1891,0.1740,0.1586,0.1434,0.1285,0.1141,0.1004,0.0877,
    0.0758,0.0650,0.0553,0.0466,0.0389,0.0322,0.0264,0.0215,0.0173,0.0138,
    0.0110,0.0086,0.0067,0.0052,0.0040,0.0030,0.0023,0.0017,0.0012,0.0009,
    0.0007};

  G4double reflCat[101];
  for (G4int i = 0; i < 101; i++) {
    reflCat[i] = 0.;
  }

  G4OpticalSurface* surfCat = new G4OpticalSurface("Cathode");

  surfCat -> SetType(dielectric_metal);
  surfCat -> SetFinish(polished);
  surfCat -> SetModel(glisur);

  G4MaterialPropertiesTable* surfCatMPT = new G4MaterialPropertiesTable();
  surfCatMPT -> AddProperty("REFLECTIVITY",kphotCat,reflCat,101);
  surfCatMPT -> AddProperty("EFFICIENCY",kphotCat,effCat,101);

  surfCat -> SetMaterialPropertiesTable(surfCatMPT);

  new G4LogicalSkinSurface("Cathode",Cathode_log,surfCat);





    /*
    static char abname[256];
    PWO_PosZ = Thickness / 2 - PWO_Thickness / 2;

    auto PWO_Solid = new G4Box("Crystal",PWO_Width * 0.5 , PWO_Width * 0.5, PWO_Thickness * 0.5);
    auto PWO_LV = new G4LogicalVolume(PWO_Solid,PWO_Material,"CrystalLV");

    //Visualization
    
    auto CrystalVisAttr = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
    CrystalVisAttr->SetLineWidth(1);
    CrystalVisAttr->SetForceSolid(false);
    PWO_LV->SetVisAttributes(CrystalVisAttr);
    
    //Crystals
    double y_C = 0;
    double x_C;
    G4int k = -1;
    
    for (int j = 0; j < 50; j++) {
        y_C = j==0 ? PWO_Gap : y_C - PWO_Width + PWO_Gap;
        x_C = (PWO_Width + PWO_Gap) * 0.5;

        for (int i = 0; i < 50 ; i++){
            double R = sqrt(x_C * x_C + y_C * y_C);

            if(R < PWO_OuterR - PWO_Width + PWO_Gap && R > PWO_InnerR){
                
                double x,y;
                k++;
                sprintf(abname, "PW_Ph_%d", k);
                    x=x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, PWO_PosZ),PWO_LV,abname,CalorLV, false, fCheckOverlaps);

                k++;
                sprintf(abname, "PW_Ph_%d", k);
                    x=-x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, PWO_PosZ),PWO_LV,abname,CalorLV, false, fCheckOverlaps);

                k++;
                sprintf(abname, "PW_Ph_%d", k);
                    x=x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, PWO_PosZ),PWO_LV,abname,CalorLV, false, fCheckOverlaps);

                k++;
                sprintf(abname, "PW_Ph_%d", k);
                    x=-x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, PWO_PosZ),PWO_LV,abname,CalorLV, false, fCheckOverlaps);

            }
        }
    }

    static char abname2[256];
    Glass_OuterR = ROut -1*cm;
    Glass_InnerR = PWO_OuterR + 3*cm;
    Glass_PosZ = Thickness/2 - Glass_Thickness/2.;
    
    auto GLASS_S = new G4Box("Glass_solid",Glass_Width * 0.5,Glass_Width * 0.5,Glass_Thickness *0.5);
    auto GLASS_LV = new G4LogicalVolume(GLASS_S,Glass_Material,"Glass_LV");
    
    //Visualization
    auto GlassVisAttr = new G4VisAttributes(G4Color(0.3, 0.4, 1., 0.5));
    GlassVisAttr->SetLineWidth(1);
    GlassVisAttr->SetForceSolid(false);
    GLASS_LV->SetVisAttributes(GlassVisAttr);
    
    y_C = 0;
    x_C = 0.;
    k = -1;

    for (int j = 0; j < 50; j++) {
            if (j == 0) { y_C = Glass_Gap; }
            else { y_C -= Glass_Width + Glass_Gap; }
            x_C = (Glass_Width + Glass_Gap) * 0.5;

        for (int i = 0; i < 50; i++) {
            double R = sqrt(x_C * x_C + y_C * y_C);

         if (R < Glass_OuterR - Glass_Width + Glass_Gap && R > Glass_InnerR) {
                    double x,y;
                    k++;
                    sprintf(abname2, "GLA_Ph_%d", k);
                    x=x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, Glass_PosZ),GLASS_LV,abname2,CalorLV, false, fCheckOverlaps);
                    
                    k++;
                    sprintf(abname2, "GLA_Ph_%d", k);
                    x=x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, Glass_PosZ),GLASS_LV,abname2,CalorLV, false, fCheckOverlaps);

                    k++;
                    sprintf(abname2, "GLA_Ph_%d", k);
                    x=x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, Glass_PosZ),GLASS_LV,abname2,CalorLV, false, fCheckOverlaps);

                    k++;
                    sprintf(abname2, "GLA_Ph_%d", k);
                    x=-x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, Glass_PosZ),GLASS_LV,abname2,CalorLV, false, fCheckOverlaps);
        }
    }
    }
      PWO_PosZ = Thickness / 2 - PWO_Thickness / 2;  

      PWO_Solid = new G4Box("Crystal", PWO_Width * 0.5, PWO_Width * 0.5, PWO_Thickness * 0.5);
    //  PWO_LV = new G4LogicalVolume(PWO_Solid,PWO_Material,"CrystalLV");

      double diameter = 2 * PWO_OuterR;

      int towersInRow = std::ceil((diameter + PWO_Gap) /  (PWO_Width + PWO_Gap));

      double leftTowerPos, topTowerPos;
        if(towersInRow%2) {
            int towersInHalfRow = std::ceil(towersInRow/2.0);
            leftTowerPos = topTowerPos = -towersInHalfRow * (PWO_Width + PWO_Gap);
        }
        else{
            int towersInHalfRow = towersInRow/2;
            leftTowerPos = topTowerPos = -(towersInHalfRow - 0.5) * (PWO_Width + PWO_Gap);

        }
       int towerIndex = 0;
       for(int colIndex=0; colIndex < towersInRow; colIndex++) {
            for(int rowIndex=0; rowIndex < towersInRow; rowIndex++) {
                double x = leftTowerPos + colIndex * (PWO_Width + PWO_Gap);
                double y = topTowerPos + rowIndex * (PWO_Width + PWO_Gap);

                if ((std::abs(y) < PWO_OuterR && std::abs(x) < PWO_OuterR) &&
                    (std::abs(y) > PWO_InnerR || std::abs(x) > PWO_InnerR))
                {
                    int code = 1 * rowIndex + colIndex;
                  //  std::string name(fmt::format("pwo_phys_{}", code));
              
                 //   std::string name = "pwo_ph_" + std::to_string(code);
                    std::stringstream sstm;
                    sstm << "pwo_phys_" << code;
                    std::string name = sstm.str();
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, PWO_PosZ), PWO_LV,name, CalorLV, false, fCheckOverlaps);
                    towerIndex++;
                }
            }
        }


    using namespace std;

    RIn = 15 * cm;
    ROut = 100 *cm;
   // G4double ROutshift = 5 * cm;
    Thickness = 40 * cm;
    //double SizeZ;
    PosZ = 0;

    //............... EMCAL Crystals modules ......................
    PWO_Thickness = 20. * cm;
    PWO_Width = 2. * cm;
    PWO_Gap = 0.01 * mm;
    PWO_InnerR = 15. * cm;
    PWO_OuterR = 82. * cm;
    PWO_PosZ = 0.;

    //............... EMCAL Glass modules ......................
    Glass_Thickness = 40. * cm;
    Glass_Width = 4. * cm;
    Glass_Gap = 0.01 * mm;

    //G4double Glass_InnerR = 30.*cm;
    //G4double Glass_OuterR = 100.*cm;
    //G4double Glass_PosZ = 0.;
    
    Glass_OuterR = ROut - 1*cm ;
    Glass_InnerR = PWO_OuterR + 3*cm;
    Glass_PosZ = Thickness / 2. - Glass_Thickness / 2.;

    GLASS_S = new G4Box("Glass_solid",Glass_Width * 0.5,Glass_Width * 0.5,Glass_Thickness *0.5);
    GLASS_LV = new G4LogicalVolume(GLASS_S,Glass_Material,"Glass_LV");

    diameter = 2 * Glass_OuterR;

    towersInRow = std::ceil((diameter + Glass_Gap) /  (Glass_Width + Glass_Gap));

    if(towersInRow%2) {
            int towersInHalfRow = std::ceil(towersInRow/2.0);
            topTowerPos = leftTowerPos = -towersInHalfRow * (Glass_Width + Glass_Gap);

        } else {
            int towersInHalfRow = towersInRow/2;
            topTowerPos = leftTowerPos = -(towersInHalfRow - 0.5) * (Glass_Width + Glass_Gap);
        }
   // int moduleIndex = 0;

    towerIndex = 0;
        for(int rowIndex=0; rowIndex < towersInRow; rowIndex++) {
            for(NofLayers++;int colIndex=0; colIndex < towersInRow; colIndex++) {
                double x = leftTowerPos + colIndex * (Glass_Width + Glass_Gap);
                double y = topTowerPos + rowIndex * (Glass_Width + Glass_Gap);
                double r = sqrt(x * x + y * y);


                if (r < Glass_OuterR && (std::abs(x) > Glass_InnerR || std::abs(y) > Glass_InnerR)) {
                    int code = 1 * rowIndex + colIndex;

                   // std::string name(fmt::format("glass_phys_{}", code));

                  //  fastformat::write(name, "gla_phys_", code);
                   // std::string name = "gla_phys_" + std::to_string(code);

                    std::stringstream sstm;
                    sstm << "glass_phys_" << code;
                    std::string name = sstm.str();
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, Glass_PosZ), GLASS_LV, name, CalorLV, false, fCheckOverlaps);
                    towerIndex++;
                }
            }
        }
*/

return worldPV;
}
void DetectorConstruction::ConstructSDandField()
{
  auto sdManager = G4SDManager::GetSDMpointer();
  G4String SDname;
 /* auto CrystalSD= new DetectorSD("CrystalSD", "CrystalHitsCollection",NofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(CrystalSD);
  SetSensitiveDetector("CrystalLV",CrystalSD);*/

  auto hadCalorimeter = new DetectorSD(SDname="/Calorimeter");
  sdManager->AddNewDetector(hadCalorimeter);
  PMTWin_right_log->SetSensitiveDetector(hadCalorimeter);
 
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);

  G4AutoDelete::Register(fMagFieldMessenger);
}