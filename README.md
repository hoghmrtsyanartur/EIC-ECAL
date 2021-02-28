# EIC-ECAL

Կրիստալների մատրիցան 3X3-ից 5X5 փոխելու համար պետք է 
include/Connstants․hh-ում kNofRows,kNofColumns փոփոխականները հավասարեցնել 5-ի։


run1.mac ֆայլը աշխատեցնելու համար հավաքել  ․/calorimeter -m run1.mac

մի քանի thread միանգամից աշխատեցնելու համար պետք է -DGEANT4_BUILD_MULTITHREADED=ON միացնել
run անելիս հավաքել ․/calorimeter -t thread-ների քանակ 

Օպտիկական ֆոտոնների փոխարեն այլ մասնիկներ գրանցելու համար պետք է
DetectorSD.cc-ում  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition())-տողում փոխել G4Electron::Electron կամ G4Proton::Proton և root ֆայլում օպտիկական ֆոտոնների անվան տակ կգրանցվեն էլեկտրոն,գամմա․․․
