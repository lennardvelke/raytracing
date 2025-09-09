#version 330 core
out vec4 FragColor;
uniform sampler2D uTex;
void main(){
    vec2 uv = gl_FragCoord.xy / textureSize(uTex,0);
    FragColor = texture(uTex, uv);
}
