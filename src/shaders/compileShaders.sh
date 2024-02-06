# Base Variables
PROJECT_DIR=~/Desktop/learningSokol/
SHADERS_DIR=${PROJECT_DIR}/src/shaders/
SOKOL_SHDC_LINUX=${PROJECT_DIR}/3rdparty/sokol-tools-bin/bin/linux/sokol-shdc
SHADER_DIALECTS=glsl330:hlsl5:metal_macos

# Compile Shaders Using Sokol's Shader Cross-Compiler
${SOKOL_SHDC_LINUX} -i ${SHADERS_DIR}/testShaders/testShaders.glsl -o ${SHADERS_DIR}/testShaders/testShaders.h -l ${SHADER_DIALECTS}
