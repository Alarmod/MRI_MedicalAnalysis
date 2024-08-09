cmake -S . -B build -DCMAKE_GENERATOR_PLATFORM=x64 -DONNX_DIR=D:/libs/onnxruntime-win-x64-gpu-1.18.0 -DOpenCV_DIR=D:/libs/opencv-4_9_0/build -DOpenCV_RUNTIME=vc16
cmake --build build --config Release
cmake --install build --prefix ./
cmake --install build --prefix ../cracks_segmentator
cmake --install build --prefix ../../ExtractYoloObjectsFromDICOM
