#version 330 core
out vec4 FragColor;
in vec3 outColor;
uniform vec4 objectColor;
uniform bool isCheck = true;
void main()
{
  if(isCheck == false){
  FragColor = vec4 (1.0f,1.0f,1.0f, 1.0f) * objectColor;
  }
   if(isCheck == true ){
   FragColor = vec4 (outColor, 1.0f);
  }
}