/* Filename: TEST.VERT */

#version 400

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform mat4 MatrWVP;
uniform mat4 MatrWV;
uniform mat4 MatrWVInverse;
uniform mat4 MatrWInverse;
uniform float Time;
uniform float PartNo;

out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPosition;
out vec2 DrawTexCoord;

void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition * 60 + vec3(0, sin(InPosition.x * 20 + Time * 3), 0), 1);
  DrawNormal = mat3(MatrWInverse) * InNormal;// * normalize(vec3(InNormal.x - InPosition.x, InNormal.y - InPosition.y, InNormal.z - InPosition.z));// * InNormal - 0.1 * abs(cos(InPosition.x  * 4 + Time * 2));
  DrawPosition = vec3(MatrWorld * vec4(InPosition, 1)) + abs(sin(Time) * 5);
  DrawColor = InColor;
  DrawTexCoord = InTexCoord;
}