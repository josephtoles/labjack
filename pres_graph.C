#include<unistd.h>
void pres_graph() {
TCanvas *c1 = new TCanvas("c1", "Pressure", 10, 610, 700, 500);
c1->SetFillColor(42);
c1->SetGrid();
const Int_t n = 0;
Double_t x[0], y[0];
gr = new TGraph(n, x, y);
gr->SetLineColor(2);
gr->SetLineWidth(4);
gr->SetMarkerColor(4);
gr->SetMarkerStyle(21);
gr->SetTitle("Graph of temperature");
gr->GetXaxis()->SetTitle("Time");
gr->GetYaxis()->SetTitle("Pressure");
gr->Draw("ACP");
c1->Update();
c1->GetFrame()->SetFillColor(21);
c1->GetFrame()->SetBorderSize(12);
c1->Modified();
sleep(7);
exit();
}
