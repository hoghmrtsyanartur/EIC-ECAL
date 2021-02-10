# EIC-ECAL

Կրիստալների մատրիցան 3*3-ից 5*5 փոխելու համար պետք է մտնել src/DetectorConstruction.cc 

1. for ցիկլերում 3-ները փոխել 5-երով

2. new G4PVPlacement(nullptr, G4ThreeVector(x-PWO_Width,y-PWO_Width, PWO_PosZ), PWO_LV,name, worldLV, false, fCheckOverlaps); տողը փոխարինել 
   new G4PVPlacement(nullptr, G4ThreeVector(x-2*PWO_Width,y-2*PWO_Width, PWO_PosZ), PWO_LV,name, worldLV, false, fCheckOverlaps); տողով 
   Փունջը մատրիցայի կենտրոնին գցելու համար
