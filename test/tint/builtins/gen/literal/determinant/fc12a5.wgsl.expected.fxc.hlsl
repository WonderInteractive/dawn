SKIP: FAILED

void determinant_fc12a5() {
  float16_t res = determinant(matrix<float16_t, 2, 2>((float16_t(0.0h)).xx, (float16_t(0.0h)).xx));
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  determinant_fc12a5();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  determinant_fc12a5();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  determinant_fc12a5();
  return;
}
FXC validation failure:
D:\Projects\RampUp\dawn\test\tint\builtins\Shader@0x000002949F67AAF0(2,3-11): error X3000: unrecognized identifier 'float16_t'
D:\Projects\RampUp\dawn\test\tint\builtins\Shader@0x000002949F67AAF0(2,13-15): error X3000: unrecognized identifier 'res'

