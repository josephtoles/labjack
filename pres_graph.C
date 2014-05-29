#include<unistd.h>
void pres_graph() {
TCanvas *c1 = new TCanvas("c1", "Pressure", 10, 530, 700, 450);
c1->SetGrid();
const Int_t n = 1;
Double_t x[1], y[1];
x[0] = 0.000000;
y[0] = -0.453537;
gr = new TGraph(n, x, y);
gr->SetMarkerColor(1);
gr->SetMarkerStyle(20);
gr->SetTitle("Graph of pressure");
gr->GetXaxis()->SetTitle("Time");
gr->GetYaxis()->SetTitle("Pressure");
gr->Draw("ALP");
c1->Update();
c1->Modified();
sleep(60);
exit();
}
