#pragma once

void AnalyzeOutput()
{
   // make ROOT not to draw histogram in new window when Draw method is invoked
   gROOT->SetBatch(kTRUE);
   // do not draw statbox of a histogram
   gStyle->SetOptStat(kFALSE);

   // opening and loading the file
   TFile *file = TFile::Open("output/pp.root");

   // retrieving the histogram; by default TFile::Get return TObject pointer
   // so you need to cast it to the type you have written 
   // (alternatively you can use c-style cast)
   TH1D *multPT = static_cast<TH1D *>(file->Get("pT multiplicity"));

   // To scale all contents of the histogram use
   multPT->Scale(1./(2.*M_PI));

   // Iterating over x axis of a histogram (bin numbering starts at 1 at ends at number of bins)
   for (int i = 1; i <= multPT->GetXaxis()->GetNbins(); i++)
   {
      // retrieving ith bin content of the histogram
      const double binContent = multPT->GetBinContent(i);
      // retrieving ith bin error of the histogram
      const double binError = multPT->GetBinError(i);
      // setting pT as the center of the ith bin
      const double pT = multPT->GetXaxis()->GetBinCenter(i);
      // delta pT can be calculated with TAXIS::GetBinWidth(int)
      // changing the bin content for each bin individually
      multPT->SetBinContent(i, binContent);
   }
   // To do: add scaling by total cross section and divide each bin by the 
   // bin width to obtain the invariant differential cross section

   // Setting the title
   multPT->SetTitle("");
   // More info on tex syntax in ROOT: 
   // https://root.cern.ch/doc/master/classTLatex.html
   // Set the X axis title
   multPT->GetXaxis()->SetTitle("p_{T}");
   // Set the Y axis title
   // To do: change the 
   multPT->GetYaxis()->SetTitle("#frac{d #sigma}{d p_{T}}");

   // You will also need to save the picture in .png and/or .pdf
   // Use ROOT TCanvas to draw on the canvas and write it as a picture
   // https://root.cern.ch/doc/master/classTCanvas.html
   TCanvas canv("canv", "", 800, 800);

   // gPad - current pad (TPad) on the canvas
   // setting log y scale
   gPad->SetLogy();

   // Graphical adjustments to the canvas: setting canvas margins
   // To do: improve these margins if needed
   gPad->SetLeftMargin(0.1);
   gPad->SetRightMargin(0.05);
   gPad->SetTopMargin(0.05);
   gPad->SetBottomMargin(0.1);

   // Graphical adjustments to the histogram axis: setting axis titles offsets
   // To do: improve these offsets if needed
   multPT->GetXaxis()->SetTitleOffset(1.);
   multPT->GetYaxis()->SetTitleOffset(1.2);

   // Drawing the histogram on the current pad
   multPT->Draw();

   // saving canvas as .pdf picture
   std::filesystem::create_directories("pictures");
   canv.SaveAs("pictures/cs_pp.pdf");
   // it can also be saved as .png
   //canv.SaveAs("output/cs_pp.png");
   
   // The example from above is for pp only
   // To do: after performing MC for p+A, A+B collisions, read the N_{ch} multiplicity and pT vs 
   // N_{ch} multiplicity histograms, divide N_{ch} multiplicity histogram into centrality regions.
   // For each centrality region take projection of pT vs N_{ch} multiplicity on pT within the needed 
   // N_{ch} range and calculate the cross section. Use TH2::ProjectionX or TH2::ProjectionY 
   // (depending on which axis you have chosen for N_{ch} and pT)
}
