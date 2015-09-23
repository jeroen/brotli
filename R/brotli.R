#' Brotli Compression
#'
#' Compress and decompress binary data with \href{https://github.com/google/brotli}{brotli}. It
#' is supposed to perform better than other algorithms, but doesn't seem to work very well :)
#'
#' @export
#' @seealso memCompress
#' @useDynLib brotli R_brotli_encode
#' @name brotli
#' @rdname brotli
#' @examples # Simple example
#' x <- serialize(iris, NULL)
#' y <- brotli_compress(x)
#' stopifnot(identical(x, brotli_decompress(y)))
#'
#' # Compare to other algorithms
#' length(x)
#' length(brotli_compress(x))
#' length(brotli_compress(x, mode = "font"))
#' length(memCompress(x, "gzip"))
#' length(memCompress(x, "bzip2"))
#' length(memCompress(x, "xz"))
brotli_compress <- function(from, mode = c("generic", "text", "font"), quality = 11, log_win = 22, log_block = 0){
  stopifnot(is.raw(from));
  mode <- switch(match.arg(mode),
    generic = 0L,
    text = 1L,
    font = 2L,
    stop("Invalid mode")
  )
  stopifnot(is.numeric(quality))
  stopifnot(is.numeric(log_win))
  stopifnot(is.numeric(log_block))
  .Call(R_brotli_encode, from, mode, quality, log_win, log_block)
}

#' @export
#' @useDynLib brotli R_brotli_decode
#' @rdname brotli
brotli_decompress <- function(from){
  stopifnot(is.raw(from))
  .Call(R_brotli_decode, from)
}
