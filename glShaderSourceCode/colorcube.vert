#version 330 core
layout (location = 0) in vec3 aPos; // incoming position coordinates 
layout (location = 1) in vec4 aColor; // incoming color coordinates 

out vec4 ourColor; // outgoing color and texture coordinates 

//uniform vec3 aColor;
uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//ourColor = aColor; // setting the attribs to the incoming and then sending it out 
	ourColor = aColor;
}