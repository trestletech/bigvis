\name{weighted.quantile}
\alias{weighted.quantile}
\title{Compute quantiles of weighted data.}
\usage{
  weighted.quantile(x, w, probs = seq(0, 1, 0.25),
    na.rm = FALSE)
}
\arguments{
  \item{probs}{numeric vector of probabilities between 0
  and 1}

  \item{na.rm}{If \code{TRUE} will automatically remove
  missing values in \code{x} or \code{w}.}

  \item{x}{numeric vector of observations}

  \item{w}{integer vector of weights, representing the
  number of time each \code{x} was observed}
}
\description{
  Compute quantiles of weighted data.
}
\details{
  Currently only implements the type 7 algorithm, as
  described in \code{\link{quantile}}. Based on
  \code{\link{quantile}} written by R-core.
}
\examples{
x <- runif(200)
w <- rpois(200, 5) + 1
weighted.quantile(x, w)
}
\seealso{
  Other weighted.statistics: \code{\link{weighted.ecdf}},
  \code{\link{weighted.sd}}, \code{\link{weighted.var}}
}

