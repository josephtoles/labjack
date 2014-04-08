#include<unistd.h>
void pres_graph() {
TCanvas *c1 = new TCanvas("c1", "Pressure", 10, 530, 700, 450);
c1->SetGrid();
const Int_t n = 4;
Double_t x[4], y[4];
x[0] = 0.000000;
y[0] = 50.327439;
x[1] = 1.000000;
y[1] = 50.377198;
x[2] = 2.000000;
y[2] = 50.377198;
x[3] = 3.000000;
y[3] = 50.476717;
gr = new TGraph(n, x, y);
gr->SetMarkerColor(1);
gr->SetMarkerStyle(20);
gr->SetTitle("Graph of pressure");
gr->GetXaxis()->SetTitle("Time");
gr->GetYaxis()->SetTitle("Pressure");
gr->Draw("ALP");
c1->Update();
c1->Modified();
sleep(10);
exit();
}
