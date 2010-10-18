#version 120

uniform sampler1D faceColours;

void main()
{
  gl_FragColor = texture1D(faceColours, gl_TexCoord[0].s);
}

