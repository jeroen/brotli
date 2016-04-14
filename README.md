# brotli

##### *A New Format for Lossless and Lossy Image Compression*

[![Build Status](https://travis-ci.org/jeroenooms/brotli.svg?branch=master)](https://travis-ci.org/jeroenooms/brotli)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/jeroenooms/brotli?branch=master&svg=true)](https://ci.appveyor.com/project/jeroenooms/brotli)
[![Coverage Status](https://codecov.io/github/jeroenooms/brotli/coverage.svg?branch=master)](https://codecov.io/github/jeroenooms/brotli?branch=master)
[![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/brotli)](http://cran.r-project.org/package=brotli)
[![CRAN RStudio mirror downloads](http://cranlogs.r-pkg.org/badges/brotli)](http://cran.r-project.org/web/packages/brotli/index.html)
[![Github Stars](https://img.shields.io/github/stars/jeroenooms/brotli.svg?style=social&label=Github)](https://github.com/jeroenooms/brotli)

> A lossless compressed data format that uses a combination of the
  LZ77 algorithm and Huffman coding. Brotli is similar in speed to deflate (gzip)
  but offers more dense compression.

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

The `libbrotli` source code is currently bundled with the package:

```r
install.package("brotli")
```