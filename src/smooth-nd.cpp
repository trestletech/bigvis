#include <algorithm>
#include <Rcpp.h>
#include "group.hpp"
using namespace Rcpp;

double tricube(double x) {
  x = fabs(x);
  if (x > 1) return 0;

  double y = 1 - x * x * x;
  return y * y * y;
}

// [[Rcpp::export]]
NumericVector smooth_nd_1(const NumericMatrix& grid_in, 
                          const NumericVector& z_in, 
                          const NumericVector& w_in_,
                          const NumericMatrix& grid_out, 
                          const int var, const double h) {

  if (var < 0) stop("var < 0");
  if (var >= grid_in.ncol()) stop("var too large");
  if (h <= 0) stop("h <= 0");
  if (grid_in.nrow() != z_in.size()) stop("Incompatible input lengths");
  if (grid_in.ncol() != grid_out.ncol()) stop("Incompatible grid sizes");

  int n_in = grid_in.nrow(), n_out = grid_out.nrow(), d = grid_in.ncol();
  NumericVector w_in = (w_in_.size() > 0) ? w_in_ : 
    rep(NumericVector::create(1), n_in);
  NumericVector z_out(n_out), w_out(n_out);

  // Will be much more efficient to slice up by input dimension:
  // and most efficient way of doing that will be to bin with / bw
  // My data structure: sparse grids
  // 
  // And once we're smoothing in one direction, with guaranteed evenly spaced
  // grid can avoid many kernel evaluations and can also compute more
  // efficient running sum

  for (int i = 0; i < n_in; ++i) {
    for(int j = 0; j < n_out; ++j) {
      // Check that all variables (apart from var) are equal
      bool equiv = true;
      for (int k = 0; k < d; ++k) {
        if (k == var) continue;
        if (grid_in(i, k) != grid_out(j, k)) {
          equiv = false;
          break;
        }
      };
      if (!equiv) continue;

      double dist = (grid_in(i, var) - grid_out(j, var)) / h;
      double k = tricube(dist) * w_in[i];

      w_out[j] += k;
      z_out[j] += z_in[i] * k;
    }
  }

  for(int j = 0; j < n_out; ++j) {
    z_out[j] /= w_out[j];
  }

  return z_out;
}

// [[Rcpp::export]]
NumericVector smooth_nd(const NumericMatrix& grid_in, 
                        const NumericVector& z_in, 
                        const NumericVector& w_in_,
                        const NumericMatrix& grid_out, 
                        const NumericVector h) {

  if (grid_in.nrow() != z_in.size()) stop("Incompatible input lengths");
  if (grid_in.ncol() != grid_out.ncol()) stop("Incompatible grid sizes");
  if (h.size() != grid_in.ncol()) stop("Incorrect h length");

  int n_in = grid_in.nrow(), n_out = grid_out.nrow(), d = grid_in.ncol();
  NumericVector w_in = (w_in_.size() > 0) ? w_in_ : 
    rep(NumericVector::create(1), n_in);
  NumericVector z_out(n_out), w_out(n_out);

  for (int i = 0; i < n_in; ++i) {
    for(int j = 0; j < n_out; ++j) {
      double w = 1;
      for (int k = 0; k < d; ++k) {
        double dist = (grid_in(i, k) - grid_out(j, k)) / h[k];
        w *= tricube(dist);
      }
      w *= w_in[i];

      w_out[j] += w;
      z_out[j] += z_in[i] * w;
    }
  }

  for(int j = 0; j < n_out; ++j) {
    z_out[j] /= w_out[j];
  }

  return z_out;
}
