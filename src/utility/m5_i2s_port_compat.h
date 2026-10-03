// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// ============================================================================
// [idf-v6.1 port]  ESP-IDF v6.0 removed several I2S symbols that M5Unified's
// Speaker/Mic classes reference:
//
//   * components/driver/deprecated/driver/i2s.h  (legacy I2S driver) -- gone
//   * the `i2s_port_t` enum type -- gone.  In v6.x, `driver/i2s_types.h`
//     defines I2S_NUM_0 .. I2S_NUM_2 as plain object-like macros, and
//     I2S_NUM_3 / I2S_NUM_MAX no longer exist at all.
//   * the SOC_I2S_NUM soc-cap that sized M5Unified's own I2S handle table.
//
// M5Unified only ever uses `i2s_port_t` as an *index into its own small handle
// table* (`i2s_chan_handle_t _i2s_handle[...]`); every actual runtime call is
// the modern handle-based `i2s_channel_*()` API.  So restoring an equivalent
// enum plus a handle-table bound is enough to make the code compile, with no
// behavioural change on this code path.
//
// Everything below is either version-guarded (so pre-v6 keeps its native
// definitions untouched) or derived from the soc-cap when one still exists.
// ============================================================================

#ifndef M5_I2S_PORT_COMPAT_H
#define M5_I2S_PORT_COMPAT_H

#include <esp_idf_version.h>

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)

// `driver/i2s_types.h` (already included by the caller, via `driver/i2s_std.h`)
// defines these as plain macros.  Undef them so the enum below can own the
// names: M5Unified writes `i2s_port_t::I2S_NUM_0`, which would otherwise expand
// to the invalid `i2s_port_t::0`.
// Unqualified use (`I2S_NUM_0`) still yields the same integer value 0.
#ifdef I2S_NUM_0
#undef I2S_NUM_0
#endif
#ifdef I2S_NUM_1
#undef I2S_NUM_1
#endif
#ifdef I2S_NUM_2
#undef I2S_NUM_2
#endif

typedef enum
{
  I2S_NUM_0   = 0,
  I2S_NUM_1   = 1,
  I2S_NUM_2   = 2,
  I2S_NUM_3   = 3,
  I2S_NUM_MAX = 4,
} i2s_port_t;

#endif  // ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)

// Bound for M5Unified's static `_i2s_handle[]` table.  Upstream sized this with
// `SOC_I2S_NUM`, which ESP-IDF v6.x removed.  This must stay OUTSIDE the version
// guard above: both the v6 and the pre-v6 builds take the modern
// `driver/i2s_std.h` path here, so both need the bound.
#ifndef M5_I2S_PORT_MAX
 #ifdef SOC_I2S_NUM
  #define M5_I2S_PORT_MAX SOC_I2S_NUM
 #elif defined ( I2S_NUM_MAX )
  #define M5_I2S_PORT_MAX I2S_NUM_MAX
 #else
  #define M5_I2S_PORT_MAX 4
 #endif
#endif

#endif  // M5_I2S_PORT_COMPAT_H
