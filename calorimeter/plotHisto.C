{
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  
  TFile f("Calor.root");

  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);

  c1->Divide(1,1);

  c1->cd(1);
  TH1D* hist1 = (TH1D*)f.Get("ECryst");
  hist1->Draw("HIST");

  c1->cd(2);
  TH1D* hist2 = (TH1D*)f.Get("LCryst");
  hist2->Draw("HIST");