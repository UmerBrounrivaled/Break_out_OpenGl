#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec3 spriteColor;

void main() {
    vec4 sampledColor = texture(image, TexCoords);
    FragColor = vec4(spriteColor * sampledColor.rgb, sampledColor.a);
}