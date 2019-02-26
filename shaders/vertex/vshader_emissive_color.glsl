attribute vec4 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// compute gl_Position
	gl_Position = projection * view * model * vPosition;
}
