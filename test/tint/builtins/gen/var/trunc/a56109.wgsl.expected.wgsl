enable f16;

fn trunc_a56109() {
  var arg_0 = vec2<f16>(f16());
  var res : vec2<f16> = trunc(arg_0);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  trunc_a56109();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  trunc_a56109();
}

@compute @workgroup_size(1)
fn compute_main() {
  trunc_a56109();
}
