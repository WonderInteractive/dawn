void exp_0f70eb() {
  float4 res = exp((1.0f).xxxx);
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  exp_0f70eb();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  exp_0f70eb();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  exp_0f70eb();
  return;
}
