//
//  epub_jni.h
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


#include <jni.h>

#ifndef _EPUB_JNI_H_
#define _EPUB_JNI_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_readium_EPubAPI
 * Method:    nativeOpenBook
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_readium_EPubAPI_nativeOpenBook(JNIEnv* env, jobject thiz, jstring jPath);


/*
 * Class:     com_readium_EPubAPI
 * Method:    closeBook
 * Signature: (I)V
 */
//JNIEXPORT void JNICALL
//Java_com_readium_EPubJNI_closeBook(JNIEnv* env, jobject thiz, int handle);

/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetTitle(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetSubtitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetSubtitle(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetShortTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetShortTitle(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetCollectionTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetCollectionTitle(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetEditionTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetEditionTitle(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetExpandedTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetExpandedTitle(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetFullTitle
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetFullTitle(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetUniqueID
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetUniqueID(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetURLSafeUniqueID
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetURLSafeUniqueID(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetPackageID
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetPackageID(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetType
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetType(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetVersion
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetVersion(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetISBN
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetISBN(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetLanguage
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetLanguage(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetCopyrightOwner
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetCopyrightOwner(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetSource
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetSource(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetAuthors
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetAuthors(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetModificationDate
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetModificationDate(JNIEnv* env, jobject thiz, jint pckgPtr);
/*
 * Class:     com_readium_model_epub3_Package
 * Method:    nativeGetSubjects
 * Signature: (I)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetSubjects(JNIEnv* env, jobject thiz, jint pckgPtr);

#ifdef __cplusplus
}
#endif
#endif
