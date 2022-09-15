/* Copyright 2022 Sipeed Technology Co., Ltd. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tinymaix.h"

#if TM_ENABLE_STAT
static const char *mdl_type_str[6] = {
    "int8", "int16", "fp32", "fp16", "fp8 1.4.3", "fp8 1.5.2",
};

static const char *tml_str_tbl[TML_MAXCNT] = {
    "Conv2D",   /*TML_CONV2D  = 0,*/
    "GAP",      /*TML_GAP     = 1,*/
    "FC",       /*TML_FC      = 2,*/
    "Softmax",  /*TML_SOFTMAX = 3,*/
    "Reshape",  /*TML_RESHAPE = 4,*/
    "DWConv2D", /*TML_DWCONV2D= 5,*/
};

static const int tml_headsize_tbl[TML_MAXCNT] = {
    sizeof(tml_conv2d_dw_t), sizeof(tml_gap_t),     sizeof(tml_fc_t),
    sizeof(tml_softmax_t),   sizeof(tml_reshape_t), sizeof(tml_conv2d_dw_t),
};

tm_err_t tm_stat(tm_mdlbin_t *b) {
  uint16_t *idim = b->in_dims;
  uint16_t *odim = b->out_dims;
  int sum_param = 0;
  int sum_ops = 0;
  uint8_t *layer_body = (uint8_t *)b->layers_body;
  int layer_i;
  for (layer_i = 0; layer_i < b->layer_cnt; layer_i++) {
    tml_head_t *h = (tml_head_t *)(layer_body);
    if (h->type < TML_MAXCNT) {
      int memout = h->out_dims[1] * h->out_dims[2] * h->out_dims[3];
      sum_param += (h->size - tml_headsize_tbl[h->type]);
      int ops = 0;
      switch (h->type) {
      case TML_CONV2D: {
        tml_conv2d_dw_t *l = (tml_conv2d_dw_t *)(layer_body);
        ops = memout * (l->kernel_w) * (l->kernel_h) *
              (h->in_dims[3]); // MAC as ops
        break;
      }
      case TML_GAP:
        ops = (h->in_dims[1]) * (h->in_dims[2]) * (h->in_dims[3]); // SUM as ops
        break;
      case TML_FC: {
        tml_fc_t *l = (tml_fc_t *)(layer_body);
        ops = (h->out_dims[3]) * (h->in_dims[3]); // MAC as ops
        break;
      }
      case TML_SOFTMAX:
        ops = 6 * (h->out_dims[3]); // mixed
        break;
      case TML_DWCONV2D: {
        tml_conv2d_dw_t *l = (tml_conv2d_dw_t *)(layer_body);
        ops = memout * (l->kernel_w) * (l->kernel_h) * 1; // MAC as ops
        break;
      }
      default:
        ops = 0;
        break;
      }
      sum_ops += ops;
    } else {
      return TM_ERR_LAYERTYPE;
    }
    layer_body += (h->size);
  }
  return TM_OK;
}

#endif
