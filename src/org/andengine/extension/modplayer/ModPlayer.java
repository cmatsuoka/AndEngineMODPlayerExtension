package org.andengine.extension.modplayer;

import java.io.IOException;

import org.helllabs.coremod.FrameInfo;
import org.helllabs.coremod.Player;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;


public class ModPlayer {
	private static final int SAMPLING_RATE = 44100;

	private final Player player;
	private final AudioTrack audio;
	protected boolean paused;
	private Thread playThread;
	private FrameInfo frameInfo;

	// Initialization-on-demand holder
    private static class Holder {
        static final ModPlayer INSTANCE = new ModPlayer();
    }

    public static ModPlayer getInstance() {
        return Holder.INSTANCE;
    }

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

	private class PlayRunnable implements Runnable {
		public void run() {
			final FrameInfo info = new FrameInfo();

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
			player.releaseModule();
		}
	}

	protected void finalize() throws Throwable {
		player.stop();
		paused = false;
		try {
			playThread.join();
		} catch (InterruptedException e) {
			// do nothing
		}
		super.finalize();
	}

	public void play(final String file) throws IOException {

		player.loadModule(file);

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
		player.setVolume(vol);
	}
	
	public int getVolume() {
		return player.getVolume();
	}
}

