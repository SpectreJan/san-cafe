#include <cafe_cuda_helper.h>
#include <pfb_channelizer.h>
#include <testing/qa_pfb_channelizer.h>
#include <util/readSamples.h>
#include <util/writeSamples.h>

namespace pfb_channelizer
{
void read_output_samples(
    std::vector<std::vector<std::complex<float>>> &sample_vec,
    std::vector<std::string> &sample_files)
{
  for (unsigned int i = 0; i < sample_vec.size(); ++i) {
    readBinarySamples<std::complex<float>>(sample_files[i], sample_vec[i]);
  }
}

int test_1()
{
  std::cout << "Hello Channelizer test\n";

  const std::string name("Channelizer Test");

  const unsigned int num_channels = 32;
  const unsigned int num_filters = num_channels;
  const int oversampling = 1;

  // Input file
  const std::string i_file = test_dir + "channelizer_test_in.bin";

  const std::string tap_file = test_dir + "resampler_taps.txt";

  // Vector for the samples
  std::vector<std::complex<float>> i_samples;
  std::vector<std::complex<float>> result_vec;
  std::vector<float> taps;

  // Read taps file
  readFloatSamples(tap_file, taps);

  // Read the prepared testfiles
  readBinarySamples<std::complex<float>>(i_file, i_samples);

  // Now initialize the output vector
  std::vector<std::vector<std::complex<float>>> o_samples_vec(
      num_filters,
      std::vector<std::complex<float>>(i_samples.size() / num_filters));

  // Inititialize the file name
  std::vector<std::string> o_file_vec;
  for (unsigned int i = 0; i < num_filters; ++i) {
    std::string file_str =
        test_dir + "/channelizer_testfiles/channelizer_test_out";
    file_str += std::to_string(i) + ".bin";
    o_file_vec.push_back(file_str);
  }

  read_output_samples(o_samples_vec, o_file_vec);

  std::cout << "\n\n";

  const unsigned int num_samples_per_filter = o_samples_vec[0].size();
  const unsigned int output_size = num_samples_per_filter * num_filters;

  result_vec.resize(output_size);

  // Initialize the GPU
  std::cout << "Initializing the GPU\n";
  cafe::cafe_init_cuda();

  // Initialize the pfb channelizer
  filter::pfb_channelizer_ccf::pfb_channelizer_sptr channelizer =
      filter::pfb_channelizer_ccf::pfb_channelizer_factory(taps, num_filters,
                                                           oversampling);

  int samples_produced =
      channelizer->filter(&i_samples[0], &result_vec[0], 512*32);

   for (unsigned int c = 0; c < num_filters; ++c) {
    std::complex<float> *channel_ptr = &result_vec[c * samples_produced];

    for (int i = 0; i < 512; ++i) {
      float diff = std::abs(channel_ptr[i] - o_samples_vec[c][i+1]);
      if (diff > 5e-2) {
        std::cout << diff << " Diff at " << i << " for channel " << c
                  << std::endl;
        std::cout << "Check not passed\n";
        return -1;
      }
    }
    //
    //for(int i = 506; i < 512; ++i) {
    //  std::cout << channel_ptr[i] << " vs " << o_samples_vec[c][i+1] << std::endl;
    //}
  }

  std::cout << "All Channelizer checks passed\n";

  int result = 0;
  return result;
}
} /* namespace pfb_channelizer */
