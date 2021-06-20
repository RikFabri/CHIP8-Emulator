#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D tex;

void main()
{
    // FragColor = vec4(texCoord, 0.f, 1.0f);
    FragColor = texture(tex, texCoord);
} 