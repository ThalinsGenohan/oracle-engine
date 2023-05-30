#version 410 core

// Constant & Global
uniform mat3      uView;
uniform uvec2     uResolution;
uniform sampler2D uSpriteSheet;

// Per Sprite
uniform uvec2 uSpriteSize;
uniform mat3  uSpriteMatrix;


#if VERT == 1


layout (location = 0) in uvec2 iTexCoords;

out vec2 vTexCoords;

mat3 cameraInvert = mat3(2.f / uResolution.x, 0.f, 0.f, 0.f, -2.f / uResolution.y, 0.f, 0.f, 0.f, 1.f);

void main()
{
	vec2 sheetSize = textureSize(uSpriteSheet, 0);
	vTexCoords = vec2(iTexCoords) / vec2(sheetSize);

	vec2 corner = vec2((int(gl_VertexID & 2) >> 1) - 0.5f, 0.5f - int(gl_VertexID & 1));
	vec3 cornerSize = vec3(corner * uSpriteSize, 1.f);
	gl_Position = vec4(cameraInvert * uView * uSpriteMatrix * cornerSize, 1.f);
}


#elif FRAG == 1


in vec2 vTexCoords;

out vec4 fColor;

void main()
{
	fColor = texture(uSpriteSheet, vTexCoords);
}


#endif
