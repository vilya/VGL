#version 120
#extension GL_EXT_geometry_shader4 : enable

void main()
{
  for (int i = 0; i < gl_VerticesIn; ++i) {
    int faceNum = gl_PrimitiveIDIn / 2;
    float amount = float(faceNum) / 5.0;
    gl_TexCoord[0].s = amount;
    gl_Position = gl_PositionIn[i];
    EmitVertex();
  }
}
