SKIP: FAILED

void tan_9f7c9c() {
  vector<float16_t, 2> res = tan((float16_t(0.0h)).xx);
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  tan_9f7c9c();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  tan_9f7c9c();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  tan_9f7c9c();
  return;
}
FXC validation failure:
D:\Projects\RampUp\dawn\test\tint\builtins\Shader@0x00000211853A84A0(2,10-18): error X3000: syntax error: unexpected token 'float16_t'

