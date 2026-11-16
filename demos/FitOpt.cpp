/** 
 *  @file   FitOpt.cpp 
 *  @brief  Contains simplest GUI of chi2/ndf fit optimization in parametric space demo using ROOT TExec
 *
 *  This file is a part of a project hep-courses (https://github.com/Sergeyir/hep-courses).
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#pragma once

void Draw(const bool firstDraw = false);
void Exec();
double GetChi2(TGraphErrors* g, TF1 *f);

// new for the following objects is required since TExec is called outside of the 
// current scope and ROOT garbage collector migth delete some of its objects by then
TCanvas *canv = new TCanvas("canv", "", 1600, 800);
// graph for showing current parametric point
TGraph *grCurrentParPoint = new TGraph();
// graph for fit
TGraphErrors *gr = new TGraphErrors();
// histogram in parametric space displaying chi2 values
TH2D *chi2Hist = new TH2D("chi2", "#chi^{2}", 200, -10., 10., 200, -10., 10.); 
// fit of gr (displayed for current parametric point)
TF1 *fit = new TF1("fit", "pol1");

void FitOpt()
{
	gStyle->SetOptStat(0);
   gErrorIgnoreLevel = kWarning;

   gROOT->SetBatch(true);

   grCurrentParPoint->SetMarkerStyle(106);
   grCurrentParPoint->SetMarkerSize(3);
   grCurrentParPoint->SetMarkerColorAlpha(kRed, 0.8);

   TRandom rnd;

   rnd.SetSeed(1);

   int nPoints = 20;
   for (int i = 0; i < nPoints; i++)
   {
      const double val = static_cast<double>(i)/static_cast<double>(nPoints)*5.;
      gr->AddPoint(val, val + rnd.Uniform(-0.5, 0.5));
      gr->SetPointError(i, 0., rnd.Uniform(0.25, 1.));
   }

   gr->SetPointError(4, 0., 0.5);

   gr->SetLineColor(kBlack);
   gr->SetLineWidth(2);
   gr->SetMarkerStyle(20);
   gr->SetMarkerSize(1.5);
   gr->SetMarkerColor(kBlack);

   fit->SetLineWidth(2);
   fit->SetLineColor(kRed - 3);
   fit->SetLineStyle(2);

   fit->SetRange(0., 10.);

   canv->Divide(2);

   chi2Hist->GetXaxis()->SetTitle("p_{0}");
   chi2Hist->GetYaxis()->SetTitle("p_{1}");

   for (int i = 1; i <= chi2Hist->GetXaxis()->GetNbins(); i++)
   {
      for (int j = 1; j <= chi2Hist->GetYaxis()->GetNbins(); j++)
      {
         fit->SetParameters(chi2Hist->GetXaxis()->GetBinCenter(i),
                            chi2Hist->GetYaxis()->GetBinCenter(j));
         
         chi2Hist->SetBinContent(i, j, GetChi2(gr, fit));
      }
   }

   gr->SetEditable(false);

   fit->SetParameters(0., 0.);

   grCurrentParPoint->AddPoint(0., 0.);

   Draw(true);

   gPad->AddExec("exec", "Exec()");
}

void Draw(const bool firstDraw)
{
   canv->cd(1);

   if (firstDraw)
   {
      gPad->DrawFrame(0., 0., 5., 5.);
   }

   gr->Draw("SAME P");
   fit->Draw("SAME");

   gPad->Modified();
   gPad->Update();

   canv->cd(2);

   if (firstDraw)
   {
      gPad->SetLogz();
      chi2Hist->Draw("COLZ");
   }

   grCurrentParPoint->Draw("SAME P");

   std::cout << std::endl << "Chi2: " << GetChi2(gr, fit);
}

void Exec()
{
	const int event = gPad->GetEvent();
   const int px = gPad->GetEventX();
   const int py = gPad->GetEventY();

	const double x = gPad->PadtoY(gPad->AbsPixeltoX(px));
	const double y = gPad->PadtoY(gPad->AbsPixeltoY(py));

   switch (event)
   {
      case kButton1Down:
      {
         fit->SetParameters(x, y);
         grCurrentParPoint->SetPoint(0, x, y);
         Draw();
         break;
      }
   }
}

double GetChi2(TGraphErrors *g, TF1 *f)
{
   double chi2 = 0.;
   for (int k = 0; k < g->GetN(); k++)
   {
      chi2 += pow((g->GetPointY(k) - f->Eval(g->GetPointX(k)))/g->GetErrorY(k), 2);
   }
   return chi2;
}
