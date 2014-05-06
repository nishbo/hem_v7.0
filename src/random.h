#ifndef RANDOM_H
#define RANDOM_H


#include <cstdlib>
#include <cmath>

#include "constants.h"

namespace genran{
    double drand();
    double uniform(double min, double max);

    double triangular(double low, double high, double mode);

    double drandn();
    double gauss(double mu, double sigma);
    double normalVariate(double mu, double sigma);
    double gauss(double mu, double sigma, double min, double max);
    double normalVariate(double mu, double sigma, double min, double max);
    double lognormVariate(double mu, double sigma);

    double expoVariate(double lambda);

    double vonmisesVariate(double mu, double kappa);

    double gammaVariate(double alpha, double beta);

    double betaVariate(double alpha, double beta);

    double paretoVariate(double alpha);

    double weibullVariate(double alpha, double beta);

    double poissonProcess(double lambda);
}

#endif // RANDOM_H
