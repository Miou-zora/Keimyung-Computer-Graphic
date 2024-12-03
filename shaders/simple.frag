#version 440

in vec3 Position;
in vec3 Normal;

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

out vec4 FragColor;

void main() {
    vec3 L = normalize(Light.Position.xyz - Position);
    vec3 V = normalize(CamPos - Position);
    vec3 diffuse = Material.Kd * Light.Id * max( dot(L, Normal), 0.0);
    vec3 ambient = Material.Ka * Light.Ia;
    vec3 specular = Material.Ks * Light.Is * pow( max( dot( reflect(-L, Normal), V), 0.0), Material.Shiness);
    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}