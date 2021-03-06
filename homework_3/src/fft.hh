#ifndef FFT_HH
#define FFT_HH
/* ------------------------------------------------------ */
#include "matrix.hh"
#include "my_types.hh"
#include <fftw3.h>
/* ------------------------------------------------------ */

struct FFT {

  static Matrix<complex> transform(Matrix<complex>& m);
  static Matrix<complex> itransform(Matrix<complex>& m);

  static Matrix<std::complex<int>> computeFrequencies(int size);
};

/* ------------------------------------------------------ */

inline Matrix<complex> FFT::transform(Matrix<complex>& m_in) {
  int n = m_in.size();
  Matrix<complex> m_out(n);
  // 2D DFT Plan
  fftw_plan p = fftw_plan_dft_2d(n, n, reinterpret_cast<fftw_complex*>(m_in.data()), reinterpret_cast<fftw_complex*>(m_out.data()), FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(p);
  fftw_destroy_plan(p);
  return m_out;
}

/* ------------------------------------------------------ */

inline Matrix<complex> FFT::itransform(Matrix<complex>& m_in) {
  int n = m_in.size();
  Matrix<complex> m_out(n);
  // 2D IDFT Plan
  fftw_plan p = fftw_plan_dft_2d(n, n, reinterpret_cast<fftw_complex*>(m_in.data()), reinterpret_cast<fftw_complex*>(m_out.data()), FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(p);
  fftw_destroy_plan(p);
  m_out /= (n*n); // normalization
  return m_out;
}

/* ------------------------------------------------------ */

inline Matrix<std::complex<int>> FFT::computeFrequencies(int size) {
  Matrix<std::complex<int>> m(size);

  for (auto&& entry: index(m)){
    int i = std::get<0>(entry);
    int j = std::get<1>(entry);
    auto& val = std::get<2>(entry);

    int kx = (i <= size/2) ? i : i - size;
    int ky = (j <= size/2) ? j : j - size;
    val = std::complex<int>(kx,ky);
  }
  return m;
}

#endif  // FFT_HH
