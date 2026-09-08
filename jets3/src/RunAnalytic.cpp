/** 
 *  @file   RunAnalytic.cpp
 *  @brief  Contains simplest case that shows how the analytic calculation can be performed for the estimation of the cross section
 *
 *  This file is a part of a project hep-courses/jets3 (https://github.com/Sergeyir/hep-courses/jets3).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef RUN_ANALYTIC_CPP
#define RUN_ANALYTIC_CPP

#include "RunAnalytic.hpp"

/// LHDAPDF::PDF object; it is used to obtain \alpha_Q, Q1, and Q2 if initialized for the given pdf set
LHAPDF::PDF *pdf;
/// ROOT TRandom object; it is used to randomly select y1, y2, and p_T for MC integration
TRandom rnd;
/// number of integration steps for MC integration
long numberOfIntegrationSteps;

int main(int argc, char **argv)
{
   // printing info on usage and exiting program if number of parameters is incorrect
   if (argc != 3) 
   {
      std::cout << "\033[1m\033[31mError:\033[0m Expected 2 parameters while " <<
                   std::to_string(argc - 1) << " parameter(s) were provided \n"\
                   "Usage: bin/RunAnalytic inputFileName.yaml numberOfIntegrationSteps" 
                << std::endl;
      std::cout << "[\033[1m\033[32mINFO\033[0m] input file example is located in input directory" 
                << std::endl;
      return 1;
   }

   // checking if that file exists; if it doesn't exist error is printed and the program is stopped
   if (!std::filesystem::exists(argv[1]))
   {
      std::cout << "\033[1m\033[31mError:\033[0m file " << argv[1] << " was not found" << std::endl;
      return 1;
   }

   numberOfIntegrationSteps = std::stol(argv[2]);

   // YAML::Node object reads all data from the .yaml file
   YAML::Node inputFileContents = YAML::LoadFile(argv[1]);

   const std::string pdfSet = inputFileContents["pdfset"].as<std::string>();
   // To do: read and assign pTHatMin, maxinum y, 
   const double pTHatMin = 0.; 
   const double absMaxY = 0.;
   const double sqrtSNN = 0.;

   // setting pdf
   pdf = LHAPDF::mkPDF(pdfSet);
   // setting random seed
   rnd.SetSeed(GetRandomSeed());

   std::filesystem::create_directory("output");
   TFile outputFile("output/analytic.root", "RECREATE");

   // To do: change this histogram to 2D (d\sigma / dp_T dy_1 dy_2)
   TH1D distrDSigmaDPT("dsigmadpT", "d#sigma/dp_{T}", 1000 + pTHatMin, 0., 100. + pTHatMin);

   const int nBinsX = distrDSigmaDPT.GetXaxis()->GetNbins();
   // iterating over $p_T$ bins
   for (int i = 1; i <= nBinsX; i++)
   {
      // the line below prints progress in percents
      std::cout << static_cast<double>(i)/static_cast<double>(nBinsX)*100. << "%\r";

      // randomly choosing y1 and y2
      const double y1 = rnd.Uniform(-absMaxY, absMaxY);
      const double y2 = rnd.Uniform(-absMaxY, absMaxY);

      double err;
      const double dSigmaDPTDY1DY2 = GetDSigmaDPTDY1DY2(distrDSigmaDPT.GetXaxis()->GetBinCenter(i), sqrtSNN, y1, y2, err);

      distrDSigmaDPT.SetBinContent(i, dSigmaDPTDY1DY2);
      distrDSigmaDPT.SetBinError(i, err);
   }

   distrDSigmaDPT.Write();

   std::cout << distrDSigmaDPT.Integral() << std::endl;

   outputFile.Close();

   return 0;
}

double GetDSigmaDOmega(const int id1, const int id2, const double pT, 
                       const double s, const double y)
{
   // To do: define mandelstam variables (t, u) and substitute lower random dummies for them
   // (hint: it will be easier if you define cos(\Theta) first)
   const double t = rnd.Uniform(-s, s);
   const double u = rnd.Uniform(-s, s);
   
   // obtaining alpha_{S} at hard scale, i.e. \mu_F = p_T
   const double alphaS = pdf->alphasQ2(pT*pT);

   // only qq->qq is shown below as an example
   if (id1 != 0 && id2 != 0 && id1 == id2)
   {
      // source: Eq. 51.7 at https://pdg.lbl.gov/2025/reviews/contents_sports.html
      return alphaS*alphaS/(9.*s)*((t*t + s*s)/(u*u) + (s*s + u*u)/(t*t) - 2.*s*s/(3.*u*t));
   }
   // To do: add other d\sigma / d\Omega processes here

   return 0.;
}

double GetX1(const double pT, const double sqrtSNN, const double y1, const double y2)
{
   // To do: fill in the formula for x1 in the current function
   return 0.;
}

double GetX2(const double pT, const double sqrtSNN, const double y1, const double y2)
{
   // To do: fill in the formula for x2 in the current function
   return 0.;
}

double GetDSigmaDPTDY1DY2(const double pT, const double sqrtSNN, 
                          const double y1, const double y2, double &err)
{
   // result of MC integration
   double result = 0.;
   // normalization constant for MC integration: equals to the number of succesfull integration steps
   long normalization = 0;

   for (long i = 0; i < numberOfIntegrationSteps; i++)
   {
      // calculating x1 and x2
      const double x1 = GetX1(pT, sqrtSNN, y1, y2);
      const double x2 = GetX2(pT, sqrtSNN, y1, y2);

      // To do: add a check that tests whether x1 and x2 are within kinematicaly possible range

      // calculating \sqrt{\hat{s}}, i.e. the center of mass energy of 2 partons
      const double s = sqrtSNN*sqrtSNN*x1*x2;

      // result of a sum over all combinations
      double result = 0.;
      
      // Only qq->qq process for u quarks is shown (id = 1). This id can go from -5 to +5 (where id=0 is a gluon)
      // To do: iterate over all possible combinations for the following expression
      // To do : determine measurement units for the following expression
      result += 8.*M_PI*pT*pdf->xfxQ2(1, x1, pT*pT)* pdf->xfxQ2(1, x2, pT*pT)*
                GetDSigmaDOmega(1, 1, pT, s, y1 - y2)/s;
      // To do: add an expression to calculate result for non-identical particles
      // normalization constant is a sum of weights; for uniform distribution all weights are 1
      normalization++;
   }

   if (normalization == 0) 
   {
      err = 0.;
      return 0.;
   }

   result /= static_cast<double>(normalization);
   err = result/sqrt(normalization);

   return result;
}

unsigned int GetRandomSeed()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	auto epoch = now_ms.time_since_epoch();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	return static_cast<unsigned int>(duration.count() % 900000000);
}

#endif /* RUN_ANALYTIC_CPP */
