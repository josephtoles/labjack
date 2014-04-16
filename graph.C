#include<unistd.h>
void graph() {
TCanvas *c1 = new TCanvas("c1", "Graph title", 10, 40, 700, 450);
c1->SetGrid();
TMultiGraph *mg = new TMultiGraph();
const Int_t n = 10;
Double_t x0[] = {0,1,2,3,4,5,6,7,8,9};
Double_t y0[] = {296.331160,296.394165,296.457169,296.331160,296.268155,296.520174,296.394165,296.394165,296.520174,296.520174};
TGraph *gr0 = new TGraph(n,x0,y0);
gr0->SetMarkerColor(2);
gr0->SetMarkerStyle(20);
mg->Add(gr0);
Double_t x1[] = {0,1,2,3,4,5,6,7,8,9};
Double_t y1[] = {296.494228,296.430564,296.430564,296.494228,296.366900,296.366900,296.430564,296.430564,296.366900,296.366900};
TGraph *gr1 = new TGraph(n,x1,y1);
gr1->SetMarkerColor(3);
gr1->SetMarkerStyle(20);
mg->Add(gr1);
Double_t x2[] = {0,1,2,3,4,5,6,7,8,9};
Double_t y2[] = {295.239513,295.239513,295.300042,295.360570,295.360570,295.300042,295.360570,295.239513,295.300042,295.300042};
TGraph *gr2 = new TGraph(n,x2,y2);
gr2->SetMarkerColor(4);
gr2->SetMarkerStyle(20);
mg->Add(gr2);
Double_t x3[] = {0,1,2,3,4,5,6,7,8,9};
Double_t y3[] = {295.238111,295.177340,295.177340,295.238111,295.298881,295.177340,295.238111,295.055799,295.177340,295.177340};
TGraph *gr3 = new TGraph(n,x3,y3);
gr3->SetMarkerColor(28);
gr3->SetMarkerStyle(20);
mg->Add(gr3);
mg->Draw("ALP");
c1->Update();
}
