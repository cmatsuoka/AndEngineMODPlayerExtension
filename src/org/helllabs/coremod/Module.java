/* Java API for Coremod
 * Copyright (C) 2014 Claudio Matsuoka
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package org.helllabs.coremod;

import java.io.IOException;
import java.nio.ByteBuffer;

public class Module {

	private final Player player;	// Xmp player
	private final long ctx;			// Xmp context

	private String name;			// Module title
	private String type;			// Module format
	private int numPatterns;		// Number of patterns
	private int numChannels;		// Number of channels
	private int numInstruments;		// Number of instruments
	private int numSamples;			// Number of samples
	private int initialSpeed;		// Initial speed
	private int initialBpm;			// Initial BPM
	private int length;				// Module length in patterns

	public static class TestInfo {
		public String name;
		public String type;
	};

	public static class Sequence {
		private int entryPoint;
		private int duration;
		
		public int getEntryPoint() {
			return entryPoint;
		}
		
		public int getDuration() {
			return duration;
		}
	}

	public static class Event {
		public byte note;
		public byte ins;
		public byte vol;
		public byte fxt;
		public byte fxp;
		public byte f2t;
		public byte f2p;
		public byte flag;
	}
	
	public static class Instrument {
		private String name;
		private int numSamples;
		private int[] sampleID;
		
		public String getName() {
			return name;
		}
		
		public int getNumSamples() {
			return numSamples;
		}
		
		public int sampleID(int n) {
			return sampleID[n];
		}
	}

	public static class Sample {
		private String name;
		private int length;
		private int loopStart;
		private int loopEnd;
		private int flags;
		private ByteBuffer data;
		
		public String getName() {
			return name;
		}
		
		public int getLength() {
			return length;
		}
		
		public int getLoopStart() {
			return loopStart;
		}
		
		public int getLoopEnd() {
			return loopEnd;
		}
		
		public int getFlags() {
			return flags;
		}
		
		public ByteBuffer getData() {
			return data;
		}
	}
	
	
	public Module(final String path) throws IOException {
		this(path, new Player());
	}

	public Module(final String path, final Player player) throws IOException {

		this.player = player;
		this.ctx = player.getContext();

		final int code = Xmp.loadModule(ctx, path);
		if (code < 0) {
			throw new IOException(Xmp.errorString[-code]);
		}

		Xmp.getModData(ctx, this);
	}

	public Module release() {
		Xmp.releaseModule(ctx);
		return this;
	}

	public static TestInfo test(final String path, TestInfo info) throws IOException {
		if (info == null) {
			info = new Module.TestInfo();
		}
		final int code = Xmp.testModule(path, info);

		if (code == -Xmp.ERROR_SYSTEM) {
			throw new IOException(Xmp.getStrError(Xmp.getErrno()));
		} else if (code < 0) {
			throw new IOException(Xmp.errorString[-code]);
		}

		return info;
	}

	public static TestInfo test(final String path) throws IOException {
		return test(path, null);
	}
	
	public Instrument instrumentData(final int num) {
		final Instrument instrument = new Instrument();
		return instrumentData(num, instrument);
	}
	
	public Instrument instrumentData(final int num, final Instrument instrument) {
		Xmp.getInstrumentData(ctx, num, instrument);
		return instrument;
	}
	
	public Sample sampleData(final int num) {
		final Sample sample = new Sample();
		return sampleData(num, sample);
	}
	
	public Sample sampleData(final int num, final Sample sample) {
		Xmp.getSampleData(ctx, num, sample);
		return sample;
	}
	
	public Player getPlayer() {
		return player;
	}

	public String getName() {
		return name;
	}

	public String getType() {
		return type;
	}

	public int getNumPatterns() {
		return numPatterns;
	}

	public int getNumChannels() {
		return numChannels;
	}

	public int getNumInstruments() {
		return numInstruments;
	}

	public int getNumSamples() {
		return numSamples;
	}
	
	public int getInitialSpeed() {
		return initialSpeed;
	}

	public int getInitialBpm() {
		return initialBpm;
	}

	public int getLength() {
		return length;
	}
}
