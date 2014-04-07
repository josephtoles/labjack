#include<unistd.h>
void temp_graph() {
TCanvas *c1 = new TCanvas("c1", "Temperature", 10, 40, 700, 450);
c1->SetGrid();
TMultiGraph *mg = new TMultiGraph();
const Int_t n1 = 0;
Double_t x1[] = {};
Double_t y1[] = {};
TGraph *gr1 = new TGraph(n1,x1,y1);
gr1->SetMarkerColor(kBlue);
gr1->SetMarkerStyle(21);
gr1->Fit("pol6","q");
mg->Add(gr1);
const Int_t n2 = 0;
Double_t x2[] = {};
Double_t y2[] = {};
TGraph *gr2 = new TGraph(n2,x2,y2);
gr2->SetMarkerColor(kBlue);
gr2->SetMarkerStyle(21);
gr2->Fit("pol6","q");
mg->Add(gr2);
mg->Draw("ap");
c1->Update();
sleep(7);
exit();
}
