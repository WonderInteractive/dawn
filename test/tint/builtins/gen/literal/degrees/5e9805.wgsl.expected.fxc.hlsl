SKIP: FAILED

float16_t tint_degrees(float16_t param_0) {
  return param_0 * 57.295779513082322865;
}

void degrees_5e9805() {
  float16_t res = tint_degrees(float16_t(0.0h));
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  degrees_5e9805();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  degrees_5e9805();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  degrees_5e9805();
  return;
}
FXC validation failure:
D:\Projects\RampUp\dawn\test\tint\builtins\Shader@0x0000021D8DD40440(1,1-9): error X3000: unrecognized identifier 'float16_t'

