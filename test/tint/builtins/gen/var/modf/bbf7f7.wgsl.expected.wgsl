fn modf_bbf7f7() {
  var arg_0 = 1.0f;
  var res = modf(arg_0);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  modf_bbf7f7();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  modf_bbf7f7();
}

@compute @workgroup_size(1)
fn compute_main() {
  modf_bbf7f7();
}
