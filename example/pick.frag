varying vec3 pickID;

void main()
{
  if (pickID.x == 0)
    discard;
  gl_FragColor = vec4(pickID, 1.0);
}

