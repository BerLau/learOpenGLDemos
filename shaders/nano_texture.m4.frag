#version 330 core
// include(`./shaders/util_h.m4.frag')

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

#define MAX_NR_DIRECTION_LIGHTS 10
#define MAX_NR_POINT_LIGHTS 10
#define MAX_NR_SPOT_LIGHTS 10

uniform int NrDirLights;
uniform int NrPointLights;
uniform int NrSpotLights;

uniform Material material;
uniform DirLight dirLights[MAX_NR_DIRECTION_LIGHTS];
uniform PointLight pointLights[MAX_NR_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_NR_SPOT_LIGHTS];

uniform vec3 viewPos;


void main()
{    
    // FragColor = texture(material.texture_diffuse, TexCoords);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result_color = vec3(0.0, 0.0, 0.0);

    for (int i =0; i < min(MAX_NR_DIRECTION_LIGHTS, NrDirLights); i++){
        result_color += CalcDirLightWithTexture(dirLights[i], norm, viewDir, material, TexCoords); 
    }
    
    for (int i = 0; i < min(MAX_NR_POINT_LIGHTS, NrPointLights); i++){
        result_color += CalcPointLightWithTexture(pointLights[i],norm, FragPos, viewDir, material, TexCoords);
    }

    for (int i =0; i < min(MAX_NR_SPOT_LIGHTS, NrSpotLights); i++){
        result_color += CalcSpotLightWithTexture(spotLights[i], norm, FragPos, viewDir, material, TexCoords);
    }
    
    FragColor = vec4(result_color, 1.0);
}
