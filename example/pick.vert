varying vec3 pickID;

void main()
{
  pickID = vec3(gl_Color.x, gl_Color.y, float(gl_VertexID));
  gl_Position = ftransform();
}

