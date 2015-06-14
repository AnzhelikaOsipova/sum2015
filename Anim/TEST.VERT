/* Filename: TEST.VERT */

#version 330

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
uniform float Time;
uniform float PartNo;

out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPosition;
out vec2 DrawTexCoord;

void main(void)
{
  DrawColor = vec4(1, 1, 1, 1);
  if(PartNo == 132)
    DrawColor = vec4(0.5, 0.25, 0.23, 1);
  if(PartNo < 89 || PartNo == 73)
    DrawColor = vec4(0.5, 0.25, 0.23, 1);
  if(PartNo >= 89)
    if(PartNo < 101)
      DrawColor = vec4(0, 0, 0, 1);
  if(PartNo == 157 || PartNo == 159 || PartNo == 185 || PartNo == 187 ||
     PartNo >= 190 && PartNo <= 231)
      DrawColor = vec4(0, 0, 0, 1);
  if(PartNo <= 44 )
    DrawColor = vec4(0.9, 0.7, 0.2, 1);
  if(PartNo == 189 )
    DrawColor = vec4(1, 0, 0, 1);
  if(PartNo == 190 || PartNo == 193 || PartNo == 196 || PartNo == 199 ||
     PartNo == 202 || PartNo == 205 || PartNo == 208 || PartNo == 211 ||
     PartNo == 214 || PartNo == 217 || PartNo == 220 || PartNo == 223 ||
     PartNo == 226 || PartNo == 229)
       DrawColor = vec4(0.6, 0.3, 0.3, 1);
  gl_Position = MatrWVP * vec4(InPosition * 0.06, 1);
 
  DrawNormal = mat3(MatrWVInverse) * InNormal;
 
  DrawPosition = mat3(MatrWV) * InPosition;
  DrawTexCoord = InTexCoord;
}
