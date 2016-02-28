#include <string.h>
#include <Rinternals.h>
#include "enc/encode.h"
#include "dec/decode.h"

using namespace brotli;

/* export to C */
extern "C" SEXP R_brotli_compress(SEXP buf, SEXP mode, SEXP quality, SEXP log_win, SEXP log_block);
extern "C" SEXP R_brotli_decompress(SEXP buf);

/* C++ function */
SEXP R_brotli_compress(SEXP buf, SEXP mode, SEXP quality, SEXP log_win, SEXP log_block){

  /* input buffer */
  size_t length = LENGTH(buf);
  uint8_t *input = RAW(buf);

  /* compression options */
  BrotliParams params;
  params.mode = (BrotliParams::Mode) asInteger(mode);
  params.quality = asInteger(quality);
  params.lgwin = asInteger(log_win);
  params.lgblock = asInteger(log_block);

  /* setup output */
  size_t output_length = 1.2 * length + 10240;
  uint8_t *output = new uint8_t[output_length];
  if(!BrotliCompressBuffer(params, length, input, &output_length, output))
    Rf_error("BrotliCompress failed");

  /* create r object */
  SEXP res = PROTECT(allocVector(RAWSXP, output_length));
  memcpy(RAW(res), output, output_length);
  delete[] output;
  UNPROTECT(1);
  return res;
}

SEXP R_brotli_decompress(SEXP buf){

  /* input buffer */
  size_t length = LENGTH(buf);
  uint8_t *input = RAW(buf);

  /* calculate output length */
  size_t outlen;
  if(!BrotliDecompressedSize(length, input, &outlen))
    Rf_error("Failed to calculate output size");

  /* allocate and execute */
  SEXP out = allocVector(RAWSXP, outlen);
  BrotliResult res = BrotliDecompressBuffer(length, input, &outlen, RAW(out));
  if(res != BROTLI_RESULT_SUCCESS)
    Rf_error("Failed to decompress buffer");
  return out;
}
