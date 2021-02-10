# EIC-ECAL

Կրիստալների մատրիցան 3X3-ից 5X5 փոխելու համար պետք է մտնել src/DetectorConstruction.cc 

1. for ցիկլերում 3-ները փոխել 5-երով

2. new G4PVPlacement(nullptr, G4ThreeVector(x-PWO_Width,y-PWO_Width, PWO_PosZ), PWO_LV,name, worldLV, false, fCheckOverlaps); տողում PWO_Width փոփոխականները բազմապատկել 2-ով փունջը մատրիցայի կենտրոնին գցելու համար։
