#include<unistd.h>
void graph() {
TCanvas *c1 = new TCanvas("c1", "Graph title", 10, 40, 700, 450);
c1->SetGrid();
TMultiGraph *mg = new TMultiGraph();
const Int_t n = 11;
Double_t x0[] = {0,1,2,3,4,5,6,7,8,9,10};
Double_t y0[] = {297.417239,297.482524,297.417239,297.286670,297.351954,297.417239,297.417239,297.417239,297.417239,297.482524,297.482524};
TGraph *gr0 = new TGraph(n,x0,y0);
gr0->SetMarkerColor(2);
gr0->SetMarkerStyle(20);
mg->Add(gr0);
Double_t x1[] = {0,1,2,3,4,5,6,7,8,9,10};
Double_t y1[] = {298.356238,298.494164,298.425201,298.425201,298.287276,298.356238,298.356238,298.287276,298.287276,298.287276,298.287276};
TGraph *gr1 = new TGraph(n,x1,y1);
gr1->SetMarkerColor(3);
gr1->SetMarkerStyle(20);
mg->Add(gr1);
Double_t x2[] = {0,1,2,3,4,5,6,7,8,9,10};
Double_t y2[] = {296.986242,296.853908,296.853908,296.920075,296.920075,296.853908,296.920075,296.986242,296.853908,296.920075,296.920075};
TGraph *gr2 = new TGraph(n,x2,y2);
gr2->SetMarkerColor(4);
gr2->SetMarkerStyle(20);
mg->Add(gr2);
Double_t x3[] = {0,1,2,3,4,5,6,7,8,9,10};
Double_t y3[] = {297.307812,297.377264,297.238360,297.238360,297.307812,297.377264,297.377264,297.377264,297.307812,297.238360,297.238360};
TGraph *gr3 = new TGraph(n,x3,y3);
gr3->SetMarkerColor(28);
gr3->SetMarkerStyle(20);
mg->Add(gr3);
mg->Draw("ALP");
c1->Update();
}
