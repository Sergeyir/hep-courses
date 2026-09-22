/** 
 *  @file  NDimIntegration.cpp
 *  @brief Contains a simple example that shows how N-dimensional integration can be performed with the use of ROOT::Math::IntegrationMultiDim
 *
 *  Use ROOT cling to run: root NDimIntegration.cpp
 *
 *  This file is a part of a project hep-courses/jets3 (https://github.com/Sergeyir/hep-courses/jets3).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#include "Math/IntegratorMultiDim.h"
#include "Math/Functor.h"

// non-normalized 4D gaus - function example
double Gaus4D(const double *x)
{
   return exp((-x[0]*x[0] - x[1]*x[1] - x[2]*x[2] - x[3]*x[3])/2.);
}

void NDimIntegration()
{
   // number of dimensions
   constexpr unsigned int nDim = 4;

   // we need to define a functor that we can then pass to integrator since integrator needs not
   // only the function address, but also number of dimensions of the function
   ROOT::Math::Functor function(&Gaus4D, nDim);

   // More info on IntegratorMultiDim: 
   // https://root.cern.ch/doc/master/classROOT_1_1Math_1_1IntegratorMultiDim.html
   // here kVEGAS sets VEGAS algorithm (I recommend using it for your task)
   // https://en.wikipedia.org/wiki/VEGAS_algorithm
   ROOT::Math::IntegratorMultiDim integrator(function,
                                             ROOT::Math::IntegrationMultiDim::kVEGAS,
                                             -1., -1., 100000);
   // in the constructor above the restrain on integration errors is removed by setting number
   // of function calls to 100000. For your work it is recommended to do the same (though you may change
   // the number of function calls if needed) since reducing errors may take a long time

   // integration limits for each dimension
   double lowLim[nDim] = {-1.0, -1.0, -1.0, -1.0};
   double uppLim[nDim] = {1.0, 1.0, 1.0, 1.0};

   double result = integrator.Integral(lowLim, uppLim);

   std::cout << "Integral = " << result << std::endl;
   std::cout << "Error    = " << integrator.Error() << std::endl;
}
