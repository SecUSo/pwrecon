/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#define _PBKDF2_SHA512_

#include "inc_vendor.cl"
#include "inc_hash_constants.h"
#include "inc_hash_functions.cl"
#include "inc_types.cl"
#include "inc_common.cl"

#define COMPARE_S "inc_comp_single.cl"
#define COMPARE_M "inc_comp_multi.cl"

__constant u64 k_sha512[80] =
{
  SHA512C00, SHA512C01, SHA512C02, SHA512C03,
  SHA512C04, SHA512C05, SHA512C06, SHA512C07,
  SHA512C08, SHA512C09, SHA512C0a, SHA512C0b,
  SHA512C0c, SHA512C0d, SHA512C0e, SHA512C0f,
  SHA512C10, SHA512C11, SHA512C12, SHA512C13,
  SHA512C14, SHA512C15, SHA512C16, SHA512C17,
  SHA512C18, SHA512C19, SHA512C1a, SHA512C1b,
  SHA512C1c, SHA512C1d, SHA512C1e, SHA512C1f,
  SHA512C20, SHA512C21, SHA512C22, SHA512C23,
  SHA512C24, SHA512C25, SHA512C26, SHA512C27,
  SHA512C28, SHA512C29, SHA512C2a, SHA512C2b,
  SHA512C2c, SHA512C2d, SHA512C2e, SHA512C2f,
  SHA512C30, SHA512C31, SHA512C32, SHA512C33,
  SHA512C34, SHA512C35, SHA512C36, SHA512C37,
  SHA512C38, SHA512C39, SHA512C3a, SHA512C3b,
  SHA512C3c, SHA512C3d, SHA512C3e, SHA512C3f,
  SHA512C40, SHA512C41, SHA512C42, SHA512C43,
  SHA512C44, SHA512C45, SHA512C46, SHA512C47,
  SHA512C48, SHA512C49, SHA512C4a, SHA512C4b,
  SHA512C4c, SHA512C4d, SHA512C4e, SHA512C4f,
};

void sha512_transform (const u64 w[16], u64 dgst[8])
{
  u64 a = dgst[0];
  u64 b = dgst[1];
  u64 c = dgst[2];
  u64 d = dgst[3];
  u64 e = dgst[4];
  u64 f = dgst[5];
  u64 g = dgst[6];
  u64 h = dgst[7];

  u64 w0_t = w[ 0];
  u64 w1_t = w[ 1];
  u64 w2_t = w[ 2];
  u64 w3_t = w[ 3];
  u64 w4_t = w[ 4];
  u64 w5_t = w[ 5];
  u64 w6_t = w[ 6];
  u64 w7_t = w[ 7];
  u64 w8_t = w[ 8];
  u64 w9_t = w[ 9];
  u64 wa_t = w[10];
  u64 wb_t = w[11];
  u64 wc_t = w[12];
  u64 wd_t = w[13];
  u64 we_t = w[14];
  u64 wf_t = w[15];

  #define ROUND_EXPAND()                            \
  {                                                 \
    w0_t = SHA512_EXPAND (we_t, w9_t, w1_t, w0_t);  \
    w1_t = SHA512_EXPAND (wf_t, wa_t, w2_t, w1_t);  \
    w2_t = SHA512_EXPAND (w0_t, wb_t, w3_t, w2_t);  \
    w3_t = SHA512_EXPAND (w1_t, wc_t, w4_t, w3_t);  \
    w4_t = SHA512_EXPAND (w2_t, wd_t, w5_t, w4_t);  \
    w5_t = SHA512_EXPAND (w3_t, we_t, w6_t, w5_t);  \
    w6_t = SHA512_EXPAND (w4_t, wf_t, w7_t, w6_t);  \
    w7_t = SHA512_EXPAND (w5_t, w0_t, w8_t, w7_t);  \
    w8_t = SHA512_EXPAND (w6_t, w1_t, w9_t, w8_t);  \
    w9_t = SHA512_EXPAND (w7_t, w2_t, wa_t, w9_t);  \
    wa_t = SHA512_EXPAND (w8_t, w3_t, wb_t, wa_t);  \
    wb_t = SHA512_EXPAND (w9_t, w4_t, wc_t, wb_t);  \
    wc_t = SHA512_EXPAND (wa_t, w5_t, wd_t, wc_t);  \
    wd_t = SHA512_EXPAND (wb_t, w6_t, we_t, wd_t);  \
    we_t = SHA512_EXPAND (wc_t, w7_t, wf_t, we_t);  \
    wf_t = SHA512_EXPAND (wd_t, w8_t, w0_t, wf_t);  \
  }

  #define ROUND_STEP(i)                                                                   \
  {                                                                                       \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, a, b, c, d, e, f, g, h, w0_t, k_sha512[i +  0]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, h, a, b, c, d, e, f, g, w1_t, k_sha512[i +  1]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, g, h, a, b, c, d, e, f, w2_t, k_sha512[i +  2]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, f, g, h, a, b, c, d, e, w3_t, k_sha512[i +  3]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, e, f, g, h, a, b, c, d, w4_t, k_sha512[i +  4]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, d, e, f, g, h, a, b, c, w5_t, k_sha512[i +  5]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, c, d, e, f, g, h, a, b, w6_t, k_sha512[i +  6]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, b, c, d, e, f, g, h, a, w7_t, k_sha512[i +  7]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, a, b, c, d, e, f, g, h, w8_t, k_sha512[i +  8]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, h, a, b, c, d, e, f, g, w9_t, k_sha512[i +  9]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, g, h, a, b, c, d, e, f, wa_t, k_sha512[i + 10]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, f, g, h, a, b, c, d, e, wb_t, k_sha512[i + 11]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, e, f, g, h, a, b, c, d, wc_t, k_sha512[i + 12]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, d, e, f, g, h, a, b, c, wd_t, k_sha512[i + 13]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, c, d, e, f, g, h, a, b, we_t, k_sha512[i + 14]); \
    SHA512_STEP (SHA512_F0o, SHA512_F1o, b, c, d, e, f, g, h, a, wf_t, k_sha512[i + 15]); \
  }

  ROUND_STEP (0);

  #ifdef _unroll
  #pragma unroll
  #endif
  for (int i = 16; i < 80; i += 16)
  {
    ROUND_EXPAND (); ROUND_STEP (i);
  }

  dgst[0] += a;
  dgst[1] += b;
  dgst[2] += c;
  dgst[3] += d;
  dgst[4] += e;
  dgst[5] += f;
  dgst[6] += g;
  dgst[7] += h;
}

void hmac_run (const u64 w1[16], const u64 ipad[8], const u64 opad[8], u64 dgst[8])
{
  dgst[0] = ipad[0];
  dgst[1] = ipad[1];
  dgst[2] = ipad[2];
  dgst[3] = ipad[3];
  dgst[4] = ipad[4];
  dgst[5] = ipad[5];
  dgst[6] = ipad[6];
  dgst[7] = ipad[7];

  sha512_transform (w1, dgst);

  u64 w[16];

  w[ 0] = dgst[0];
  w[ 1] = dgst[1];
  w[ 2] = dgst[2];
  w[ 3] = dgst[3];
  w[ 4] = dgst[4];
  w[ 5] = dgst[5];
  w[ 6] = dgst[6];
  w[ 7] = dgst[7];
  w[ 8] = 0x8000000000000000;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = 0;
  w[15] = (128 + 64) * 8;

  dgst[0] = opad[0];
  dgst[1] = opad[1];
  dgst[2] = opad[2];
  dgst[3] = opad[3];
  dgst[4] = opad[4];
  dgst[5] = opad[5];
  dgst[6] = opad[6];
  dgst[7] = opad[7];

  sha512_transform (w, dgst);
}

void hmac_run_x (const u64 ipad[8], const u64 opad[8], u64 dgst[8])
{
  u64 w[16];

  w[ 0] = dgst[0];
  w[ 1] = dgst[1];
  w[ 2] = dgst[2];
  w[ 3] = dgst[3];
  w[ 4] = dgst[4];
  w[ 5] = dgst[5];
  w[ 6] = dgst[6];
  w[ 7] = dgst[7];
  w[ 8] = 0x8000000000000000;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = 0;
  w[15] = (128 + 64) * 8;

  dgst[0] = ipad[0];
  dgst[1] = ipad[1];
  dgst[2] = ipad[2];
  dgst[3] = ipad[3];
  dgst[4] = ipad[4];
  dgst[5] = ipad[5];
  dgst[6] = ipad[6];
  dgst[7] = ipad[7];

  sha512_transform (w, dgst);

  w[ 0] = dgst[0];
  w[ 1] = dgst[1];
  w[ 2] = dgst[2];
  w[ 3] = dgst[3];
  w[ 4] = dgst[4];
  w[ 5] = dgst[5];
  w[ 6] = dgst[6];
  w[ 7] = dgst[7];

  dgst[0] = opad[0];
  dgst[1] = opad[1];
  dgst[2] = opad[2];
  dgst[3] = opad[3];
  dgst[4] = opad[4];
  dgst[5] = opad[5];
  dgst[6] = opad[6];
  dgst[7] = opad[7];

  sha512_transform (w, dgst);
}

void hmac_init (u64 w[16], u64 ipad[8], u64 opad[8])
{
  w[ 0] ^= 0x3636363636363636;
  w[ 1] ^= 0x3636363636363636;
  w[ 2] ^= 0x3636363636363636;
  w[ 3] ^= 0x3636363636363636;
  w[ 4] ^= 0x3636363636363636;
  w[ 5] ^= 0x3636363636363636;
  w[ 6] ^= 0x3636363636363636;
  w[ 7] ^= 0x3636363636363636;
  w[ 8] ^= 0x3636363636363636;
  w[ 9] ^= 0x3636363636363636;
  w[10] ^= 0x3636363636363636;
  w[11] ^= 0x3636363636363636;
  w[12] ^= 0x3636363636363636;
  w[13] ^= 0x3636363636363636;
  w[14] ^= 0x3636363636363636;
  w[15] ^= 0x3636363636363636;

  ipad[0] = SHA512M_A;
  ipad[1] = SHA512M_B;
  ipad[2] = SHA512M_C;
  ipad[3] = SHA512M_D;
  ipad[4] = SHA512M_E;
  ipad[5] = SHA512M_F;
  ipad[6] = SHA512M_G;
  ipad[7] = SHA512M_H;

  sha512_transform (w, ipad);

  w[ 0] ^= 0x6a6a6a6a6a6a6a6a;
  w[ 1] ^= 0x6a6a6a6a6a6a6a6a;
  w[ 2] ^= 0x6a6a6a6a6a6a6a6a;
  w[ 3] ^= 0x6a6a6a6a6a6a6a6a;
  w[ 4] ^= 0x6a6a6a6a6a6a6a6a;
  w[ 5] ^= 0x6a6a6a6a6a6a6a6a;
  w[ 6] ^= 0x6a6a6a6a6a6a6a6a;
  w[ 7] ^= 0x6a6a6a6a6a6a6a6a;
  w[ 8] ^= 0x6a6a6a6a6a6a6a6a;
  w[ 9] ^= 0x6a6a6a6a6a6a6a6a;
  w[10] ^= 0x6a6a6a6a6a6a6a6a;
  w[11] ^= 0x6a6a6a6a6a6a6a6a;
  w[12] ^= 0x6a6a6a6a6a6a6a6a;
  w[13] ^= 0x6a6a6a6a6a6a6a6a;
  w[14] ^= 0x6a6a6a6a6a6a6a6a;
  w[15] ^= 0x6a6a6a6a6a6a6a6a;

  opad[0] = SHA512M_A;
  opad[1] = SHA512M_B;
  opad[2] = SHA512M_C;
  opad[3] = SHA512M_D;
  opad[4] = SHA512M_E;
  opad[5] = SHA512M_F;
  opad[6] = SHA512M_G;
  opad[7] = SHA512M_H;

  sha512_transform (w, opad);
}

__kernel void m12300_init (__global pw_t *pws, __global kernel_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global oraclet_tmp_t *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV0_buf, __global u32 *d_scryptV1_buf, __global u32 *d_scryptV2_buf, __global u32 *d_scryptV3_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 il_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = get_global_id (0);

  if (gid >= gid_max) return;

  u32 w0[4];

  w0[0] = swap32 (pws[gid].i[ 0]);
  w0[1] = swap32 (pws[gid].i[ 1]);
  w0[2] = swap32 (pws[gid].i[ 2]);
  w0[3] = swap32 (pws[gid].i[ 3]);

  u32 w1[4];

  w1[0] = swap32 (pws[gid].i[ 4]);
  w1[1] = swap32 (pws[gid].i[ 5]);
  w1[2] = swap32 (pws[gid].i[ 6]);
  w1[3] = swap32 (pws[gid].i[ 7]);

  u32 w2[4];

  w2[0] = swap32 (pws[gid].i[ 8]);
  w2[1] = swap32 (pws[gid].i[ 9]);
  w2[2] = swap32 (pws[gid].i[10]);
  w2[3] = swap32 (pws[gid].i[11]);

  u32 w3[4];

  w3[0] = swap32 (pws[gid].i[12]);
  w3[1] = swap32 (pws[gid].i[13]);
  w3[2] = swap32 (pws[gid].i[14]);
  w3[3] = swap32 (pws[gid].i[15]);

  /**
   * salt
   */

  u64 data[16];

  data[ 0] = hl32_to_64 (salt_bufs[salt_pos].salt_buf[0], salt_bufs[salt_pos].salt_buf[1]);
  data[ 1] = hl32_to_64 (salt_bufs[salt_pos].salt_buf[2], salt_bufs[salt_pos].salt_buf[3]);
  data[ 2] = 0x415554485f50424b;
  data[ 3] = 0x4446325f53504545;
  data[ 4] = 0x44595f4b45590000;
  data[ 5] = 0x0001800000000000;
  data[ 6] = 0;
  data[ 7] = 0;
  data[ 8] = 0;
  data[ 9] = 0;
  data[10] = 0;
  data[11] = 0;
  data[12] = 0;
  data[13] = 0;
  data[14] = 0;
  data[15] = (128 + 16 + 22 + 4) * 8;

  u64 w[16];

  w[ 0] = hl32_to_64 (w0[0], w0[1]);
  w[ 1] = hl32_to_64 (w0[2], w0[3]);
  w[ 2] = hl32_to_64 (w1[0], w1[1]);
  w[ 3] = hl32_to_64 (w1[2], w1[3]);
  w[ 4] = hl32_to_64 (w2[0], w2[1]);
  w[ 5] = hl32_to_64 (w2[2], w2[3]);
  w[ 6] = hl32_to_64 (w3[0], w3[1]);
  w[ 7] = hl32_to_64 (w3[2], w3[3]);
  w[ 8] = 0;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = 0;
  w[15] = 0;

  u64 ipad[8];
  u64 opad[8];

  hmac_init (w, ipad, opad);

  tmps[gid].ipad[0] = ipad[0];
  tmps[gid].ipad[1] = ipad[1];
  tmps[gid].ipad[2] = ipad[2];
  tmps[gid].ipad[3] = ipad[3];
  tmps[gid].ipad[4] = ipad[4];
  tmps[gid].ipad[5] = ipad[5];
  tmps[gid].ipad[6] = ipad[6];
  tmps[gid].ipad[7] = ipad[7];

  tmps[gid].opad[0] = opad[0];
  tmps[gid].opad[1] = opad[1];
  tmps[gid].opad[2] = opad[2];
  tmps[gid].opad[3] = opad[3];
  tmps[gid].opad[4] = opad[4];
  tmps[gid].opad[5] = opad[5];
  tmps[gid].opad[6] = opad[6];
  tmps[gid].opad[7] = opad[7];

  for (u32 i = 0, j = 1; i < 8; i += 8, j += 1)
  {
    u64 dgst[8];

    hmac_run (data, ipad, opad, dgst);

    tmps[gid].dgst[i + 0] = dgst[0];
    tmps[gid].dgst[i + 1] = dgst[1];
    tmps[gid].dgst[i + 2] = dgst[2];
    tmps[gid].dgst[i + 3] = dgst[3];
    tmps[gid].dgst[i + 4] = dgst[4];
    tmps[gid].dgst[i + 5] = dgst[5];
    tmps[gid].dgst[i + 6] = dgst[6];
    tmps[gid].dgst[i + 7] = dgst[7];

    tmps[gid].out[i + 0] = dgst[0];
    tmps[gid].out[i + 1] = dgst[1];
    tmps[gid].out[i + 2] = dgst[2];
    tmps[gid].out[i + 3] = dgst[3];
    tmps[gid].out[i + 4] = dgst[4];
    tmps[gid].out[i + 5] = dgst[5];
    tmps[gid].out[i + 6] = dgst[6];
    tmps[gid].out[i + 7] = dgst[7];
  }
}

__kernel void m12300_loop (__global pw_t *pws, __global kernel_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global oraclet_tmp_t *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV0_buf, __global u32 *d_scryptV1_buf, __global u32 *d_scryptV2_buf, __global u32 *d_scryptV3_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 il_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  const u32 gid = get_global_id (0);

  if (gid >= gid_max) return;

  u64 ipad[8];

  ipad[0] = tmps[gid].ipad[0];
  ipad[1] = tmps[gid].ipad[1];
  ipad[2] = tmps[gid].ipad[2];
  ipad[3] = tmps[gid].ipad[3];
  ipad[4] = tmps[gid].ipad[4];
  ipad[5] = tmps[gid].ipad[5];
  ipad[6] = tmps[gid].ipad[6];
  ipad[7] = tmps[gid].ipad[7];

  u64 opad[8];

  opad[0] = tmps[gid].opad[0];
  opad[1] = tmps[gid].opad[1];
  opad[2] = tmps[gid].opad[2];
  opad[3] = tmps[gid].opad[3];
  opad[4] = tmps[gid].opad[4];
  opad[5] = tmps[gid].opad[5];
  opad[6] = tmps[gid].opad[6];
  opad[7] = tmps[gid].opad[7];

  u64 dgst[8];

  dgst[0] = tmps[gid].dgst[0];
  dgst[1] = tmps[gid].dgst[1];
  dgst[2] = tmps[gid].dgst[2];
  dgst[3] = tmps[gid].dgst[3];
  dgst[4] = tmps[gid].dgst[4];
  dgst[5] = tmps[gid].dgst[5];
  dgst[6] = tmps[gid].dgst[6];
  dgst[7] = tmps[gid].dgst[7];

  for (u32 j = 0; j < loop_cnt; j++)
  {
    hmac_run_x (ipad, opad, dgst);

    tmps[gid].out[0] ^= dgst[0];
    tmps[gid].out[1] ^= dgst[1];
    tmps[gid].out[2] ^= dgst[2];
    tmps[gid].out[3] ^= dgst[3];
    tmps[gid].out[4] ^= dgst[4];
    tmps[gid].out[5] ^= dgst[5];
    tmps[gid].out[6] ^= dgst[6];
    tmps[gid].out[7] ^= dgst[7];
  }

  tmps[gid].dgst[0] = dgst[0];
  tmps[gid].dgst[1] = dgst[1];
  tmps[gid].dgst[2] = dgst[2];
  tmps[gid].dgst[3] = dgst[3];
  tmps[gid].dgst[4] = dgst[4];
  tmps[gid].dgst[5] = dgst[5];
  tmps[gid].dgst[6] = dgst[6];
  tmps[gid].dgst[7] = dgst[7];
}

__kernel void m12300_comp (__global pw_t *pws, __global kernel_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global oraclet_tmp_t *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV0_buf, __global u32 *d_scryptV1_buf, __global u32 *d_scryptV2_buf, __global u32 *d_scryptV3_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 il_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = get_global_id (0);

  if (gid >= gid_max) return;

  const u32 lid = get_local_id (0);

  u64 w[16];

  w[ 0] = tmps[gid].out[0];
  w[ 1] = tmps[gid].out[1];
  w[ 2] = tmps[gid].out[2];
  w[ 3] = tmps[gid].out[3];
  w[ 4] = tmps[gid].out[4];
  w[ 5] = tmps[gid].out[5];
  w[ 6] = tmps[gid].out[6];
  w[ 7] = tmps[gid].out[7];
  w[ 8] = hl32_to_64 (salt_bufs[salt_pos].salt_buf[0], salt_bufs[salt_pos].salt_buf[1]);
  w[ 9] = hl32_to_64 (salt_bufs[salt_pos].salt_buf[2], salt_bufs[salt_pos].salt_buf[3]);
  w[10] = 0x8000000000000000;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = 0;
  w[15] = (64 + 16) * 8;

  u64 dgst[8];

  dgst[0] = SHA512M_A;
  dgst[1] = SHA512M_B;
  dgst[2] = SHA512M_C;
  dgst[3] = SHA512M_D;
  dgst[4] = SHA512M_E;
  dgst[5] = SHA512M_F;
  dgst[6] = SHA512M_G;
  dgst[7] = SHA512M_H;

  sha512_transform (w, dgst);

  const u32 r0 = h32_from_64 (dgst[0]);
  const u32 r1 = l32_from_64 (dgst[0]);
  const u32 r2 = h32_from_64 (dgst[1]);
  const u32 r3 = l32_from_64 (dgst[1]);

  #define il_pos 0

  #include COMPARE_M
}
