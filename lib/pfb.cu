#include <filterbank_execute.h>
#include <pfb.h>
#include <stream_to_streams.h>
#include <iostream>

namespace cuda
{
void pfb_execute(float2 *in_stream, float2 *in_streams, float2 *fft_in,
                 float2 *fft_out, pfb_cuda_config *cuda_config,
                 int channel_size, int history,
                 size_t shared_mem_size)
{
  dim3 shuffle_blockconfig(cuda_config->shuffle_blockdim_x,
                           cuda_config->shuffle_blockdim_y, 1);
  dim3 fb_blockconfig(cuda_config->fb_blockdim_x, cuda_config->fb_blockdim_y,
                      1);
  stream_to_streams<<<cuda_config->shuffle_griddim, shuffle_blockconfig>>>(
      in_stream, in_streams, channel_size, history);
  cudaDeviceSynchronize();
  cudaError_t err = cudaGetLastError();

  if (err != cudaSuccess) {
    std::cout << "Shuffle kernel failed with message : "
              << cudaGetErrorString(err) << "\n";
  }

  filterbank_execute<<<cuda_config->fb_griddim, fb_blockconfig,
                       shared_mem_size>>>(in_streams, fft_in);
  cudaDeviceSynchronize();

  err = cudaGetLastError();

  if (err != cudaSuccess) {
    std::cout << "Filterbank kernel failed with message : "
              << cudaGetErrorString(err) << "\n";
  }
}

} /* end namespace cuda */
