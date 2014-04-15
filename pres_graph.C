#include<unistd.h>
void pres_graph() {
TCanvas *c1 = new TCanvas("c1", "Pressure", 10, 530, 700, 450);
c1->SetGrid();
const Int_t n = 2;
Double_t x[2], y[2];
x[0] = 0.000000;
y[0] = 14.948339;
x[1] = 1.000000;
y[1] = 14.948339;
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
