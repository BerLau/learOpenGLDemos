#version 330 core
// include(`./shaders/util_h.m4.frag')

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

#define NR_DIRECTION_LIGHTS 1
#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 1

uniform Material material;
uniform DirLight dirLights[NR_DIRECTION_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform vec3 viewPos;


void main()
{    
    // FragColor = texture(material.texture_diffuse, TexCoords);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result_color = vec3(0.0, 0.0, 0.0);

    for (int i =0; i < NR_DIRECTION_LIGHTS; i++){
        result_color += CalcDirLightNoTexture(dirLights[i], norm, viewDir, material, TexCoords);
    }
    
    for (int i =0; i < NR_POINT_LIGHTS ; i++){
        result_color += CalcPointLightNoTexture(pointLights[i],norm, FragPos, viewDir, material, TexCoords);
    }

    for (int i =0; i < NR_SPOT_LIGHTS; i++){
        result_color += CalcSpotLightNoTexture(spotLights[i], norm, FragPos, viewDir, material, TexCoords);
    }

    FragColor = vec4(result_color, 1.0);
}
