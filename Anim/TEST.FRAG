/* Filename: TEST.FRAG: */

#version 330

uniform float Time;
uniform float PartNo;

out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

uniform sampler2D DrawTexture;

void main(void)
{
  if(PartNo < 89);
   // DrawColor = vec4(0, 0, 0, 0);
  
  OutColor = DrawColor * dot(normalize(DrawNormal), normalize(vec3(1, 1, 1)));
  //OutColor = vec4(DrawPosition, 1) * 2;
  //OutColor = vec4(1, 1, 1, 1);
}
