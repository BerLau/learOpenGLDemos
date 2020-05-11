#version 330 core
//
struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};

vec3 CalcDirLightWithTexture(DirLight light, vec3 nomral, vec3 viewDir, Material material, vec2 texCoords){
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(nomral, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, nomral);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * texture(material.texture_diffuse, texCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse, texCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular, texCoords).rgb;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLightWithTexture(PointLight light, vec3 nomral, vec3 fragPos, vec3 viewDir, Material material, vec2 texCoords){
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(nomral, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, nomral);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic *( dist * dist));

    vec3 ambient = light.ambient * texture(material.texture_diffuse, texCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse, texCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular, texCoords).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLightWithTexture(SpotLight light, vec3 nomral, vec3 fragPos, vec3 viewDir, Material material, vec2 texCoords){

    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta-light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(nomral, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, nomral);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic *( dist * dist));

    vec3 ambient = light.ambient * texture(material.texture_diffuse, texCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse, texCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular, texCoords).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);

}

vec3 CalcDirLightNoTexture(DirLight light, vec3 nomral, vec3 viewDir, Material material, vec2 texCoords){
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(nomral, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, nomral);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLightNoTexture(PointLight light, vec3 nomral, vec3 fragPos, vec3 viewDir, Material material, vec2 texCoords){
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(nomral, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, nomral);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic *( dist * dist));

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLightNoTexture(SpotLight light, vec3 nomral, vec3 fragPos, vec3 viewDir, Material material, vec2 texCoords){

    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta-light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(nomral, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, nomral);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic *( dist * dist));

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);

}


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


