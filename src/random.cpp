#include "random.h"
#include "iostream"

double genran::drand()
{
    return ((double) rand() / (RAND_MAX));
}

double genran::uniform(double min, double max)
{
    if (min > max)
        exit(1203);

    return min + drand() * (max - min);
}


/*
Triangular distribution.

Continuous distribution bounded by given lower and upper limits,
and having a given mode value in-between.

http://en.wikipedia.org/wiki/Triangular_distribution
*/
double genran::triangular(double low, double high, double mode)
{
    double u = drand();
    double c = (mode - low) / (high - low);
    if (u > c){
        u = 1.0 - u;
        c = 1.0 - c;

        // exchange low&high
        low += high;
        high = low - high;
        low -= high;
    }
    return low + (high - low) * sqrt(u * c);
}

/*
Uses Marsaglia's ziggurat method.

References:
   [1] Marsaglia, G. and Tsang, W.W. (1984) "A fast, easily implemented
       method for sampling from decreasing or symmetric unimodal density
       functions", SIAM J. Sci. Statist. Computing, 5:349-359.
   [2] Evans, M., Hastings, N., and Peacock, B. (1993) Statistical
       Distributions, 2nd ed., Wiley, 170pp.
*/
double genran::drandn()
{
    int accuracy = 12;
    int buf = 0.0;
    for (int i=0; i < accuracy; ++i)
        buf += drand();
    return  buf - double(accuracy) / 2.0;
}

double genran::gauss(double mu, double sigma)
{
    return mu + sigma * drandn();
}

double genran::normalVariate(double mu, double sigma)
{
    return gauss(mu, sigma);
}

double genran::gauss(double mu, double sigma, double min, double max)
{
    if (max < min)
        exit(1204);
    if (max == min)
        return min;

    int buf;
    while(1){
        buf = gauss(mu, sigma);
        if(min <= buf <= max)
            break;
    }
    return buf;
}

double genran::normalVariate(double mu, double sigma, double min, double max)
{
    return gauss(mu, sigma, min, max);
}

/*
The mean and variance of a lognormal random variable with parameters MU
and SIGMA are
    M = exp(MU + SIGMA^2/2)
    V = exp(2*MU + SIGMA^2) * (exp(SIGMA^2) - 1)
Therefore, to generate data from a lognormal distribution with mean M and
variance V, use
    MU = log(M^2 / sqrt(V+M^2))
    SIGMA = sqrt(log(V/M^2 + 1))
*/
double genran::lognormVariate(double mu, double sigma)
{
    return exp(gauss(mu, sigma));
}

/*
lambda is 1.0 divided by the desired mean.  It should benonzero. Uses the
inversion method.

References:
    [1]  Devroye, L. (1986) Non-Uniform Random Variate Generation, 
        Springer-Verlag.
*/
double genran::expoVariate(double lambda)
{
    if (lambda == 0.0)
        exit(1205);

    return -1.0 / lambda * log(drand());
}

/*
Circular data distribution.

mu is the mean angle, expressed in radians between 0 and 2*pi, and
kappa is the concentration parameter, which must be greater than or
equal to zero.  If kappa is equal to zero, this distribution reduces
to a uniform random angle over the range 0 to 2*pi.

mu:    mean angle (in radians between 0 and 2*pi)
kappa: concentration parameter kappa (>= 0)
if kappa = 0 generate uniform random angle

Based upon an algorithm published in: Fisher, N.I.,
"Statistical Analysis of Circular Data", Cambridge
University Press, 1993.

Thanks to Magnus Kessler for a correction to the
implementation of step 4.
*/
double genran::vonmisesVariate(double mu, double kappa)
{
    if (kappa <= 1e-6)
        return TWOPI * drand();

    double a = 1.0 + sqrt(1.0 + 4.0 * kappa * kappa);
    double b = (a - sqrt(2.0 * a))/(2.0 * kappa);
    double r = (1.0 + b * b)/(2.0 * b);
    double f;

    while (1){
        double z = cos(PI * drand());
        f = (1.0 + r * z)/(r + z);
        double c = kappa * (r - f);

        double u2 = drand();

        if (u2 < c * (2.0 - c) or u2 <= c * exp(1.0 - c))
            break;
    }

    double theta;
    if (drand() > 0.5)
        theta = fmod(mu, TWOPI) + acos(f);
    else
        theta = fmod(mu, TWOPI) - acos(f);

    return theta;
}

/*
Gamma distribution.  Not the gamma function!

Conditions on the parameters are alpha > 0 and beta > 0.

alpha > 0, beta > 0, mean is alpha*beta, variance is alpha*beta**2.
*/
double genran::gammaVariate(double alpha, double beta)
{
    if (alpha <= 0.0 or beta <= 0.0)
        exit(1206);
    if (alpha > 1.0){
        // Uses R.C.H. Cheng, "The generation of Gamma
        // variables with non-integral shape parameters",
        // Applied Statistics, (1977), 26, No. 1, p71-74

        double ainv = sqrt(2.0 * alpha - 1.0);
        double bbb = alpha - LOG4;
        double ccc = alpha + ainv;

        double u1, u2, v, x, z, r;

        while (1){
            u1 = uniform(1e-6, 1.0 - 1e-6);
            u2 = 1.0 - drand();
            v = log(u1/(1.0 - u1))/ainv;
            x = alpha * exp(v);
            z = u1 * u1 * u2;
            r = bbb + ccc*v - x;
            if (r + SG_MAGICCONST - 4.5*z >= 0.0 or r >= log(z))
                return x * beta;
        }
    } else if (alpha == 1.0){
        return expoVariate(1.0);
    } else { // alpha is between 0 and 1 (exclusive)
        // Uses ALGORITHM GS of Statistical Computing - Kennedy & Gentle

        double x, u, b, p, u1;
        while (1){
            u = drand();
            b = (E + alpha)/E;
            p = b*u;
            if (p <= 1.0)
                x = pow(p, (1.0/alpha));
            else
                x = -log((b - p)/alpha);
            u1 = drand();
            if (p > 1.0){
                if (u1 <= pow(x, (alpha - 1.0)))
                    break;
            } else if (u1 <= exp(-x))
                break;
        }
        return x * beta;
    }
}

/*
"""Beta distribution.

Conditions on the parameters are alpha > 0 and beta > 0.
Returned values range between 0 and 1.

This version due to Janne Sinkkonen, and matches all the std
texts (e.g., Knuth Vol 2 Ed 3 pg 134 "the beta distribution").
*/
double genran::betaVariate(double alpha, double beta)
{
    double y = gammaVariate(alpha, 1.0);
    if (y == 0.0)
        return 0.0;
    else
        return y / (y + gammaVariate(beta, 1.0));
}

/*
Pareto distribution. alpha is the shape parameter.

Jain, pg. 495
*/
double genran::paretoVariate(double alpha)
{
    double u = 1.0 - drand();
    return pow(1.0 / u, 1.0 / alpha);
}

/*
Weibull distribution.

alpha is the scale parameter and beta is the shape parameter.

Jain, pg. 499; bug fix courtesy Bill Arms
*/
double genran::weibullVariate(double alpha, double beta)
{
    double u = 1.0 - drand();
    return alpha * pow(-log(u), 1.0/beta);
}

/*
Generates time of the next Poisson event.

expoVariate accepts 1/mean, and exponential dixtribution needs to be called with
mean=1/freqency.
 */
double genran::poissonProcess(double frequency)
{
    return expoVariate(frequency);
}