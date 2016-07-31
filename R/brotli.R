#' Brotli Compression
#'
#' Brotli is a compression algorithm optimized for the web, in particular small text
#' documents.
#'
#' Brotli decompression is at least as fast as for gzip while significantly
#' improving the compression ratio. The price we pay is that compression is much
#' slower than gzip. Brotli is therefore most effective for serving static content
#' such as fonts and html pages.
#'
#' For binary (non-text) data, the compression ratio of Brotli usually does not beat
#' \code{bz2} or \code{xz (lzma)}, however decompression for these algorithms is too
#' slow for browsers in e.g. mobile devices.
#'
#' @export
#' @seealso \link{memCompress}
#' @useDynLib brotli R_brotli_compress
#' @name brotli
#' @rdname brotli
#' @param buf raw vector with data to compress/decompress
#' @param quality value between 0 and 11
#' @param window log of window size
#' @references J. Alakuijala and Z. Szabadka (July 2016). \emph{Brotli Compressed
#' Data Format}. IETF Internet Draft \url{https://tools.ietf.org/html/rfc7932}.
#' @examples # Simple example
#' myfile <- file.path(R.home(), "COPYING")
#' x <- readBin(myfile, raw(), file.info(myfile)$size)
#' y <- brotli_compress(x)
#' stopifnot(identical(x, brotli_decompress(y)))
#'
#' # Compare to other algorithms
#' length(x)
#' length(brotli_compress(x))
#' length(memCompress(x, "gzip"))
#' length(memCompress(x, "bzip2"))
#' length(memCompress(x, "xz"))
brotli_compress <- function(buf, quality = 11, window = 22){
  stopifnot(is.raw(buf));
  stopifnot(is.numeric(quality))
  stopifnot(is.numeric(window))
  .Call(R_brotli_compress, buf, as.integer(quality), as.integer(window))
}

#' @export
#' @useDynLib brotli R_brotli_decompress
#' @rdname brotli
brotli_decompress <- function(buf){
  stopifnot(is.raw(buf))
  .Call(R_brotli_decompress, buf)
}


# serialize(iris, brofile("~/Desktop/test2.bro", "wb"))
# res <- unserialize(brofile("~/Desktop/test2.bro", "rb"))
brofile <- function(file, open = ""){
  if(grepl("r", open)){
    bro <- system.file("bin/bro", package = "brotli")
    pipe(paste(bro, "--input", file), open = open)
  } else if(grepl("w", open)){
    bro <- system.file("bin/bro", package = "brotli")
    pipe(paste(bro, "--output", file), open = open)
  } else {
    stop("Connection must be opened as readable or writeable")
  }
}
