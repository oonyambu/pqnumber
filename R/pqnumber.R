#' @useDynLib pqnumber, .registration = TRUE
NULL

#' @importFrom Rcpp evalCpp
NULL

#' Print a pqnumber in a good manner
#' @param x a pqnumber
#'
#' @export
print.pqnumber <- function(x, digits = getOption("digits"), ...) {
  cat(do.call(my_format, x))
}
