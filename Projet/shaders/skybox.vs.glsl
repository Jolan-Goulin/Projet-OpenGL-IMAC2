#version 300 es

precision mediump float;

layout (location = 0) in vec3 aVertexPosition;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;

out vec3 vTexCoords;



void main()
{
    gl_Position =  uMVPMatrix * uMVMatrix *  vec4(aVertexPosition, 1.0);
    vTexCoords = aVertexPosition;
}