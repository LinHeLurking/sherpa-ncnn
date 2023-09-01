# How to Build

1. Download EMSDK
2. Activate EMSDK 

```
# cd into where you extract emsdk 
./emsdk install latest
./emsdk activate latest
. ./emsdk_env.sh
```

3. Copy Assets

Download models from [here](https://k2-fsa.github.io/sherpa/ncnn/pretrained_models/zipformer-transucer-models.html#marcoyang-sherpa-ncnn-streaming-zipformer-zh-14m-2023-02-23-chinese). And extract them into `wasm-examples/assets/`. After extracting, it should look like this:

```
wasm-examples
├── assets
│   ├── decoder_jit_trace-pnnx.ncnn.bin
│   ├── decoder_jit_trace-pnnx.ncnn.param
│   ├── encoder_jit_trace-pnnx.ncnn.bin
│   ├── encoder_jit_trace-pnnx.ncnn.param
│   ├── export-for-ncnn-bilingual.sh
│   ├── joiner_jit_trace-pnnx.ncnn.bin
│   ├── joiner_jit_trace-pnnx.ncnn.param
│   ├── README.md
│   ├── test_wavs
│   │   ├── 0.wav
│   │   ├── 1.wav
│   │   ├── 2.wav
│   │   ├── 3.wav
│   │   ├── 4.wav
│   │   └── 5.wav
│   └── tokens.txt
├── CMakeLists.txt
├── index.html
├── README.md
├── serve.py
└── wasm.cc
```

4. Configure CMake 

```
# cd back into src top
mkdir build && cd build # if not created 
cmake -DSHERPA_NCNN_ENABLE_WASM=ON -DCMAKE_TOOLCHAIN_FILE=$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DWASM_FEATURE=simd-threads ..
```

5. Build 

```
make -j8
```

After building, all components will be copied into `<build-dir>/wasm-examples/dist/`. You can use the simple python script we provided to verify results:

```
# Assume now you are in build directory
./wasm-examples/dist/serve.py --directory ./wasm-examples/dist
```

open [127.0.0.1:8000](http://127.0.0.1:8000) to check results.

