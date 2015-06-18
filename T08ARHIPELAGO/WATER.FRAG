/* Filename: TEST.FRAG: */

#version 330
                                                 	
uniform float Time;
uniform float PartNo;
uniform float TotalParts;

uniform mat4 MatrView;
uniform mat4 MatrWVInverse;
uniform mat4 MatrWorld;

out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Kp;
uniform float Kt;

uniform float IsTextureUse;
uniform sampler2D DrawTexture;


vec4 Shade( vec3 P, vec3 N )
{
  float a = 1;
  vec3 color = vec3(0, 0, 0);
  vec3 L = normalize(vec3(0.5, 6, 1));
  vec3 V = -vec3(-MatrView[3][0], -MatrView[3][1], -MatrView[3][2]);
  V = normalize(V * vec3(MatrWVInverse * vec4(0, 0, 0, 1)) - L);
  N = faceforward(N, V, N);
  N = normalize(N);

  vec3 nKa = vec3(0, 0, 1);
  vec3 nKd = vec3(0.4, 0.4, 0.4);
  vec3 nKs = vec3(0.7, 0.7, 0.7);
  float nKp = 20;
  float nKt = 1;
  color = nKa;
   float nl = dot(N, L); 
  if (nl > 0)
  {
    if (IsTextureUse > 0)
    {
      vec4 tc = texture2D(DrawTexture, DrawTexCoord);
      a = tc.a;
      color += nKd * tc.rgb * nl;
    }
    else
      color += nKd * nl;
  }

  vec3 R = reflect(V, N);
  float rl = dot(R, L); 
  if (rl > 0)
    color += nKs * pow(rl, nKp);
  return vec4(0.37 * color, nKt * a);
}

void main( void )
{
  OutColor = DrawColor * Shade(DrawPosition.xyz, DrawNormal);
}


