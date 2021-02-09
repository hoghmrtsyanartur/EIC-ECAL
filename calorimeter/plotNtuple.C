{
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  
  TFile f("Calor.root");

  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);

  c1->Divide(1,1);

  TNtuple* ntuple = (TNtuple*)f.Get("Calorimeter");

  c1->cd(1);
  ntuple->Draw("ECryst");

  c1->cd(2);
  ntuple->Draw("LCryst");