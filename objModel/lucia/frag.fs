#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse_map;

void main()
{    
    FragColor = texture(diffuse_map, TexCoords);
	//FragColor = vec4(0,0,0,1); 
}