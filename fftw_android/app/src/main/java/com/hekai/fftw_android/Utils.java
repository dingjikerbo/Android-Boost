package com.hekai.fftw_android;

public class Utils {
	
	public static native String fftw_init();
	public static native String fftw_dft_2d();
	public static native String fftw_dft_r2c_2d();
	public static native String fftwf_dft_r2c_2d();
	public static native String fftwf_dft_r2c_2d_thread();
	public static native String fftwf_dft_r2c_2d_thread_measure();
	
	static{
		System.loadLibrary("fftw_android");
	}

}
