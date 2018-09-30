#ifndef _PFB_H_
#define _PFB_H_
#include <cafe_constants.h>
#include <cuda_runtime.h>

namespace cuda {

void pfb_execute(float2 *in_stream, float2 *in_streams, float2 *fft_in,
                 float2 *fft_out, pfb_cuda_config *cuda_config,
                 int channel_size, int history,
                 size_t shared_mem_size);


}  // namespace cuda

#endif /* _PFB_H_ */
