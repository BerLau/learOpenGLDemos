#version 330 core
//include(`./shaders/util_h.m4.frag')

out vec4 FragColor;

#define NR_POINT_LIGHTS 4
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
// in vec3 ourColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D poster;

uniform vec3 viewPos;


void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result_color = CalcDirLight(dirLight, norm, viewDir);
    for (int i =0; i < NR_POINT_LIGHTS; i++){
        result_color += CalcPointLight(pointLights[i],norm, FragPos, viewDir);
    }

    result_color += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
    vec4 result_texture = texture(poster, TexCoords);

    // FragColor = mix(vec4(result_color, 1.0),result_texture, 0.45);
    FragColor = vec4(result_color, 1.0);
    
}


