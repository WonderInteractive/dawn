struct SB_RW_atomic {
  arg_0 : atomic<u32>,
}

struct SB_RW {
  arg_0 : u32,
}

struct x__atomic_compare_exchange_resultu32 {
  old_value : u32,
  exchanged : bool,
}

@group(0) @binding(0) var<storage, read_write> sb_rw : SB_RW_atomic;

fn atomicCompareExchangeWeak_63d8e6() {
  var arg_1 : u32 = 0u;
  var arg_2 : u32 = 0u;
  var res : x__atomic_compare_exchange_resultu32 = x__atomic_compare_exchange_resultu32(0u, false);
  arg_1 = 1u;
  arg_2 = 1u;
  let x_21 : u32 = arg_2;
  let x_22 : u32 = arg_1;
  let old_value_1 = atomicCompareExchangeWeak(&(sb_rw.arg_0), x_22, x_21).old_value;
  let x_23 : u32 = old_value_1;
  res = x__atomic_compare_exchange_resultu32(x_23, (x_23 == x_21));
  return;
}

fn fragment_main_1() {
  atomicCompareExchangeWeak_63d8e6();
  return;
}

@fragment
fn fragment_main() {
  fragment_main_1();
}

fn compute_main_1() {
  atomicCompareExchangeWeak_63d8e6();
  return;
}

@compute @workgroup_size(1i, 1i, 1i)
fn compute_main() {
  compute_main_1();
}
