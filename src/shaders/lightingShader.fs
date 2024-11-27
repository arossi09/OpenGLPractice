#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float linear;
    float constant;
    float quadratic;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float linear;
    float constant;
    float quadratic;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 fragPos;

uniform SpotLight spotLight;
uniform PointLight pointLight;
uniform Material material;
uniform vec3 viewPos;
uniform bool toggleFlashlight;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    //calculate the normal of the vertices
    vec3 norm = normalize(Normal);
    //we need the vertex from the frag position to the camera
    vec3 viewDir = normalize(viewPos - fragPos);

    //calculate the result with the ambient diffuse, specular, and  object color
    vec3 result = calcPointLight(pointLight, norm, fragPos, viewDir);

    if(toggleFlashlight)
        result += calcSpotLight(spotLight, norm, fragPos, viewDir);

    FragColor = vec4(result, 1.0);

}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    //ambient
    vec3 ambient =light.ambient * material.ambient;

    // calculate the vertex direction that the light is hitting the vertex
    vec3 lightDir = normalize(light.position-fragPos);

    // calculate the angle the light is hitting the vertex
    float diff = max(dot(normal, lightDir), 0);
    //calculate the diffuse
    vec3 diffuse = light.diffuse * (material.diffuse * diff);

    //we need the reflection direction to calculate the angle between
    //the view dir and reflect 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //we need to multiply the strength spec and light color to get
    //the final specular value
    vec3 specular =light.specular * (spec * material.specular);

    //we need the distance for the attentutaion formula
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
            light.quadratic * (distance *distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);



}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    // calculate the vertex direction that the light is hitting the vertex
    vec3 lightDir = normalize(light.position-fragPos);



    vec3 ambient =light.ambient * material.ambient;

    // calculate the angle the light is hitting the vertex
    float diff = max(dot(normal, lightDir), 0);
    //calculate the diffuse
    vec3 diffuse = light.diffuse * (material.diffuse * diff);

    //we need the reflection direction to calculate the angle between
    //the view dir and reflect 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //we need to multiply the strength spec and light color to get
    //the final specular value
    vec3 specular =light.specular * (spec * material.specular);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
            light.quadratic * (distance *distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    
    ambient *= intensity * attenuation;
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;
    


    return (ambient + diffuse + specular);

}
