#include "enc/encode.h"
#include "dec/decode.h"

#include <stdlib.h>
#include <string.h>

#define STRICT_R_HEADERS
#define R_NO_REMAP
#include <Rinternals.h>

SEXP R_brotli_compress(SEXP x, SEXP quality, SEXP lgwin){

  BrotliEncoderState* state = BrotliEncoderCreateInstance(0, 0, 0);
  BrotliEncoderSetParameter(state, BROTLI_PARAM_QUALITY, INTEGER(quality)[0]);
  BrotliEncoderSetParameter(state, BROTLI_PARAM_LGWIN, INTEGER(lgwin)[0]);

  uint8_t * buf = NULL;
  size_t total_out = 0;
  size_t bufsize = 65536;
  const uint8_t* next_in = RAW(x);
  size_t available_in = Rf_length(x);
  BROTLI_BOOL success;
  BROTLI_BOOL done;
  do {
    bufsize = 2 * bufsize;
    buf = realloc(buf, bufsize);
    size_t available_out = bufsize - total_out;
    uint8_t * next_out = buf + total_out;
    success = BrotliEncoderCompressStream(state, BROTLI_OPERATION_FINISH,
      &available_in, &next_in, &available_out, &next_out, &total_out);
    done = BrotliEncoderIsFinished(state);
    //Rprintf("available_in: %9d - available_out: %9d - total_out: %9d\n", available_in, available_out, total_out);
  } while(success && available_in && !done);

  /* Check if completed successfully */
  BrotliEncoderDestroyInstance(state);
  if(!success || !done || available_in){
    free(buf);
    Rf_error("Encoding error");
  }

  /* Return output */
  SEXP output = Rf_allocVector(RAWSXP, total_out);
  memcpy(RAW(output), buf, total_out);
  free(buf);
  return output;
}

SEXP R_brotli_decompress(SEXP x){

  /* init input */
  const uint8_t* next_in = RAW(x);
  size_t available_in = Rf_length(x);

  /* init output */
  size_t total_out = 0;
  size_t bufsize = 65536;
  BrotliState* state = BrotliCreateState(NULL, NULL, NULL);
  BrotliResult res = BROTLI_RESULT_NEEDS_MORE_OUTPUT;
  uint8_t * buf = NULL;
  while(res == BROTLI_RESULT_NEEDS_MORE_OUTPUT && available_in > 0) {
    bufsize = 2 * bufsize;
    buf = realloc(buf, bufsize);
    size_t available_out = bufsize - total_out;
    uint8_t * next_out = buf + total_out;
    res = BrotliDecompressStream(
      &available_in, &next_in, &available_out,
      &next_out, &total_out, state);
    //Rprintf("available_in: %9d - available_out: %9d - total_out: %9d\n", available_in, available_out, total_out);
  }
  BrotliDestroyState(state);
  if(res != BROTLI_RESULT_SUCCESS){
    free(buf);
    if(res == BROTLI_RESULT_NEEDS_MORE_INPUT || res == BROTLI_RESULT_NEEDS_MORE_OUTPUT)
      Rf_error("Botli decode failed: incomplete input");
    if(res == BROTLI_RESULT_ERROR)
      Rf_error("Botli decode failed: bad data");
    Rf_error("Botli decode failed: unknown error");
  }

  SEXP output = Rf_allocVector(RAWSXP, total_out);
  memcpy(RAW(output), buf, total_out);
  free(buf);
  return output;
}
