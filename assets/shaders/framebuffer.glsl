#version 410 core

uniform sampler2D uScreenTexture;


#if VERT == 1


layout (location = 0) in vec4 iVertex;

out vec2 vTexCoords;

void main()
{
	vTexCoords = iVertex.zw;
	gl_Position = vec4(iVertex.xy, 0.f, 1.f);
}


#elif FRAG == 1


in vec2 vTexCoords;

out vec4 fColor;

void main()
{
	fColor = texture(uScreenTexture, vTexCoords);
}

#endif
