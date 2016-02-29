#include "enc/encode.h"
#include "dec/decode.h"

#define STRICT_R_HEADERS
#define R_NO_REMAP
#include <string.h>
#include <Rinternals.h>

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
  params.mode = (BrotliParams::Mode) Rf_asInteger(mode);
  params.quality = Rf_asInteger(quality);
  params.lgwin = Rf_asInteger(log_win);
  params.lgblock = Rf_asInteger(log_block);

  /* setup output */
  size_t output_length = 1.2 * length + 10240;
  uint8_t *output = new uint8_t[output_length];
  if(!BrotliCompressBuffer(params, length, input, &output_length, output))
    Rf_error("BrotliCompress failed");

  /* create r object */
  SEXP res = PROTECT(Rf_allocVector(RAWSXP, output_length));
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
  SEXP out = Rf_allocVector(RAWSXP, outlen);
  BrotliResult res = BrotliDecompressBuffer(length, input, &outlen, RAW(out));
  if(res != BROTLI_RESULT_SUCCESS)
    Rf_error("Failed to decompress buffer");
  return out;
}
