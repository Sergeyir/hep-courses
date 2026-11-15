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

#include <thread>

#include "TH1.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TColor.h"
#include "Math/MinimizerOptions.h"

void FitHist()
{
   // make ROOT not to draw histogram in new window when Draw method is invoked
   gROOT->SetBatch(kTRUE);

   // do not draw statbox of a histogram
   gStyle->SetOptStat(kFALSE);

   // specify the number of threads to be used for approximations 
   // (the line below sets this number to the number of available threads on the current machine)
   ROOT::EnableImplicitMT(std::thread::hardware_concurrency());

   // by default ROOT uses Minuit (Linear for linear functions) minimizer with Midgrad 
   // minimization algorithm - it is good in general for most approximations, however sometimes 
   // it is iseful to change the minimizer and/or algorythm depending on the 
   // specific approximation task
   // see different minimizers in ROOT::Math::MinimizerOptions::SetDefaultMinimizer
   // https://root.cern.ch/doc/master/classROOT_1_1Math_1_1MinimizerOptions.html#a66f97163f69c2f198e4a48789a63621d
   // the default minimizer that will be used for approximations can be set with as following
   //ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Genetic"); // setting Genetic minimizer
   // in Minuit the following setting is not available 
   // (very useful for high-dimension or large number of parameters)
   //ROOT::Math::MinimizerOptions::SetDefaultMaxIterations(1000);

   // there is a way to make approximation more precise by setting the tolerance 
   // (used only by Minuit and Minuit2, default is 0.01)
   ROOT::Math::MinimizerOptions::SetDefaultTolerance(0.01);
   // however the tolerance is bounded by the maximum number of function calls so that 
   // fitting algorithms are not going to perpetually run trying to decrease the tolerance
   // set the maximum number of tries of approximations
   ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(10000);
   // the decreased maximum tolerance does not guarantee increased approximation accuracy
   // one way to furhter increase accuracy is to switch to fix/bound approximation parameters
   // sometimes trying different minimizers and minimization algorithms also increases accuracy

   // the following strategy might increase the accuracy of the approximation in some cases
   //ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);

   // creating a histogram
   TH1D hist("hist", "", 200, -10., 10.);

   // initializing TF1 method (1-d function) with gaus+exp function
   TF1 fit("fit", "[0]*exp(-(x - [1])^2/(2.*[2]^2)) + [3] + [4]*x + [5]*x^2 + [6]*x^3");
   // where [0], [1], [2]... - approximation parameters
   // more on TF1: https://root.cern.ch/doc/master/classTF1.html
   // alternatively you can set up approximation like this if functions 
   // function you intend to use are already pre-defined
   // TF1 fit("fit", "gaus(0) + pol3(3)");

   // same as above but for exponential function
   // this function will be drawn later to show the background (i.e. exp)
   TF1 fitBG("fit bg", "pol3");

   // setting the range of a function
   // the function will be drawn within this 
   // range when TF1::Draw is called for this method
   // also this range will be used for approximation 
   // if option "R" is specified
   fit.SetRange(-10., 10.);
   fitBG.SetRange(-10., 10.);

   // you can set parameters. The fitting algorithm will start with these which might 
   // greately optimize the approximation. Very useful when you have many parameters
   fit.SetParameters(400, 0., 1., 300., -6., -0.6, 0.05);

   // filling histogram randomly so it would not be empty;
   // in your code you would probably already have a filled histogram by this point
   hist.FillRandom("fit", 200000);

   // you can also limit approximation parameters 
   // (which optimizes the minimization algorithm)
   // very useful when you have many parameters
   // limiting mean within -1. and 1.
   fit.SetParLimits(1, -1., 1.); 

   // additionally you can fix one of the parameters.
   // sometimes this is useful when you know the approximation 
   // parameter value or you want to fix approximation at 
   // some parametric points for systematic studies
   // fit.FixParameter(2, 1.); // fixing sigma to 1

   // setting line with and style of fits and histogram
   // more on lines: https://root.cern.ch/doc/master/classTAttLine.html
   fit.SetLineWidth(3);
   fitBG.SetLineStyle(2);
   fitBG.SetLineWidth(3);
   hist.SetLineWidth(2);

   // setting colors to easily differentiate between drawn objects
   // more on colors: https://root.cern.ch/doc/master/classTColor.html
   hist.SetLineColor(kBlack);
   fit.SetLineColorAlpha(kRed - 3, 0.8); // also adding alpha
   fitBG.SetLineColorAlpha(kGray + 3, 0.5);

   // creating 800x800px canvas to draw on
   // more on TCanvas: https://root.cern.ch/doc/master/classTCanvas.html
   TCanvas canv("canv", "", 800, 800);

   std::cout << "Performing fit " << std::endl;
   // performing approximations
   // - option "R" specifies that the range of the 
   //   TF1 will be used for approximation
   // - option "B" signals ROOT to not ignore limitations and fixes
   //   of approximation parameters
   // - option "M" signals ROOT to perform to use improved algorithm (for Minuit/Minuit2 only)
   // - option "E" signals ROOT to perform better parameter error estimation
   // - option "+" signals ROOT to not remove previously drawn fit 
   //   (in case you want to save multiple fits)
   // more on approximation parameters you can read in TH1::Fit
   // https://root.cern.ch/doc/master/classTH1.html#a7e7d34c91d5ebab4fc9bba3ca47dabdd
   hist.Fit(&fit, "RBME+");
   // if you don't want approximation results in your terminal output add option "Q"

   // setting parameters for the background function
   for (int i = 0; i < fitBG.GetNpar(); i++)
   {
      fitBG.SetParameter(i, fit.GetParameter(3 + i));
   }

   // creating legend (arguments are position on the canvas (xmin, ymin, xmax, ymax; 0 < x,y < 1)
   // more on TLegend: https://root.cern.ch/doc/master/classTLegend.html
   TLegend legend(0.65, 0.6, 0.9, 0.9);
   legend.SetLineColorAlpha(0, 0.);
   legend.SetFillColorAlpha(0, 0.);

   // adding objects to the legend (drawing options passed here tell how to show the histogram in the legend)
   legend.AddEntry(&hist, "histogram", "L");
   legend.AddEntry(&fit, "gaus+background", "L");
   legend.AddEntry(&fitBG, "background", "L");

   // fits are saved into list of approximations for the histogram
   // and are drawn automaticaly when the histogram is drawn
   hist.Draw();

   // drawing background function (exp)
   fitBG.Draw("SAME");

   // drawing the legend
   legend.Draw();

   // using TF1::GetParameter(int), TF1::GetParError(int) to retrieve information about approximation
   std::cout << "Parameters of the fit: \n" <<
                "  gaus scale: " << fit.GetParameter(0) << "±" << fit.GetParError(0) << "\n" <<
                "  gaus mean: " << fit.GetParameter(1) << "±" << fit.GetParError(1) << "\n" <<
                "  gaus sigma: " << fit.GetParameter(2) << "±" << fit.GetParError(2) << std::endl;
   
   std::cout << "Fit Chi2/NDF: " << fit.GetChisquare()/fit.GetNDF() << std::endl;
   std::cout << "Fit P value: " << fit.GetProb() << std::endl;

   std::cout << "Check out the picture this macro produced:" << std::endl;

   canv.SaveAs("Fits.pdf");
}
