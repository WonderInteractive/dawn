SKIP: FAILED

void ldexp_7485ce() {
  vector<float16_t, 3> arg_0 = (float16_t(0.0h)).xxx;
  int3 arg_1 = (1).xxx;
  vector<float16_t, 3> res = ldexp(arg_0, arg_1);
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  ldexp_7485ce();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  ldexp_7485ce();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  ldexp_7485ce();
  return;
}
FXC validation failure:
D:\Projects\RampUp\dawn\test\tint\builtins\Shader@0x000001553C486F80(2,10-18): error X3000: syntax error: unexpected token 'float16_t'
D:\Projects\RampUp\dawn\test\tint\builtins\Shader@0x000001553C486F80(4,10-18): error X3000: syntax error: unexpected token 'float16_t'

