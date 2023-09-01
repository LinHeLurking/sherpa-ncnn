#include <stdio.h>

#include <algorithm>
#include <chrono>  // NOLINT
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

#include "net.h"  // NOLINT
#include "sherpa-ncnn/csrc/recognizer.h"
#include "sherpa-ncnn/csrc/wave-reader.h"

const char *tokens = "tokens.txt";
const char *encoder_param = "encoder_jit_trace-pnnx.ncnn.param";
const char *encoder_bin = "encoder_jit_trace-pnnx.ncnn.bin";
const char *decoder_param = "decoder_jit_trace-pnnx.ncnn.param";
const char *decoder_bin = "decoder_jit_trace-pnnx.ncnn.bin";
const char *joiner_param = "joiner_jit_trace-pnnx.ncnn.param";
const char *joiner_bin = "joiner_jit_trace-pnnx.ncnn.bin";
const int32_t num_threads = 1;
const char *decode_method = "modified_beam_search";
const char *wav_filename = "test_wavs/0.wav";

sherpa_ncnn::RecognitionResult sherpa_recognize_ncnn_worker() {
  sherpa_ncnn::RecognizerConfig config;
  config.model_config.tokens = tokens;
  config.model_config.encoder_param = encoder_param;
  config.model_config.encoder_bin = encoder_bin;
  config.model_config.decoder_param = decoder_param;
  config.model_config.decoder_bin = decoder_bin;
  config.model_config.joiner_param = joiner_param;
  config.model_config.joiner_bin = joiner_bin;
  config.model_config.encoder_opt.num_threads = num_threads;
  config.model_config.decoder_opt.num_threads = num_threads;
  config.model_config.joiner_opt.num_threads = num_threads;

  float expected_sampling_rate = 16000;
  std::string method = decode_method;
  if (method == "greedy_search" || method == "modified_beam_search") {
    config.decoder_config.method = method;
  }
  std::cout << "decode method: " << config.decoder_config.method << std::endl;
  config.feat_config.sampling_rate = expected_sampling_rate;
  config.feat_config.feature_dim = 80;

  sherpa_ncnn::Recognizer recognizer(config);

  std::cout << "config: " << config.ToString() << "\n";

  bool is_ok = false;
  std::vector<float> samples =
      sherpa_ncnn::ReadWave(wav_filename, expected_sampling_rate, &is_ok);
  if (!is_ok) {
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    exit(-1);
  }

  const float duration = samples.size() / expected_sampling_rate;
  std::cout << "wav filename: " << wav_filename << "\n";
  std::cout << "wav duration (s): " << duration << "\n";

  auto begin = std::chrono::steady_clock::now();
  std::cout << "Started!\n";
  auto stream = recognizer.CreateStream();
  stream->AcceptWaveform(expected_sampling_rate, samples.data(),
                         samples.size());
  std::vector<float> tail_paddings(
      static_cast<int>(0.3 * expected_sampling_rate));
  stream->AcceptWaveform(expected_sampling_rate, tail_paddings.data(),
                         tail_paddings.size());

  while (recognizer.IsReady(stream.get())) {
    recognizer.DecodeStream(stream.get());
  }
  auto result = recognizer.GetResult(stream.get());
  std::cout << "Done!\n";

  std::cout << "Recognition result for " << wav_filename << "\n"
            << result.ToString();
  // std::cout << "stokens: " << std::endl;
  // for (auto &s : result.stokens) {
  //   std::cout << s << std::endl;
  // }

  auto end = std::chrono::steady_clock::now();
  float elapsed_seconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
          .count() /
      1000.;

  fprintf(stdout, "Elapsed seconds: %.3f s\n", elapsed_seconds);
  float rtf = elapsed_seconds / duration;
  fprintf(stdout, "Real time factor (RTF): %.3f / %.3f = %.3f\n",
          elapsed_seconds, duration, rtf);

  return result;
}

sherpa_ncnn::RecognitionResult LAST_RES;

extern "C" {
int32_t sherpa_recognize_ncnn() {
  std::cout << "Starting sherpa ncnn wasm inference" << std::endl;
  LAST_RES = sherpa_recognize_ncnn_worker();
  return 0;
}

const char *sherpa_query_text() { return LAST_RES.text.data(); }
int sherpa_query_n_timestamp() { return LAST_RES.timestamps.size(); }
float *sherpa_query_timestamps() { return LAST_RES.timestamps.data(); }
}
