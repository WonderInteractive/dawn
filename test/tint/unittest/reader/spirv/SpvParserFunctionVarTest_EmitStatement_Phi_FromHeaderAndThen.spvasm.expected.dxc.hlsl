SKIP: FAILED

static uint x_1 = 0u;
static bool x_7 = false;
static bool x_8 = false;

void main_1() {
  const bool x_101 = x_7;
  const bool x_102 = x_8;
  [loop] while (true) {
    uint x_2_phi = 0u;
    if (x_101) {
      break;
    }
    x_2_phi = 0u;
    if (x_102) {
      x_2_phi = 1u;
      {
        x_1 = x_2_phi;
      }
      continue;
    } else {
      {
        x_1 = x_2_phi;
      }
      continue;
    }
    {
      x_1 = x_2_phi;
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
shader.hlsl:33: error: Loop must have break.
Validation failed.



