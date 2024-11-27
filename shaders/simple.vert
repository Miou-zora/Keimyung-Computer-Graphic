#version 440

layout (location = 0) in vec4 VertexPosition; //Set particular attribute number //Get the first index of VAO and set it on VertexPosition
layout (location = 1) in vec3 VertexNormal; //Get the second index of VAO and set it on VertexNormal

out vec3 LightIntensity; //Vertex Color

uniform vec4 LightLocation;
uniform vec3 Kd;
uniform vec3 Ld;
uniform vec3 Ka;
uniform vec3 La;
uniform vec3 Ks;
uniform vec3 Ls;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
    vec3 N = normalize( NormalMatrix * VertexNormal);
    vec4 P = ModelViewMatrix * VertexPosition;
    vec3 L = normalize(LightLocation - P).xyz;
    vec3 diffuse = Kd * Ld * max( dot(L, N), 0.0);
    vec3 ambient = Ka * La;
    vec3 specular = Ks * Ls * pow( max( dot( reflect(-L, N), normalize(-P.xyz)), 0.0), 16.0);
    LightIntensity = diffuse + ambient + specular;
    gl_Position = MVP * VertexPosition;
}
