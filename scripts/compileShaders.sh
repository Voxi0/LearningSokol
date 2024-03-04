# Variables
SOKOL_SHDC_LINUX_BIN="$(pwd)/3rdparty/sokol-tools-bin/bin/linux/sokol-shdc"
SHADER_SRC_DIR="$(pwd)/src/shaders"
SHADER_BUILD_DIR="src/shaders/build"
SHADER_FLAGS="-l glsl330:hlsl5:metal_macos"

# Compile Shaders
${SOKOL_SHDC_LINUX_BIN} -i ${SHADER_SRC_DIR}/testShader.glsl -o ${SHADER_BUILD_DIR}/testShader.h ${SHADER_FLAGS}
