# brotli

##### *A New Format for Lossless and Lossy Image Compression*

[![Build Status](https://travis-ci.org/jeroen/brotli.svg?branch=master)](https://travis-ci.org/jeroen/brotli)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/jeroen/brotli?branch=master&svg=true)](https://ci.appveyor.com/project/jeroen/brotli)
[![Coverage Status](https://codecov.io/github/jeroen/brotli/coverage.svg?branch=master)](https://codecov.io/github/jeroen/brotli?branch=master)
[![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/brotli)](http://cran.r-project.org/package=brotli)
[![CRAN RStudio mirror downloads](http://cranlogs.r-pkg.org/badges/brotli)](https://cran.r-project.org/package=brotli)
[![Github Stars](https://img.shields.io/github/stars/jeroen/brotli.svg?style=social&label=Github)](https://github.com/jeroen/brotli)

> A lossless compressed data format that uses a combination of the
  LZ77 algorithm and Huffman coding. Brotli is similar in speed to deflate (gzip)
  but offers more dense compression.

## Documentation

About the R package:

 - Vignette: [Text Compression in R: brotli, gzip, xz and bz2](https://cran.r-project.org/web/packages/brotli/vignettes/benchmarks.html)

Other resources:

 - [RFC 7932: Brotli Compressed Data Format](https://tools.ietf.org/html/rfc7932)
 - [Comparison of Brotli, Deflate, Zopfli, LZMA, LZHAM and Bzip2 Compression Algorithms](https://cran.r-project.org/web/packages/brotli/vignettes/brotli-2015-09-22.pdf)

## Hello World

```r
# Simple example
myfile <- file.path(R.home(), "COPYING")
x <- readBin(myfile, raw(), file.info(myfile)$size)
y <- brotli_compress(x)
stopifnot(identical(x, brotli_decompress(y)))

# Compare to other algorithms
length(x)
length(brotli_compress(x))
length(memCompress(x, "gzip"))
length(memCompress(x, "bzip2"))
length(memCompress(x, "xz"))

```

## Installation

The `libbrotli` source code is bundled with the package:

```r
install.package("brotli")
```
