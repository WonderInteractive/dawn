SKIP: FAILED

static uint var_1 = 0u;

void main_1() {
  [loop] while (true) {
    var_1 = 1u;
    if (false) {
      break;
    }
    {
      var_1 = 2u;
    }
  }
  return;
}

void main() {
  main_1();
  return;
}
DXC validation failure:
warning: DXIL.dll not found.  Resulting DXIL will not be signed for use in release environments.

error: validation errors
shader.hlsl:16: error: Loop must have break.
Validation failed.



