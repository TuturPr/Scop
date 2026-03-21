#version 330 core
#define PI 3.141592
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 vertexColor;

uniform sampler2D ourTexture;
uniform bool flipTexture;

void main()
{
    vec2 texCoord = TexCoord;
    if (flipTexture) {
        texCoord.y = 1.0 - texCoord.y;
    }
    vec4 texColor = texture(ourTexture, texCoord);

    // If the texture is uniform gray (default), use vertex color instead
    if (length(texColor.rgb - vec3(0.5)) < 0.01) {
        FragColor = vec4(vertexColor, 1.0);
    } else {
        FragColor = texColor;
    }
}
