#version 440

layout (location = 0) in vec4 VertexPosition; //Set particular attribute number //Get the first index of VAO and set it on VertexPosition
layout (location = 1) in vec3 VertexNormal; //Get the second index of VAO and set it on VertexNormal

out vec3 LightIntensity; //Vertex Color

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform vec3 CamPos;

struct LightInfo {
    vec4 Position; // Light position in eye coords.
    vec3 Ia; // Ambient light intensity
    vec3 Id; // Diffuse light intensity
    vec3 Is; // Specular light intensity
};
uniform LightInfo Light;

struct MaterialInfo {
    vec3 Ka; // Ambient reflectivity
    vec3 Kd; // Diffuse reflectivity
    vec3 Ks; // Specular reflectivity
    float Shiness; // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

void main()
{
    vec4 P = ModelMatrix * VertexPosition;
    vec3 L = normalize((Light.Position - P).xyz);
    vec3 N = normalize(NormalMatrix * VertexNormal);
    vec3 V = normalize(CamPos - P.xyz);
    vec3 diffuse = Material.Kd * Light.Id * max( dot(L, N), 0.0);
    vec3 ambient = Material.Ka * Light.Ia;
    vec3 specular = Material.Ks * Light.Is * pow( max( dot( reflect(-L, N), V), 0.0), Material.Shiness);
    LightIntensity = diffuse + ambient + specular;
    gl_Position = MVP * VertexPosition;
}
