enable f16;

fn frexp_5257dd() {
  var res = frexp(f16());
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  frexp_5257dd();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  frexp_5257dd();
}

@compute @workgroup_size(1)
fn compute_main() {
  frexp_5257dd();
}
