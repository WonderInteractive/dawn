fn min_a45171() {
  var res : vec3<i32> = min(vec3<i32>(1), vec3<i32>(1));
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  min_a45171();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  min_a45171();
}

@compute @workgroup_size(1)
fn compute_main() {
  min_a45171();
}
