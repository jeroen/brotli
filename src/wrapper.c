#include "brotli/encode.h"
#include "brotli/decode.h"

#include <stdlib.h>
#include <string.h>

#define STRICT_R_HEADERS
#define R_NO_REMAP
#include <Rinternals.h>
#include <R_ext/Visibility.h>

attribute_visible SEXP R_brotli_compress(SEXP x, SEXP quality, SEXP lgwin){

  uint32_t qual = INTEGER(quality)[0];
  uint32_t window = INTEGER(lgwin)[0];
  if(qual < BROTLI_MIN_QUALITY || qual > BROTLI_MAX_QUALITY)
    Rf_error("Invalid value for qual:%d", qual);
  if(window < BROTLI_MIN_WINDOW_BITS || window > BROTLI_MAX_WINDOW_BITS)
    Rf_error("Invalid value for lgwin:%d", window);
  BrotliEncoderState* state = BrotliEncoderCreateInstance(0, 0, 0);
  BrotliEncoderSetParameter(state, BROTLI_PARAM_QUALITY, qual);
  BrotliEncoderSetParameter(state, BROTLI_PARAM_LGWIN, window);

  uint8_t * buf = NULL;
  size_t total_out = 0;
  size_t bufsize = 256;
  const uint8_t* next_in = RAW(x);
  size_t available_in = Rf_length(x);
  BROTLI_BOOL success;
  BROTLI_BOOL done;
  do {
    buf = realloc(buf, bufsize);
    size_t available_out = bufsize - total_out;
    uint8_t * next_out = buf + total_out;
    success = BrotliEncoderCompressStream(state, BROTLI_OPERATION_FINISH,
      &available_in, &next_in, &available_out, &next_out, &total_out);
    done = BrotliEncoderIsFinished(state);
    bufsize = 2 * bufsize;
    //Rprintf("available_in: %9d - available_out: %9d - total_out: %9d\n", available_in, available_out, total_out);
  } while(success && !done);

  /* Check if completed successfully */
  BrotliEncoderDestroyInstance(state);
  if(!success || !done || available_in){
    free(buf);
    Rf_error("Brotli encoding error");
  }

  /* Return output */
  SEXP output = Rf_allocVector(RAWSXP, total_out);
  memcpy(RAW(output), buf, total_out);
  free(buf);
  return output;
}

attribute_visible SEXP R_brotli_decompress(SEXP x){

  /* init input */
  const uint8_t* next_in = RAW(x);
  size_t available_in = Rf_length(x);

  /* init output */
  size_t total_out = 0;
  size_t bufsize = 256;
  BrotliDecoderState* state = BrotliDecoderCreateInstance(NULL, NULL, NULL);
  BrotliDecoderResult res = BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT;
  uint8_t * buf = NULL;
  while(res == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT) {
    buf = realloc(buf, bufsize);
    size_t available_out = bufsize - total_out;
    uint8_t * next_out = buf + total_out;
    res = BrotliDecoderDecompressStream(state,
      &available_in, &next_in, &available_out,
      &next_out, &total_out);
      bufsize = 2 * bufsize;
      //Rprintf("available_in: %9d - available_out: %9d - total_out: %9d\n", available_in, available_out, total_out);
  }
  BrotliDecoderDestroyInstance(state);
  if(res != BROTLI_DECODER_RESULT_SUCCESS){
    free(buf);
    if(res == BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT)
      Rf_error("Botli decode failed: incomplete input");
    if(res == BROTLI_DECODER_RESULT_ERROR)
      Rf_error("Botli decode failed: bad data");
    Rf_error("Botli decode failed: unknown error");
  }

  SEXP output = Rf_allocVector(RAWSXP, total_out);
  memcpy(RAW(output), buf, total_out);
  free(buf);
  return output;
}

static const R_CallMethodDef CallEntries[] = {
  {"R_brotli_compress",   (DL_FUNC) &R_brotli_compress,   3},
  {"R_brotli_decompress", (DL_FUNC) &R_brotli_decompress, 1},
  {NULL, NULL, 0}
};

void R_init_brotli(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}

