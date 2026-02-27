#' @title pqnumber Package
#' @description Functions for PQnumber computations
#' @importFrom Rcpp evalCpp
#' @useDynLib pqnumber, .registration = TRUE

NULL


#' Print a pqnumber in a good manner
#' @param x a pqnumber
#'
#' @export
print.pqnumber <- function(x, digits = getOption("digits"), ...) {
  cat(do.call(my_format, x))
}
