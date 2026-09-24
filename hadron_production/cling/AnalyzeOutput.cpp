void AnalyzeOutput()
{
   // opening the file
   TFile *file = TFile::Open("output/pp.root");

   // retrieving the histogram; by default TFile::Get return TObject pointer
   // so you need to cast it to the type you have written
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

   // Draw the histogram if needed
   multPT->Draw();
   
   // The example from above is for pp only
   // To do: after performing MC for p+A, A+B collisions, read the N_{ch} multiplicity and pT vs 
   // N_{ch} multiplicity histograms, divide N_{ch} multiplicity histogram into centrality regions.
   // For each centrality region take projection of pT vs N_{ch} multiplicity on pT within the needed 
   // N_{ch} range and calculate the cross section. Use TH2::ProjectionX or TH2::ProjectionY 
   // (depending on which axis you have chosen for N_{ch} and pT)
}
