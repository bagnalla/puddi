attribute vec4 vPosition;
attribute vec4 vNormal;

varying vec3 cubeMapCoord;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform vec4 cameraPosition;
uniform bool reflective;

void main()
{
	// compute vPosition in world space
	vec4 vPositionWorld = model * vPosition;
	
	// reflective
	if (reflective)
	{
		vec3 v = normalize((vPositionWorld - cameraPosition).xyz);
		vec3 N = normalize((model * vNormal).xyz);
		cubeMapCoord = v - 2 * dot(v, N) * N;
	}
	// non reflective
	else
	{
		//cubeMapCoord = (vPositionWorld - model[3]).xyz;
		cubeMapCoord = vPosition.xyz;
	}
	
	// compute gl_Position
	gl_Position = projection * camera * vPositionWorld;
}
