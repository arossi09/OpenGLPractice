#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 fragPos;

uniform sampler2D texture1;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{

    //ambient
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * lightColor;

    //diffuse
    //calculate the normal of the vertices
    vec3 norm = normalize(Normal);
    // calculate the vertex direction that the light is hitting the vertex
    vec3 lightDir = normalize(lightPos-fragPos);
    // calculate the angle the light is hitting the vertex
    float diff = max(dot(norm, lightDir), 0);
    //calculate the diffuse
    vec3 diffuse = diff * lightColor;

    
    //specular
    float specular_strength = 0.5;
    //we need the vertex from the frag position to the camera
    vec3 viewDir = normalize(viewPos - fragPos);
    //we need the reflection direction to calculate the angle between
    //the view dir and reflect 
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //we need to multiply the strength spec and light color to get
    //the final specular value
    vec3 specular = specular_strength * spec * lightColor;
    //calculate the result with the ambient diffuse, specular, and  object color
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);

}
