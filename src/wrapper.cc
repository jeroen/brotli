#include <string.h>
#include <Rinternals.h>
#include "enc/encode.h"
#include "dec/decode.h"

#define BROTLI_VERSION "0.1.0"
using namespace brotli;

/* export to C */
extern "C" SEXP R_brotli_encode(SEXP buf, SEXP mode, SEXP quality, SEXP log_win, SEXP log_block);
extern "C" SEXP R_brotli_decode(SEXP buf);

/* C++ function */
SEXP R_brotli_encode(SEXP buf, SEXP mode, SEXP quality, SEXP log_win, SEXP log_block){

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


int output_callback(void* data, const uint8_t* buf, size_t count) {
  std::vector<uint8_t> *output = (std::vector<uint8_t> *)data;
  output->insert(output->end(), buf, buf + count);
  return (int)count;
}

SEXP R_brotli_decode(SEXP buf){

  /* input buffer */
  size_t length = LENGTH(buf);
  uint8_t *input = RAW(buf);

  /* dynamic buffer because output size is unknown */
  BrotliMemInput memin;
  BrotliInput in = BrotliInitMemInput(input, length, &memin);

  BrotliOutput out;
  std::vector<uint8_t> output;
  out.cb_ = &output_callback;
  out.data_ = &output;

  if(!BrotliDecompress(in, out))
    Rf_error("BrotliDecompress failed");

  /* output R object */
  size_t output_length = output.size();
  SEXP res = PROTECT(allocVector(RAWSXP, output_length));
  memcpy(RAW(res), output.data(), output_length);
  UNPROTECT(1);
  return res;
}
