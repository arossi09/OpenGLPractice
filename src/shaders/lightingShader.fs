#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 fragPos;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{

    //ambient
    vec3 ambient =light.ambient * material.ambient;

    //diffuse
    //calculate the normal of the vertices
    vec3 norm = normalize(Normal);
    // calculate the vertex direction that the light is hitting the vertex
    vec3 lightDir = normalize(light.position-fragPos);
    // calculate the angle the light is hitting the vertex
    float diff = max(dot(norm, lightDir), 0);
    //calculate the diffuse
    vec3 diffuse = light.diffuse * (material.diffuse * diff);

    //we need the vertex from the frag position to the camera
    vec3 viewDir = normalize(viewPos - fragPos);
    //we need the reflection direction to calculate the angle between
    //the view dir and reflect 
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //we need to multiply the strength spec and light color to get
    //the final specular value
    vec3 specular =light.specular * (spec * material.specular);
    //calculate the result with the ambient diffuse, specular, and  object color
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

}
