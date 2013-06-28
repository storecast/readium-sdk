//
//  epub_jni.cpp
//  ePub3
//
//  Created by AnFengDe on 2013-05-24.
//  Copyright (c) 2012-2013 The Readium Foundation and contributors.
//
//  The Readium SDK is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#include <util/macros.h>
#include <util/jni_macros.h>
#include <ePub3/archive.h>
#include <ePub3/container.h>
#include <string>

#include "epub_jni.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

// --- INSPIRED BY Library/jni/src/jni/com_hw_cookie_ebookreader_engine_adobe_AdobeReader.cpp --------

#define GET_UTF8(jstring, cstring)  \
	const char *cstring = env->GetStringUTFChars(jstring, NULL);\
    if (cstring == NULL)\
    {\
        printf("GetStringUTFChars returned null. Could not allocate memory to hold the UTF-8 string\n");\
        return;\
    }

#define GET_UTF8_RET(jstring, cstring, ret)  \
	const char *cstring = env->GetStringUTFChars(jstring, NULL);\
    if (cstring == NULL)\
    {\
        printf("GetStringUTFChars returned null. Could not allocate memory to hold the UTF-8 string\n");\
        return ret;\
    }

#define RELEASE_UTF8(jstring, cstring)  env->ReleaseStringUTFChars(jstring, cstring);

/*
 * Cached methods and fields IDs. Initialized by the NTX_API_ReaderInitial function.
 */

static jclass javaObjectsFactoryClass = NULL;
static jclass javaSpineItemClass = NULL;

static jmethodID createSpineItem_ID;

#define INIT_FACADE_METHOD_ID(mtd_id, mtd_name, mtd_sig) INIT_METHOD_ID(mtd_id, javaObjectsFactoryClass, "com/readium/jni/JavaObjectsFactory", mtd_name, mtd_sig)
#define INIT_FACADE_STATIC_METHOD_ID(mtd_id, mtd_name, mtd_sig) INIT_STATIC_METHOD_ID(mtd_id, javaObjectsFactoryClass, "com/readium/jni/JavaObjectsFactory", mtd_name, mtd_sig)

/*
 * Class:     com_hw_cookie_ebookreader_engine_adobe_AdobeReader
 * Method:    initIDs
 * Signature: ()Z
 */
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    // Get jclass with env->FindClass.
    // Register methods with env->RegisterNatives.

    jboolean res;

	INIT_CLASS (javaObjectsFactoryClass, "com/readium/jni/JavaObjectsFactory")

	// INIT_STATIC_METHOD_ID(adobeReader_setTmpNativeImage_ID, adobeReaderClass, "com.hw.cookie.ebookreader.engine.adobe.AdobeReader", "setTmpNativeImage", "(III)V")

	// INIT_METHOD_ID (nativeTocItem_addChild_ID, nativeTocItemClass, "com.hw.cookie.ebookreader.engine.NativeTocItem", "addChild", "(Ljava/lang/String;Ljava/lang/String;I)Lcom/hw/cookie/ebookreader/engine/NativeTocItem;")

	INIT_FACADE_STATIC_METHOD_ID (createSpineItem_ID, "createSpineItem", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Lcom/readium/model/epub3/SpineItem;");

	end:

	// Just "for fun", we create a first Java SpineItem object:
	jstring idRef = env->NewStringUTF("idRef");
	jstring packageStorageId = env->NewStringUTF("packageStorageId");
	jstring href = env->NewStringUTF("href");
	jstring pageSpread = env->NewStringUTF("pageSpread");
	jstring renditionLayout = env->NewStringUTF("renditionLayout");

	jobject SpineItem = env->CallStaticObjectMethod(javaObjectsFactoryClass, createSpineItem_ID, idRef, packageStorageId, href, pageSpread, renditionLayout);

    return JNI_VERSION_1_6;
}

// JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved);

jstring returnJstring(JNIEnv *env, char* str) {
	if (str == NULL) {
		return NULL;
	}

	jstring jstr = env->NewStringUTF(str);
	free(str);

	return jstr;
}
//--------------------

shared_ptr<ePub3::Container> _container;

void readPackages()
{
    auto packages = _container->Packages();

    for (auto package = packages.begin(); package != packages.end(); ++package) {

        // [_packages addObject:[[[LOXPackage alloc] initWithSdkPackage:*package] autorelease]];
        PRINT("_container.Version: %p\n", _container->Version().c_str());
        PRINT("package type: %p\n", package);

        // Just to show an example, we create the list of Spine Items:
        auto spine = (*package)->FirstSpineItem();
        PRINT("spine->Idref() = %s\n", spine->Idref().c_str());
        while ((spine = spine->Next()) != nullptr) {
            PRINT("spine->Idref() = %s\n", spine->Idref().c_str());
        }
    }
}

void initializeEpub3SdkApi()
{
	PRINT(">>> Calling ePub3::Archive::Initialize...\n");
    ePub3::Archive::Initialize();
	PRINT("<<< ePub3::Archive::Initialize...\n");
}



JNIEXPORT jint JNICALL
Java_com_readium_EPubJNI_openBook(JNIEnv* env, jobject thiz, jstring jPath)
{
	initializeEpub3SdkApi();

	PRINT("**** Calling GetStringUTFChars...\n");
	const char *nativePath = env->GetStringUTFChars(jPath, NULL);
	if (nativePath == NULL) {
		PRINT("GetStringUTFChars returned null. Could not allocate memory to hold the UTF-8 string\n");
		return -1;
	}
	PRINT("GetStringUTFChars returned %s\n", nativePath);

	std::string path = std::string(nativePath);
	_container = ePub3::Container::OpenContainer(path);
	PRINT("_container OK\n");

	readPackages();

	PRINT("_container OK\n");
	PRINT("_container.Version: %p\n", _container->Version().c_str());

	env->ReleaseStringUTFChars(jPath, nativePath);

	return 0;
}

/*
 * Class:     org_readium_sdk_android_EPub3
 * Method:    closeBook
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_com_readium_EPubJNI_closeBook(JNIEnv* env, jobject thiz, int handle){

}

#ifdef __cplusplus
}
#endif
