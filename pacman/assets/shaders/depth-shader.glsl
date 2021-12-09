// Depth shader for shadow mapping

#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_LightSpaceMatrix;

void main() {
    gl_Position = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

void main()
{
    // To be set as non read/write in shader configuration
    // No point in setting color here
}