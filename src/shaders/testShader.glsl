@ctype mat4 glm::mat4

@vs vs
    // Vertex Attributes
    in vec3 vertexPosition;
    in vec2 textureCoords;

    // Uniform Variables
    uniform testVSUniforms {
        mat4 pvm;
    };

    // Output
    out vec2 texCoords;

    // Main
    void main() {
        // Set Final Vertex Position
        gl_Position = pvm * vec4(vertexPosition, 1.0f);

        // Send Texture Coordinates to Fragment Shader
        texCoords = textureCoords;
    }
@end

@fs fs
    // Input
    in vec2 texCoords;

    // Uniform Variables

    // Test Texture
    uniform texture2D testTexImg;
    uniform sampler testTexSmp;

    // Output
    out vec4 fragColor;

    // Main
    void main() {
        // Set Final Fragment Color
        fragColor = texture(sampler2D(testTexImg, testTexSmp), texCoords);
    }
@end

@program testShader vs fs
