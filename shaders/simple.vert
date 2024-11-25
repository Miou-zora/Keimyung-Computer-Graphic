#version 440

layout (location = 0) in vec4 VertexPosition; //Set particular attribute number //Get the first index of VAO and set it on VertexPosition
layout (location = 1) in vec3 VertexNormal; //Get the second index of VAO and set it on VertexNormal

out vec3 LightIntensity; //Vertex Color

uniform vec4 LightLocation;
uniform vec3 Kd;
uniform vec3 Ld;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
    vec3 N = normalize( NormalMatrix * VertexNormal);
    //How to transform the VertexPosition into Eye Coordinate?
    vec4 P = ModelViewMatrix * VertexPosition;
    //Vector from Light Locaiton to P
    vec3 L = normalize(LightLocation - P).xyz;
     //Calculate Kd*Id*(L·N) //Remember that if (L·N) < 0, set it to 0.
    LightIntensity = Kd * Ld * max( dot(L, N), 0.0);
    gl_Position = MVP * VertexPosition;
}
