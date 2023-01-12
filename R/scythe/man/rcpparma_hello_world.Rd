\name{RcppArmadillo-Functions}
\alias{rcpparma_hello_world}
\alias{rcpparma_innerproduct}
\alias{rcpparma_outerproduct}
\alias{rcpparma_bothproducts}
\title{Set of functions in example RcppArmadillo package}
\description{
  These four functions are created when
  \code{RcppArmadillo.package.skeleton()} is invoked to create a
  skeleton packages.
}
\usage{
rcpparma_hello_world()
rcpparma_outerproduct(x)
rcpparma_innerproduct(x)
rcpparma_bothproducts(x)
}
\arguments{
  \item{x}{a numeric vector}
}
\value{
  \code{rcpparma_hello_world()} does not return a value, but displays a
  message to the console.

  \code{rcpparma_outerproduct()} returns a numeric matrix computed as the
  outer (