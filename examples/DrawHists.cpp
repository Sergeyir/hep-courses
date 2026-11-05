/** 
 *  @file  DrawHists.cpp
 *  @brief Contains a simple example that shows how to set up and draw multiple histograms on the same canvas
 *
 *  Use ROOT cling to run: root DrawHists.cpp
 *
 *  This file is a part of a repository hep-courses (https://github.com/Sergeyir/hep-courses).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/

#pragma once

#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TColor.h"


// Recommended links: 

void DrawHists()
{
   // make ROOT not to draw histogram in new window when Draw method is invoked
   gROOT->SetBatch(kTRUE);
   // do not draw statbox of a histogram
   gStyle->SetOptStat(kFALSE);

   // creating 1st histogram
   TH1D hist1("hist1", "", 200, -10., 10.);
   // creating 2nd histogram
   TH1D hist2("hist2", "", 200, -10., 10.);

   // setting colors to easily differentiate drawn histograms
   // More on colors: https://root.cern.ch/doc/master/classTColor.html
   hist1.SetLineColor(kRed - 3);
   hist2.SetLineColor(kAzure - 3);

   // setting histograms line width and dashed line style for a 2nd histogram
   // More on lines: https://root.cern.ch/doc/master/classTAttLine.html
   hist1.SetLineWidth(2);
   hist2.SetLineWidth(2);
   hist2.SetLineStyle(2);

   // filling a histogram so it would not be empty
   // in your code you would probably already have 
   // a filled histogram by this point
   hist1.FillRandom("gaus", 10000);
   hist2.FillRandom("gaus", 5000);

   // setting axis titles (you only need to set it up for the histogram you draw first)
   hist1.GetXaxis()->SetTitle("x title [x units]");
   hist1.GetYaxis()->SetTitle("Counts");

   // creating legend (arguments are position on the canvas (xmin, ymin, xmax, ymax; 0 < x,y < 1)
   // More on TLegend: https://root.cern.ch/doc/master/classTLegend.html
   TLegend legend(0.7, 0.7, 0.95, 0.95);

   // setting the semi-transparent bounding box lines and background of the legend
   legend.SetLineColorAlpha(kGray, 0.5);
   legend.SetFillColorAlpha(kWhite, 0.5);
   // it can also be fully transparent if you pass the alpha argument 0 as 
   // it is shown below (first argument doesn't matter in that case):
   //legend.SetFillColorAlpha(0, 0.);
   //legend.SetLineColorAlpha(0, 0.);
   // more on fill: https://root.cern.ch/doc/master/classTAttFill.html

   // Creating 800x800px canvas to draw on
   // more on TCanvas: https://root.cern.ch/doc/master/classTCanvas.html
   TCanvas canv("canv", "", 800, 800);

   // setting canvas margins, i.e. the distance the canvas is positioned away from the edge of the picture
   gPad->SetLeftMargin(0.09);
   gPad->SetRightMargin(0.03);
   gPad->SetTopMargin(0.02);
   gPad->SetBottomMargin(0.08);

   // setting title offsets, i.e. the distance the axis title is positioned away from the axis
   hist1.GetXaxis()->SetTitleOffset(1.);
   hist1.GetYaxis()->SetTitleOffset(1.4);

   // drawing histogram axis only
   hist1.Draw("AXIS");
   // drawing axis ticks on the right (X+) and on top (Y+)
   hist1.Draw("SAME AXIS X+ Y+");

   // drawing the contents of the first histogram
   hist1.Draw("SAME");
   // drawing the contents of the second histogram
   hist2.Draw("SAME");

   // adding histogram to the legend (drawing options passed here tell how to show the histogram in the legend)
   legend.AddEntry(&hist1, "gaus 1", "L");
   legend.AddEntry(&hist2, "gaus 2", "L");

   // drawing the legend
   legend.Draw();

   std::cout << "Check out the picture this macro produced:" << std::endl;
   canv.SaveAs("Hists.pdf");
}
