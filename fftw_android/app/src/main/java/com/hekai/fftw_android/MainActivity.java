package com.hekai.fftw_android;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener{

	private Button bt_init, bt_fftw_dft_2d, bt_fftw_dft_r2c_2d,
			bt_fftwf_dft_r2c_2d, bt_fftwf_dft_r2c_2d_thread,
			bt_fftwf_dft_r2c_2d_thread_measure;
	
	private TextView tv_result;
	private static final int LOOP_SIZE = 10;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		tv_result = (TextView)findViewById(R.id.tv_result);
		
		bt_init = (Button)findViewById(R.id.bt_init);
		bt_fftw_dft_2d = (Button)findViewById(R.id.bt_fftw_dft_2d);
		bt_fftw_dft_r2c_2d = (Button)findViewById(R.id.bt_fftw_dft_r2c_2d);
		bt_fftwf_dft_r2c_2d = (Button)findViewById(R.id.bt_fftwf_dft_r2c_2d);
		bt_fftwf_dft_r2c_2d_thread = (Button)findViewById(R.id.bt_fftwf_dft_r2c_2d_thread);
		bt_fftwf_dft_r2c_2d_thread_measure = (Button)findViewById(R.id.bt_fftwf_dft_r2c_2d_thread_measure);
		
		bt_init.setOnClickListener(this);
		bt_fftw_dft_2d.setOnClickListener(this);
		bt_fftw_dft_r2c_2d.setOnClickListener(this);
		bt_fftwf_dft_r2c_2d.setOnClickListener(this);
		bt_fftwf_dft_r2c_2d_thread.setOnClickListener(this);
		bt_fftwf_dft_r2c_2d_thread_measure.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {
		if(v.equals(bt_init)){
			mHandler.obtainMessage(MSG_ID_INIT).sendToTarget();
		}else if(v.equals(bt_fftw_dft_2d)){
			mHandler.obtainMessage(MSG_ID_DFT_2D).sendToTarget();
		}else if(v.equals(bt_fftw_dft_r2c_2d)){
			mHandler.obtainMessage(MSG_ID_DFT_R2C_2D).sendToTarget();
		}else if(v.equals(bt_fftwf_dft_r2c_2d)){
			mHandler.obtainMessage(MSG_ID_F_DFT_R2C_2D).sendToTarget();
		}else if(v.equals(bt_fftwf_dft_r2c_2d_thread)){
			mHandler.obtainMessage(MSG_ID_F_DFT_R2C_2D_THREAD).sendToTarget();
		}else if(v.equals(bt_fftwf_dft_r2c_2d_thread_measure)){
			mHandler.obtainMessage(MSG_ID_F_DFT_R2C_2D_THREAD_MEASURE).sendToTarget();
		}
	}
	
	private static final int MSG_ID_UDPATE_RESULT=0;
	private static final int MSG_ID_INIT=1;
	private static final int MSG_ID_DFT_2D=2;
	private static final int MSG_ID_DFT_R2C_2D=3;
	private static final int MSG_ID_F_DFT_R2C_2D=4;
	private static final int MSG_ID_F_DFT_R2C_2D_THREAD=5;
	private static final int MSG_ID_F_DFT_R2C_2D_THREAD_MEASURE=6;
	
	private Handler mHandler = new Handler(){
		
		@Override
		public void handleMessage(Message msg) {
			switch(msg.what){
			case MSG_ID_UDPATE_RESULT:
				String result = msg.obj == null ? "" : (String) msg.obj;
				tv_result.setText(result);
				break;
			case MSG_ID_INIT:
			case MSG_ID_DFT_2D:
			case MSG_ID_DFT_R2C_2D:
			case MSG_ID_F_DFT_R2C_2D:
			case MSG_ID_F_DFT_R2C_2D_THREAD:
			case MSG_ID_F_DFT_R2C_2D_THREAD_MEASURE:
				new FftwThread(this,msg.what).start();
				break;
			}
		};
	};

	class FftwThread extends Thread{
		private Handler handler;
		private int what;
		
		public FftwThread(Handler mHandler, int what) {
			this.handler = mHandler;
			this.what = what;
		}
		
		@Override
		public void run() {
			String result = "";
			for(int i=0;i<LOOP_SIZE;i++){
				switch(what){
				case MSG_ID_INIT:
					result = Utils.fftw_init();
					break;
				case MSG_ID_DFT_2D:
					result = Utils.fftw_dft_2d();
					break;
				case MSG_ID_DFT_R2C_2D:
					result = Utils.fftw_dft_r2c_2d();
					break;
				case MSG_ID_F_DFT_R2C_2D:
					result = Utils.fftwf_dft_r2c_2d();
					break;
				case MSG_ID_F_DFT_R2C_2D_THREAD:
					result = Utils.fftwf_dft_r2c_2d_thread();
					break;
				case MSG_ID_F_DFT_R2C_2D_THREAD_MEASURE:
					result = Utils.fftwf_dft_r2c_2d_thread_measure();
					break;
				}
			}
			handler.obtainMessage(MSG_ID_UDPATE_RESULT, result).sendToTarget();
		}
		
	}

}
