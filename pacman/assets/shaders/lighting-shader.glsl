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
uniform mat4 u_LightSpaceMatrix;

out vec3 v_FragPosition;
out vec4 v_FragPositionLightSpace;
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

    // Shadowmap
    v_FragPositionLightSpace = u_LightSpaceMatrix * vec4(v_FragPosition, 1.0);

	gl_Position = u_ViewProjection * vec4(v_FragPosition, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_FragPosition;
in vec4 v_FragPositionLightSpace;
in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in float v_TexID;

uniform sampler2D u_Textures[32];

// Lighting specs
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition; 
uniform vec3 u_ViewPosition;

// Shadow specs
uniform sampler2D u_DiffuseTexture; 
uniform sampler2D u_ShadowMap;

float shadowCalculation(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_FragPosition);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    vec3 shadowMapColor = texture(u_DiffuseTexture, v_TexCoord).rgb;

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
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    float spec = pow(max(dot(norm, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * u_LightColor;

    float shadow = shadowCalculation(v_FragPositionLightSpace);

    //vec3 shadowLighting = vec4((ambient + (1.0 - shadow) * (diffuse + specular) * shadowMapColor), 1.0);

	//color = (texture(u_Textures[int(v_TexID)], v_TexCoord) * v_Color);
    color = (texture(u_Textures[int(v_TexID)], v_TexCoord) * v_Color) * vec4((ambient + (1.0 - shadow) * (diffuse + specular) * shadowMapColor), 1.0);
}