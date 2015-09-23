#' Brotli Compression
#'
#' Compress and decompress binary data with Brotli.
#'
#' @export
#' @seealso memCompress
#' @useDynLib brotli R_brotli_encode
#' @name brotli
#' @rdname brotli
brotli_encode <- function(from, mode = c("generic", "text", "font"), quality = 11, log_win = 22, log_block = 0){
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
#' @useDynLib brotli R_brotli_encode
#' @rdname brotli
brotli_decode <- function(from){
  stopifnot(is.raw(from))
  .Call(R_brotli_encode, from)
}
