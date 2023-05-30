#version 410 core

// Constant
uniform mat3      uView;
uniform uvec2     uResolution;
uniform sampler2D uTileAtlas;
uniform uvec2     uTileSize;

// Per Tilemap
uniform uint  uMapWidth;
uniform ivec2 uPosition;
uniform uint  uTilesetID;


#if VERT == 1


layout (location = 0) in uint iTileID;

out uvec2 vTilePos;

void main()
{
	// Model space
	ivec2 tileCoord = ivec2(gl_VertexID % int(uMapWidth), 0.f);
	tileCoord.y = (gl_VertexID - tileCoord.x) / int(uMapWidth);
	tileCoord += uPosition;

	vTilePos = uvec2(iTileID, uTilesetID);
	gl_Position = vec4(tileCoord * uTileSize, 0.f, 1.f);
}


#elif GEOM == 1


layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in uvec2 vTilePos[];

out vec2 gTexCoords;

mat3 cameraInvert = mat3(2.f / uResolution.x, 0.f, 0.f, 0.f, -2.f / uResolution.y, 0.f, 0.f, 0.f, 1.f);

void main()
{
	vec3 size = vec3(uTileSize, 0.f);
	vec2 pos = gl_in[0].gl_Position.xy;
	vec2 atlasSize = textureSize(uTileAtlas, 0);
	uvec2 tilePos = vTilePos[0];
	mat3 view = cameraInvert * uView;

	// Bottom left 0,0
	gl_Position = vec4(view * vec3(pos + size.zy, 1.f), 1.f);
	gTexCoords = (tilePos + vec2(0.f, 1.f)) / vec2(atlasSize / uTileSize);
	EmitVertex();

	// Top left 0,1
	gl_Position = vec4(view * vec3(pos + size.zz, 1.f), 1.f);
	gTexCoords = (tilePos + vec2(0.f, 0.f)) / vec2(atlasSize / uTileSize);
	EmitVertex();

	// Bottom right 1,0
	gl_Position = vec4(view * vec3(pos + size.xy, 1.f), 1.f);
	gTexCoords = (tilePos + vec2(1.f, 1.f)) / vec2(atlasSize / uTileSize);
	EmitVertex();

	// Top right 1,1
	gl_Position = vec4(view * vec3(pos + size.xz, 1.f), 1.f);
	gTexCoords = (tilePos + vec2(1.f, 0.f)) / vec2(atlasSize / uTileSize);
	EmitVertex();

	EndPrimitive();
}


#elif FRAG == 1


in vec2 gTexCoords;

out vec4 fColor;

void main()
{
	fColor = texture(uTileAtlas, gTexCoords);	
}

#endif
