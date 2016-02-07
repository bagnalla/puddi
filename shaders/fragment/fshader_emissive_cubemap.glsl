varying vec3 cubeMapCoord;

uniform samplerCube cubeMap;

void main()
{
	gl_FragColor = texture(cubeMap, cubeMapCoord);
}
