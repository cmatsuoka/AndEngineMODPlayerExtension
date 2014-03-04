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

#include <string.h>
#include <errno.h>
#include <jni.h>
#include <xmp.h>

#define METHOD(type,name) JNIEXPORT type JNICALL \
Java_org_helllabs_coremod_Xmp_##name


/*
 * Native API methods
 */

METHOD(jlong, createContext) (JNIEnv *env, jobject obj)
{
        return (jlong)xmp_create_context();
}

METHOD(void, freeContext) (JNIEnv *env, jobject obj, jlong ctx)
{
        xmp_free_context((xmp_context)ctx);
}

METHOD(jint, loadModule) (JNIEnv *env, jobject obj, jlong ctx, jstring path)
{
	const char *filename;
	int res;

	filename = (*env)->GetStringUTFChars(env, path, NULL);
	res = xmp_load_module((xmp_context)ctx, (char *)filename);
	(*env)->ReleaseStringUTFChars(env, path, filename);

	return res;
}

METHOD(jint, testModule) (JNIEnv *env, jobject obj, jstring path, jobject info)
{
	const char *filename;
	int res;
	struct xmp_test_info ti;

	filename = (*env)->GetStringUTFChars(env, path, NULL);
	res = xmp_test_module((char *)filename, &ti);
	(*env)->ReleaseStringUTFChars(env, path, filename);

	if (res < 0) {
		jclass modInfoClass = (*env)->FindClass(env,
	                       	"org/helllabs/coremod/Module$TestInfo");
		jfieldID field;
	
		if (modInfoClass == NULL)
			return JNI_FALSE;
			
		field = (*env)->GetFieldID(env, modInfoClass, "name",
					"Ljava/lang/String;");
		(*env)->SetObjectField(env, info, field,
					(*env)->NewStringUTF(env, ti.name));

		field = (*env)->GetFieldID(env, modInfoClass, "type",
					"Ljava/lang/String;");
		(*env)->SetObjectField(env, info, field,
					(*env)->NewStringUTF(env, ti.type));
	}

	return res;
}

METHOD(void, releaseModule) (JNIEnv *env, jobject obj, jlong ctx)
{
	xmp_release_module((xmp_context)ctx);
}

METHOD(jint, startPlayer) (JNIEnv *env, jobject obj, jlong ctx, jint rate, jint flags)
{
	return xmp_start_player((xmp_context)ctx, rate, flags);
}

METHOD(jobject, getFrameInfo) (JNIEnv *env, jobject obj, jlong ctx, jobject info)
{
	struct xmp_frame_info fi;
	jclass frameInfoClass;
	jfieldID field;

	xmp_get_frame_info((xmp_context)ctx, &fi);

	frameInfoClass = (*env)->FindClass(env,
                       	"org/helllabs/coremod/Player$FrameInfo");

	field = (*env)->GetFieldID(env, frameInfoClass, "pos", "I");
	(*env)->SetIntField(env, info, field, fi.pos);

	field = (*env)->GetFieldID(env, frameInfoClass, "pattern", "I");
	(*env)->SetIntField(env, info, field, fi.pattern);

	field = (*env)->GetFieldID(env, frameInfoClass, "row", "I");
	(*env)->SetIntField(env, info, field, fi.row);

	field = (*env)->GetFieldID(env, frameInfoClass, "numRows", "I");
	(*env)->SetIntField(env, info, field, fi.num_rows);

	field = (*env)->GetFieldID(env, frameInfoClass, "frame", "I");
	(*env)->SetIntField(env, info, field, fi.frame);

	field = (*env)->GetFieldID(env, frameInfoClass, "speed", "I");
	(*env)->SetIntField(env, info, field, fi.speed);

	field = (*env)->GetFieldID(env, frameInfoClass, "bpm", "I");
	(*env)->SetIntField(env, info, field, fi.bpm);

	field = (*env)->GetFieldID(env, frameInfoClass, "time", "I");
	(*env)->SetIntField(env, info, field, fi.time);

	field = (*env)->GetFieldID(env, frameInfoClass, "totalTime", "I");
	(*env)->SetIntField(env, info, field, fi.total_time);

	field = (*env)->GetFieldID(env, frameInfoClass, "frameTime", "I");
	(*env)->SetIntField(env, info, field, fi.frame_time);

	field = (*env)->GetFieldID(env, frameInfoClass,
				"buffer", "Ljava/nio/ByteBuffer;");
	if ((*env)->GetObjectField(env, info, field) == NULL) {
		jobject buf = (*env)->NewDirectByteBuffer(env, fi.buffer,
						XMP_MAX_FRAMESIZE);
		(*env)->SetObjectField(env, info, field, buf);
	}

	field = (*env)->GetFieldID(env, frameInfoClass, "bufferSize", "I");
	(*env)->SetIntField(env, info, field, fi.buffer_size);

	field = (*env)->GetFieldID(env, frameInfoClass, "totalSize", "I");
	(*env)->SetIntField(env, info, field, fi.total_size);

	field = (*env)->GetFieldID(env, frameInfoClass, "volume", "I");
	(*env)->SetIntField(env, info, field, fi.volume);

	field = (*env)->GetFieldID(env, frameInfoClass, "loopCount", "I");
	(*env)->SetIntField(env, info, field, fi.loop_count);

	field = (*env)->GetFieldID(env, frameInfoClass, "virtChannels", "I");
	(*env)->SetIntField(env, info, field, fi.virt_channels);

	field = (*env)->GetFieldID(env, frameInfoClass, "virtUsed", "I");
	(*env)->SetIntField(env, info, field, fi.virt_used);

	field = (*env)->GetFieldID(env, frameInfoClass, "sequence", "I");
	(*env)->SetIntField(env, info, field, fi.sequence);

	return info;
}

METHOD(jint, playFrame) (JNIEnv *env, jobject obj, jlong ctx)
{
	return xmp_play_frame((xmp_context)ctx);
}

METHOD(void, endPlayer) (JNIEnv *env, jobject obj, jlong ctx)
{
	xmp_end_player((xmp_context)ctx);
}

METHOD(int, setPlayer) (JNIEnv *env, jobject obj, jlong ctx, jint param, jlong value)
{
	return xmp_set_player((xmp_context)ctx, param, value);
}

METHOD(int, getPlayer) (JNIEnv *env, jobject obj, jlong ctx, jint param)
{
	return xmp_get_player((xmp_context)ctx, param);
}

METHOD(jobjectArray, getFormatList) (JNIEnv *env, jobject obj)
{
	jobjectArray ret;
	char **list;
	int i;

	list = xmp_get_format_list();
	for (i = 0; list[i]; i++);
	
	ret = (jobjectArray)(*env)->NewObjectArray(env, i,  
				(*env)->FindClass(env, "java/lang/String"),  
				(*env)->NewStringUTF(env, ""));  

	while (i--) {  
		(*env)->SetObjectArrayElement(env, ret, i,
				(*env)->NewStringUTF(env, list[i]));  
	} 

	return ret;
}

METHOD(int, nextPosition) (JNIEnv *env, jobject obj, jlong ctx)
{
	return xmp_next_position((xmp_context)ctx);
}

METHOD(int, prevPosition) (JNIEnv *env, jobject obj, jlong ctx)
{
	return xmp_prev_position((xmp_context)ctx);
}

METHOD(int, setPosition) (JNIEnv *env, jobject obj, jlong ctx, jint num)
{
	return xmp_set_position((xmp_context)ctx, num);
}

METHOD(void, scanModule) (JNIEnv *env, jobject obj, jlong ctx)
{
	xmp_scan_module((xmp_context)ctx);
}

METHOD(void, stopModule) (JNIEnv *env, jobject obj, jlong ctx)
{
	return xmp_stop_module((xmp_context)ctx);
}

METHOD(void, restartModule) (JNIEnv *env, jobject obj, jlong ctx)
{
	return xmp_restart_module((xmp_context)ctx);
}

METHOD(int, seekTime) (JNIEnv *env, jobject obj, jlong ctx, jint time)
{
	return xmp_seek_time((xmp_context)ctx, time);
}

METHOD(int, channelMute) (JNIEnv *env, jobject obj, jlong ctx, jint chn, jint val)
{
	return xmp_channel_mute((xmp_context)ctx, chn, val);
}

METHOD(int, channelVol) (JNIEnv *env, jobject obj, jlong ctx, jint chn, jint val)
{
	return xmp_channel_vol((xmp_context)ctx, chn, val);
}


/*
 * Helpers
 */

METHOD(int, getErrno) (JNIEnv *env, jobject obj)
{
	return errno;
}

METHOD(jobject, getStrError) (JNIEnv *env, jobject obj, jint err)
{
	char c[128];
	strerror_r(err, c, 128);
	return (*env)->NewStringUTF(env, c);
}

METHOD(void, getModData) (JNIEnv *env, jobject obj, jlong ctx, jobject mod)
{
	struct xmp_module_info mi;
	jclass modInfoClass;
	jfieldID field;

	xmp_get_module_info((xmp_context)ctx, &mi);

	modInfoClass = (*env)->FindClass(env,
                       		"org/helllabs/coremod/Module");

	field = (*env)->GetFieldID(env, modInfoClass, "name",
				"Ljava/lang/String;");
	(*env)->SetObjectField(env, mod, field,
				(*env)->NewStringUTF(env, mi.mod->name));

	field = (*env)->GetFieldID(env, modInfoClass, "type",
				"Ljava/lang/String;");
	(*env)->SetObjectField(env, mod, field,
				(*env)->NewStringUTF(env, mi.mod->type));

	field = (*env)->GetFieldID(env, modInfoClass, "numPatterns", "I");
	(*env)->SetIntField(env, mod, field, mi.mod->pat);

	field = (*env)->GetFieldID(env, modInfoClass, "numChannels", "I");
	(*env)->SetIntField(env, mod, field, mi.mod->chn);

	field = (*env)->GetFieldID(env, modInfoClass, "numInstruments", "I");
	(*env)->SetIntField(env, mod, field, mi.mod->ins);

	field = (*env)->GetFieldID(env, modInfoClass, "initialSpeed", "I");
	(*env)->SetIntField(env, mod, field, mi.mod->spd);

	field = (*env)->GetFieldID(env, modInfoClass, "initialBpm", "I");
	(*env)->SetIntField(env, mod, field, mi.mod->bpm);

	field = (*env)->GetFieldID(env, modInfoClass, "length", "I");
	(*env)->SetIntField(env, mod, field, mi.mod->len);

/*
	field = (*env)->GetFieldID(env, modInfoClass, "volBase", "I");
	(*env)->SetIntField(env, info, field, mi.vol_base);

	field = (*env)->GetFieldID(env, modInfoClass, "numSequences", "I");
	(*env)->SetIntField(env, info, field, mi.num_sequences);
*/
}


/* Mixer API */

METHOD(jint, startSmix) (JNIEnv *env, jobject obj, jlong ctx, jint chn, jint smp)
{
	return xmp_start_smix((xmp_context)ctx, chn, smp);
}

METHOD(void, endSmix) (JNIEnv *env, jobject obj, jlong ctx)
{
	xmp_end_smix((xmp_context)ctx);
}

METHOD(jint, smixPlayInstrument) (JNIEnv *env, jobject obj, jlong ctx, jint ins, jint note, jint vol, jint chn)
{
	return xmp_smix_play_instrument((xmp_context)ctx, ins, note, vol, chn);
}

METHOD(jint, smixPlaySample) (JNIEnv *env, jobject obj, jlong ctx, jint ins, jint note, jint vol, jint chn)
{
	return xmp_smix_play_sample((xmp_context)ctx, ins, note, vol, chn);
}

METHOD(jint, smixChannelPan) (JNIEnv *env, jobject obj, jlong ctx, jint chn, jint pan)
{
	return xmp_smix_channel_pan((xmp_context)ctx, chn, pan);
}

METHOD(jint, smixLoadSample) (JNIEnv *env, jobject obj, jlong ctx, jint num, jstring path)
{
	const char *filename;
	int res;

	filename = (*env)->GetStringUTFChars(env, path, NULL);
	res = xmp_smix_load_sample((xmp_context)ctx, num, (char *)filename);
	(*env)->ReleaseStringUTFChars(env, path, filename);

	return res;
}

METHOD(jint, smixReleaseSample) (JNIEnv *env, jobject obj, jlong ctx, jint num)
{
	return xmp_smix_release_sample((xmp_context)ctx, num);
}

