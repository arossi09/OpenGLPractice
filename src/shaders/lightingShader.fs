#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 fragPos;

uniform sampler2D texture1;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{

    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * lightColor;

    //calculate the normal of the vertices
    vec3 norm = normalize(Normal);
    // calculate the vertex direction that the light is hitting the vertex
    vec3 lightDir = normalize(lightPos-fragPos);
    // calculate the angle the light is hitting the vertex
    float diff = max(dot(lightDir, norm), 0);
    //calculate the diffuse
    vec3 diffuse = diff * lightColor;
    //calculate the result with the ambient diffuse and object color
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);

}
