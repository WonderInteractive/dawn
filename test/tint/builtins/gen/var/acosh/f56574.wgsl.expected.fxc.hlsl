SKIP: FAILED

vector<float16_t, 3> tint_acosh(vector<float16_t, 3> x) {
  return log((x + sqrt(((x * x) - float16_t(1.0h)))));
}

void acosh_f56574() {
  vector<float16_t, 3> arg_0 = (float16_t(0.0h)).xxx;
  vector<float16_t, 3> res = tint_acosh(arg_0);
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  acosh_f56574();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  acosh_f56574();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  acosh_f56574();
  return;
}
FXC validation failure:
D:\Projects\RampUp\dawn\test\tint\builtins\Shader@0x0000027554446BD0(1,8-16): error X3000: syntax error: unexpected token 'float16_t'

