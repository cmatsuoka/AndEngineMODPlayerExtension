package org.andengine.extension.modplayer;

import java.io.IOException;

import org.helllabs.coremod.FrameInfo;
import org.helllabs.coremod.Module;
import org.helllabs.coremod.Player;
import org.helllabs.coremod.Xmp;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;


public class ModPlayer {
	private static final int SAMPLING_RATE = 44100;

	private static ModPlayer instance;
	private final Player player;
	private Module module;	
	private final AudioTrack audio;
	protected boolean paused;
	private Thread playThread;
	private FrameInfo frameInfo;

	protected ModPlayer() {
		final int minSize = AudioTrack.getMinBufferSize(SAMPLING_RATE,
				AudioFormat.CHANNEL_OUT_STEREO,
				AudioFormat.ENCODING_PCM_16BIT);

		audio = new AudioTrack(AudioManager.STREAM_MUSIC, SAMPLING_RATE,
				AudioFormat.CHANNEL_OUT_STEREO,
				AudioFormat.ENCODING_PCM_16BIT,
				minSize < 4096 ? 4096 : minSize,
				AudioTrack.MODE_STREAM);

		player = new Player();
	}

	public static ModPlayer getInstance() {
		if (instance == null) {
			instance = new ModPlayer();
			instance.paused = false;
		}
		return instance;
	}


	private class PlayRunnable implements Runnable {
		public void run() {
			FrameInfo info = new FrameInfo();

			while (player.playFrame(info)) {			
				audio.write(info.getBufferArray(), 0, info.getBufferSize());
				frameInfo = info;

				while (paused) {
					try {
						Thread.sleep(100);
					} catch (InterruptedException e) {
						break;
					}
				}
			}

			player.end();
			audio.stop();
			module.release();
		}
	}

	protected void finalize() throws Throwable {
		player.stop();
		paused = false;
		super.finalize();
	}

	public void play(final String file) throws IOException {

		module = new Module(file, player);

		audio.play();
		player.start();

		final PlayRunnable playRunnable = new PlayRunnable();
		playThread = new Thread(playRunnable);
		playThread.start();
	}

	public void stop() {
		player.stop();
		paused = false;
	}

	public void pause() {
		paused = !paused;
	}

	public int time() {
		return frameInfo.getTime();
	}

	public void seek(final int millis) {
		player.seekTime(millis);
	}

	public int getSpeed() {
		return frameInfo.getSpeed();
	}

	public int getBpm() {
		return frameInfo.getBpm();
	}

	public int getPosition() {
		return frameInfo.getPosition();
	}

	public int getPattern() {
		return frameInfo.getPattern();
	}

	public boolean isPaused() {
		return paused;
	}
	
	public void setVolume(final int vol) {
		player.set(Xmp.PLAYER_VOLUME, vol);
	}
	
	public int getVolume() {
		return player.get(Xmp.PLAYER_VOLUME);
	}
}

