/* Filename: TEST.FRAG: */

#version 330
                                                 	
uniform float Time;
uniform float PartNo;
uniform float TotalParts;

uniform mat4 MatrView;
uniform mat4 MatrWVInverse;

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
  vec3 L = normalize(vec3(1, 1, 1));
  vec3 V = -vec3(-MatrView[3][0], -MatrView[3][1], -MatrView[3][2]);
  V = normalize(V * vec3(MatrWVInverse * vec4(0, 0, 0, 1)) - L);
  N = faceforward(N, V, N);
  N = normalize(N);

  color = Ka;

  float nl = dot(N, L); 
  if (nl > 0)
  {
    if (IsTextureUse > 0)
    {
      vec4 tc = texture2D(DrawTexture, vec2(DrawPosition));// DrawTexCoord);
      a = tc.a;
      color += Kd * tc.rgb * nl;
    }
    else
      color += Kd * nl;
  }

  vec3 R = reflect(V, N);
  float rl = dot(R, L); 
  if (rl > 0)
    color += Ks * pow(rl, Kp);
  return vec4(0.6 * color, Kt * a);
}

void main( void )
{
  vec4 tc = vec4 (1, 1, 1, 1);
  if (IsTextureUse > 0)
      tc = texture2D(DrawTexture, DrawTexCoord);
  OutColor = DrawColor * tc;
}

