/** 
 *  @file  FitHists.cpp
 *  @brief Contains a simple example that shows how to set up and approximate histogram with TF1
 *
 *  Use ROOT cling to run: root FitHist.cpp
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

void FitHist()
{
   // make ROOT not to draw histogram in new window when Draw method is invoked
   gROOT->SetBatch(kTRUE);

   // do not draw statbox of a histogram
   gStyle->SetOptStat(kFALSE);

   // the following line will always append "Q" option to fits
   // gStyle->SetOptFit(kFALSE);

   // creating a histogram
   TH1D hist("hist", "", 200, -10., 10.);

   // filling a histogram so it would not be empty
   // in your code you would probably already have 
   // a filled histogram by this point
   hist.FillRandom("gaus", 10000);

   // initializing TF1 method (1-d function) with gaus function
   // more on TF1: https://root.cern.ch/doc/master/classTF1.html
   TF1 fit1("fit1", "gaus");
   // another way to set the function can be achieved as following
   TF1 fit2("fit2", "[0]*exp(-(x - [1])^2/(2.*[2]^2))");
   // where [0], [1], [2] - approximation parameters

   // setting the range of a function
   // the function will be drawn within this 
   // range when TF1::Draw is called for this method
   // also this range will be used for approximation 
   // if option "R" is specified
   fit1.SetRange(-10., 1.);
   fit2.SetRange(-1., 10.);

   // you can set parameters. The fitting algorithm 
   // will start with these which might optimize the approximation
   // very useful when you have many parameters
   fit1.SetParameters(400, 0., 1.);
   fit2.SetParameters(400, 0., 1.);

   // you can also limit approximation parameters 
   // (which optimizes the minimization algorithm)
   // very useful when you have many parameters
   // limiting mean within -0.1 and 0.1
   fit1.SetParLimits(1, -0.1, 0.1); 
   // limiting sigma within 0.9 and 1.1
   fit2.SetParLimits(1, 0.9, 1.1); 

   // additionally you can fix one of the parameters.
   // sometimes this is useful when you know the approximation 
   // parameter value or you want to fix approximation at 
   // some parametric points for systematic studies
   fit1.FixParameter(2, 1.); // fixing sigma to 1
   fit2.FixParameter(1, 0.); // fixing mean to 0

   // setting line with and style of fits and histogram
   // More on lines: https://root.cern.ch/doc/master/classTAttLine.html
   fit1.SetLineStyle(2);
   fit1.SetLineWidth(3);
   fit2.SetLineStyle(5);
   fit2.SetLineWidth(3);
   hist.SetLineWidth(2);

   // setting colors to easily differentiate between drawn objects
   // More on colors: https://root.cern.ch/doc/master/classTColor.html
   hist.SetLineColor(kGray + 3);
   fit1.SetLineColorAlpha(kRed - 3, 0.8); // also adding alpha
   fit2.SetLineColorAlpha(kAzure - 3, 0.8);

   // Creating 800x800px canvas to draw on
   // more on TCanvas: https://root.cern.ch/doc/master/classTCanvas.html
   TCanvas canv("canv", "", 800, 800);

   // performing approximations
   // - option "R" specifies that the range of the 
   //   TF1 will be used for approximation
   // - option "Q" signals ROOT to not print info of 
   //   approximation in the terminal
   // - option "B" signals ROOT to not ignore limitations and fixes
   //   of approximation parameters
   // - option "+" signals ROOT to not remove previously drawn fit
   // More on approximation parameters you can read in TH1::Fit
   // description in TH1 class reference: https://root.cern.ch/doc/master/classTH1.html
   hist.Fit(&fit1, "RQB");
   hist.Fit(&fit2, "RQB+");

   // creating legend (arguments are position on the canvas (xmin, ymin, xmax, ymax; 0 < x,y < 1)
   // More on TLegend: https://root.cern.ch/doc/master/classTLegend.html
   TLegend legend(0.7, 0.7, 0.95, 0.95);

   // adding objects to the legend (drawing options passed here tell how to show the histogram in the legend)
   legend.AddEntry(&hist, "gaus", "L");
   legend.AddEntry(&fit1, "1st fit", "L");
   legend.AddEntry(&fit2, "2nd fit", "L");

   // fits are saved into list of approximations for the histogram
   // and are drawn automaticaly when the histogram is drawn
   hist.Draw();

   // drawing the legend
   legend.Draw();

   // to retrieve information about approximation use TF1 methods
   std::cout << "Approximation parameters for 1st fit: \n" <<
                "  scale: " << fit1.GetParameter(0) << "±" <<
                               fit1.GetParError(0) << std::endl <<
                "  mean: " << fit1.GetParameter(1) << "±" <<
                              fit1.GetParError(1) << std::endl <<
                "  sigma: " << fit1.GetParameter(2) << " (fixed)\n" <<
                "Approximation parameters for 2nd fit: \n" <<
                "  scale: " << fit2.GetParameter(0) << "±" <<
                               fit2.GetParError(0) << std::endl <<
                "  mean: " << fit2.GetParameter(1) << " (fixed)\n" <<
                "  sigma: " << fit2.GetParameter(2) << "±" <<
                               fit2.GetParError(2) << std::endl;

   std::cout << "Check out the picture this macro produced:" << std::endl;

   canv.SaveAs("Fits.pdf");
}
