#include "fft.hh"
#include "my_types.hh"
#include <gtest/gtest.h>

/*****************************************************************/
TEST(FFT, transform)
{
  UInt N = 512;
  Matrix<complex> m(N);

  Real k = 2 * M_PI / N;
  for (auto &&entry : index(m))
  {
    int i = std::get<0>(entry);
    int j = std::get<1>(entry);
    auto &val = std::get<2>(entry);
    val = cos(k * i);
  }

  Matrix<complex> res = FFT::transform(m);

  for (auto &&entry : index(res))
  {
    int i = std::get<0>(entry);
    int j = std::get<1>(entry);
    auto &val = std::get<2>(entry);
    if (std::abs(val) > 1e-10)
      std::cout << i << "," << j << " = " << val << std::endl;

    if (i == 1 && j == 0)
      ASSERT_NEAR(std::abs(val), N * N / 2, 1e-10);
    else if (i == N - 1 && j == 0)
      ASSERT_NEAR(std::abs(val), N * N / 2, 1e-10);
    else
      ASSERT_NEAR(std::abs(val), 0, 1e-10);
  }
}
/*****************************************************************/

TEST(FFT, inverse_transform)
{
  UInt N = 512;
  Matrix<complex> m(N);

  Real k = 2 * M_PI / N;
  for (auto &&entry : index(m))
  {
    int i = std::get<0>(entry);
    int j = std::get<1>(entry);
    auto &val = std::get<2>(entry);
    if (i == 1 && j == 0)
      val = N * N / 2;
    else if (i == N - 1 && j == 0)
      val = N * N / 2;
    else
      val = 0;
  }

  Matrix<complex> res = FFT::itransform(m);

  // compare the result
  for (auto &&entry : index(res))
  {
    int i = std::get<0>(entry);
    int j = std::get<1>(entry);
    auto &val = std::get<2>(entry);

    ASSERT_NEAR(val.real(), cos(k * i), 1e-10);
    ASSERT_LE(val.imag(), 1e-10);
  }
}
/*****************************************************************/

TEST(FFT, computeFrequencies)
{
  int N = 512;
  Matrix<std::complex<int>> res = FFT::computeFrequencies(N);

  // numpy.fft.fftfreq as reference
  std::vector<int> np_freq(N);
  if (N % 2 == 0)
  {
    for (int i = 0; i < N; i++)
    {
      if (i <= N / 2)
        np_freq[i] = i;
      else
        np_freq[i] = i - N;
    }
  }
  else
  {
    for (int i = 0; i < N; i++)
    {
      if (i <= N / 2)
        np_freq[i] = i;
      else
        np_freq[i] = i - N;
    }
  }

  // test
  for (auto &&entry : index(res))
  {
    int i = std::get<0>(entry);
    int j = std::get<1>(entry);
    auto &val = std::get<2>(entry);
    ASSERT_NEAR(val.real(), np_freq[i], 1e-10);
    ASSERT_NEAR(val.imag(), np_freq[j], 1e-10);
  }
}
