// dear emacs, please treat this as -*- C++ -*-

#include <Rmath.h>

#include "pomp.h" // in R, do 'system.file("include/pomp.h",package="pomp")' to find this header file

// define some macros to make the code easier to read
#define LOG_R       (p[parindex[0]]) // growth rate
#define LOG_K       (p[parindex[1]]) // carrying capacity
#define LOG_SIGMA   (p[parindex[2]]) // process noise level
#define LOG_TAU     (p[parindex[3]]) // measurement noise level

#define Y           (y[obsindex[0]])   // observed population size
#define X           (x[stateindex[0]]) // actual population size
#define XPRIME      (f[stateindex[0]]) // new population size (for skeleton function only)

// normal measurement model density
void gompertz_normal_dmeasure (double *lik, double *y, double *x, double *p, int give_log,
				int *obsindex, int *stateindex, int *parindex, int *covindex,
				int ncovars, double *covars, double t) {
  *lik = dlnorm(Y,log(X),exp(LOG_TAU),give_log);
}

// normal measurement model simulator
void gompertz_normal_rmeasure (double *y, double *x, double *p, 
				int *obsindex, int *stateindex, int *parindex, int *covindex,
				int ncovars, double *covars, double t) {
  Y = rlnorm(log(X),exp(LOG_TAU));
}

// stochastic Gompertz model with log-normal process noise
void gompertz_simulator (double *x, const double *p, 
			  const int *stateindex, const int *parindex, const int *covindex,
			  int covdim, const double *covar, 
			  double t, double dt)
{
  double S = exp(-exp(LOG_R)*dt);
  double sigma = exp(LOG_SIGMA);
  double logeps = (sigma > 0.0) ? rnorm(0,sigma) : 0.0;
  X = exp((1-S)*LOG_K)*pow(X,S)*exp(logeps); // note X is over-written by this line
}

// the deterministic skeleton
void gompertz_skeleton (double *f, double *x, const double *p, 
			 const int *stateindex, const int *parindex, const int *covindex,
			 int covdim, const double *covar, double t) 
{
  double dt = 1.0;
  double S = exp(-exp(LOG_R)*dt);
  XPRIME = exp((1-S)*LOG_K)*pow(X,S); // X is not over-written in the skeleton function
}
