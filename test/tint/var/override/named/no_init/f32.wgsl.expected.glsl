#version 310 es

void tint_symbol() {
  if ((0.0f == 0.0f)) {
  }
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  tint_symbol();
  return;
}
