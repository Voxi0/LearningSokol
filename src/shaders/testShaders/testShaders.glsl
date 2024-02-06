#version 330 core

@vs vs
layout(location = 0) in vec3 vertexPos;
uniform uniformVars {
    mat4 pvm;
};

void main(void) {
    gl_Position = pvm * vec4(vertexPos, 1.0f);
}
@end

@fs fs
out vec4 fragColor;
void main(void) {
    fragColor = vec4(1.0f, 0.7f, 0.3f, 1.0f);
}
@end

@program testShaders vs fs
