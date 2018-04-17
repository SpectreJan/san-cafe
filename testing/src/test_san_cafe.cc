#include <testing/qa_pfb_resampler.h>
#include <testing/qa_pfb_channelizer.h>
#include <complex>
#include <exception>
#include <iostream>
#include <vector>

int main()
{
  std::cout << "Starting Test of SAN-CAFE" << std::endl;

  //pfb_resampler::test_1();
  std::cout << "\n\n\n";
  pfb_channelizer::test_1();

  return 0;
}
