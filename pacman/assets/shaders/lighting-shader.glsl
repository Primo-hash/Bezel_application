// 3D Shader w/lighting

#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in float a_TexID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model = mat4(1.0f);

out vec3 v_FragPosition;
out vec4 v_Color;
out vec3 v_Normal;
out vec2 v_TexCoord;
out float v_TexID;


void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexID = a_TexID;

	// For Phong lighting
	v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));

	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

	gl_Position = u_ViewProjection * vec4(v_FragPosition, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_FragPosition;
in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in float v_TexID;

uniform sampler2D u_Textures[32];

// Lighting specs
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition; 
uniform vec3 u_ViewPosition;

void main()
{
	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;
  	
    // diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDirection = normalize(u_LightPosition - v_FragPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * u_LightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectDir = reflect(-lightDirection, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColor; 

	color = (texture(u_Textures[int(v_TexID)], v_TexCoord) * v_Color) * vec4((ambient + diffuse + specular), 1.0);
}