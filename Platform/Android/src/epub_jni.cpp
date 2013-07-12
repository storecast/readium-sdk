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
#include <ePub3/nav_element.h>
#include <ePub3/nav_point.h>
#include <ePub3/nav_table.h>
#include <ePub3/property.h>
#include <string>
#include <vector>
#include <typeinfo>

#include "epub_jni.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

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
 * Cached methods and fields IDs.
 */

static jclass javaObjectsFactoryClass = NULL;
static jclass javaSpineItemClass = NULL;

static jmethodID createContainer_ID;
static jmethodID addPackageToContainer_ID;
static jmethodID createStringList_ID;
static jmethodID addStringToList_ID;
static jmethodID createSpineItemList_ID;
static jmethodID createSpineItem_ID;
static jmethodID addSpineItemToList_ID;
static jmethodID createNavigationTable_ID;
static jmethodID createNavigationPoint_ID;
static jmethodID addElementToParent_ID;
static jmethodID createBuffer_ID;
static jmethodID appendBytesToBuffer_ID;

static shared_ptr<ePub3::Package> currentPckgPtr;
static shared_ptr<ePub3::Container> currentContainer;

#define pckg    (*((shared_ptr<ePub3::Package>*)pckgPtr))
#define contnr    (*((shared_ptr<ePub3::Container>*)contnrPtr))

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


	INIT_FACADE_STATIC_METHOD_ID (createContainer_ID, "createContainer", "(ILjava/lang/String;)Lcom/readium/model/epub3/Container;");
	INIT_FACADE_STATIC_METHOD_ID (addPackageToContainer_ID, "addPackageToContainer", "(Lcom/readium/model/epub3/Container;I)V");

	INIT_FACADE_STATIC_METHOD_ID (createStringList_ID, "createStringList", "()Ljava/util/List;");
	INIT_FACADE_STATIC_METHOD_ID (addStringToList_ID, "addStringToList", "(Ljava/util/List;Ljava/lang/String;)V");

	INIT_FACADE_STATIC_METHOD_ID (createSpineItemList_ID, "createSpineItemList", "()Ljava/util/List;");
	INIT_FACADE_STATIC_METHOD_ID (createSpineItem_ID, "createSpineItem", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Lcom/readium/model/epub3/SpineItem;");
	INIT_FACADE_STATIC_METHOD_ID (addSpineItemToList_ID, "addSpineItemToList", "(Ljava/util/List;Lcom/readium/model/epub3/SpineItem;)V");

	INIT_FACADE_STATIC_METHOD_ID (createNavigationTable_ID, "createNavigationTable", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Lcom/readium/model/epub3/components/navigation/NavigationTable;");
	INIT_FACADE_STATIC_METHOD_ID (createNavigationPoint_ID, "createNavigationPoint", "(Ljava/lang/String;Ljava/lang/String;)Lcom/readium/model/epub3/components/navigation/NavigationPoint;");
	INIT_FACADE_STATIC_METHOD_ID (addElementToParent_ID, "addElementToParent", "(Lcom/readium/model/epub3/components/navigation/NavigationElement;Lcom/readium/model/epub3/components/navigation/NavigationElement;)V");

	INIT_FACADE_STATIC_METHOD_ID (createBuffer_ID, "createBuffer", "()Ljava/nio/ByteBuffer;");
	INIT_FACADE_STATIC_METHOD_ID (appendBytesToBuffer_ID, "appendBytesToBuffer", "(Ljava/nio/ByteBuffer;[B)Ljava/nio/ByteBuffer;");

	end:

    return JNI_VERSION_1_6;
}

// JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved);

jstring returnJstring(JNIEnv *env, char* str, bool freeNative=false) {
	if (str == NULL) {
		return NULL;
	}

    jstring jstr = env->NewStringUTF(str);
    if (freeNative) {
    	free(str);
    }

	return jstr;
}
//--------------------

char* getProperty(ePub3::Package* package, char* name, char* pref, ePub3::PropertyHolder* forObject)
{
	PRINT("getProperty(%s:%s)", name, pref);
    auto propertyName = ePub3::string(name);
    auto prefix = ePub3::string(pref);
    auto iri = package->MakePropertyIRI(propertyName, prefix);

    auto propertyList = forObject->PropertiesMatching(iri);

    if (propertyList.size() > 0) {
        auto prop = propertyList[0];
//    	PRINT("getProperty(%s:%s) : %s", name, pref, prop->Value().c_str());
        return prop->Value().c_str();
    }
//	PRINT("getProperty(%s:%s) : EMPTY", name, pref);
    return "";
}

void initializeEpub3SdkApi()
{
	PRINT(">>> Calling ePub3::Archive::Initialize...\n");
    ePub3::Archive::Initialize();
	PRINT("<<< ePub3::Archive::Initialize...\n");
}

void loadChildren(JNIEnv* env, jobject jparent, shared_ptr<ePub3::NavigationElement> parent)
{
	auto children = parent->Children();
	for (auto childIt = children.begin(); childIt != children.end(); ++childIt) {
		auto instance = &*childIt;
		auto navigationElement = instance->get();
		if (ePub3::NavigationPoint *navigationPoint = dynamic_cast<ePub3::NavigationPoint*>(navigationElement)) {
			jstring title = returnJstring(env, navigationPoint->Title().c_str(), false);
			jstring content = returnJstring(env, navigationPoint->Content().c_str(), false);
			jobject jchild = env->CallStaticObjectMethod(javaObjectsFactoryClass, createNavigationPoint_ID,
					title, content);
			env->DeleteLocalRef(title);
			env->DeleteLocalRef(content);
			env->CallStaticVoidMethod(javaObjectsFactoryClass, addElementToParent_ID,
					jparent, jchild);
			loadChildren(env, jchild, *instance);
			env->DeleteLocalRef(jchild);
		}
	}
}

jobject loadNavigationTable(JNIEnv* env, shared_ptr<class ePub3::NavigationTable> navigationTable)
{
    if (navigationTable != nullptr) {
		jstring type = returnJstring(env, navigationTable->Type().c_str(), false);
		jstring title = returnJstring(env, navigationTable->Title().c_str(), false);
		jstring sourceHref = returnJstring(env, navigationTable->SourceHref().c_str(), false);

		jobject jnavigationTable = env->CallStaticObjectMethod(javaObjectsFactoryClass, createNavigationTable_ID,
				type, title, sourceHref);

		env->DeleteLocalRef(type);
		env->DeleteLocalRef(title);
		env->DeleteLocalRef(sourceHref);

		loadChildren(env, jnavigationTable, navigationTable);

		return jnavigationTable;
    }
	return NULL;
}


JNIEXPORT jobject JNICALL
Java_com_readium_EPubAPI_openBook(JNIEnv* env, jobject thiz, jstring jPath)
{
	initializeEpub3SdkApi();

	PRINT("**** Calling GetStringUTFChars...\n");
	const char *nativePath = env->GetStringUTFChars(jPath, NULL);
	if (nativePath == NULL) {
		PRINT("GetStringUTFChars returned null. Could not allocate memory to hold the UTF-8 string\n");
		return NULL;
	}
	PRINT("GetStringUTFChars returned %s\n", nativePath);

	std::string path = std::string(nativePath);
	shared_ptr<ePub3::Container> _container = ePub3::Container::OpenContainer(path);
	PRINT("_container OK\n");
	currentContainer = _container;

	jobject jContainer = env->CallStaticObjectMethod(javaObjectsFactoryClass,
			createContainer_ID, (jint) &_container, jPath);

    auto packages = _container->Packages();

    for (auto packageIt = packages.begin(); packageIt != packages.end(); ++packageIt) {
    	auto package = &*(&*packageIt);
        PRINT("_container.Version: %s\n", _container->Version().c_str());
        PRINT("package type: %p %s\n", package, typeid(package).name());
        currentPckgPtr = *package;
        PRINT("currentPckgPtr\n");
        env->CallStaticVoidMethod(javaObjectsFactoryClass, addPackageToContainer_ID,
        		jContainer, (jint) &currentPckgPtr);
        PRINT("addPackageToContainer_ID\n");

    }

	env->ReleaseStringUTFChars(jPath, nativePath);

	return jContainer;
}

JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->Title().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetSubtitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->Subtitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetShortTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->ShortTitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetCollectionTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->CollectionTitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetEditionTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->EditionTitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetExpandedTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->ExpandedTitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetFullTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->FullTitle().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetUniqueID(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->UniqueID().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetURLSafeUniqueID(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->URLSafeUniqueID().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetPackageID(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->PackageID().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetBasePath(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->BasePath().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetType(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->Type().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetVersion(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->Version().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetISBN(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->ISBN().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetLanguage(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->Language().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetCopyrightOwner(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->CopyrightOwner().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetSource(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->Source().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetAuthors(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->Authors().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetModificationDate(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	char *data = pckg->ModificationDate().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetPageProgressionDirection(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	ePub3::PageProgression pageProgressionDirection = pckg->PageProgressionDirection();
	char *direction;
	switch (pageProgressionDirection) {
	case ePub3::PageProgression::LeftToRight:
		direction = "ltr";
		break;
	case ePub3::PageProgression::RightToLeft:
		direction = "rtl";
		break;
	case ePub3::PageProgression::Default:
	default:
		direction = "";
	}
	return returnJstring(env, direction);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetAuthorList(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	jobject stringList = env->CallStaticObjectMethod(javaObjectsFactoryClass,
			createStringList_ID);
	auto authorNames = pckg->AuthorNames();
    for (auto author = authorNames.begin(); author != authorNames.end(); ++author) {
		char *data = author->c_str();
		jstring jauthor = returnJstring(env, data, false);
		env->CallStaticVoidMethod(javaObjectsFactoryClass, addStringToList_ID,
				stringList, jauthor);
		env->DeleteLocalRef(jauthor);
    }
	return stringList;
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetSubjects(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	jobject stringList = env->CallStaticObjectMethod(javaObjectsFactoryClass,
			createStringList_ID);
	auto subjects = pckg->Subjects();
    for (auto subject = subjects.begin(); subject != subjects.end(); ++subject) {
		char *data = subject->c_str();
		jstring jsubject = returnJstring(env, data, false);
		env->CallStaticVoidMethod(javaObjectsFactoryClass, addStringToList_ID,
				stringList, jsubject);
		env->DeleteLocalRef(jsubject);
    }
	return stringList;
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetSpineItems(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	jobject spineItemList = env->CallStaticObjectMethod(javaObjectsFactoryClass,
			createSpineItemList_ID);

    auto spine = pckg->FirstSpineItem();
    do {
    	jstring idRef = returnJstring(env, spine->Idref().c_str());
        auto manifestItem = spine->ManifestItem();
    	jstring href = returnJstring(env, manifestItem->BaseHref().c_str(), false);
    	const char* _page_spread;
    	ePub3::PageSpread spread = spine->Spread();
    	switch (spread) {
    	case ePub3::PageSpread::Left:
    		_page_spread = "page-spread-left";
    		break;
    	case ePub3::PageSpread::Right:
    		_page_spread = "page-spread-right";
    		break;
    	case ePub3::PageSpread::Center:
    		_page_spread = "page-spread-center";
    		break;
    	default:
    		_page_spread = "";
    	}
    	jstring pageSpread = returnJstring(env, _page_spread);
    	const char *_renditionLayout = getProperty((&*pckg), "layout", "rendition", (&*spine));
    	jstring renditionLayout = env->NewStringUTF(_renditionLayout);

    	jobject spineItem = env->CallStaticObjectMethod(javaObjectsFactoryClass, createSpineItem_ID,
    			idRef, href, pageSpread, renditionLayout);

		env->CallStaticVoidMethod(javaObjectsFactoryClass, addSpineItemToList_ID,
				spineItemList, spineItem);
		env->DeleteLocalRef(idRef);
		env->DeleteLocalRef(href);
		env->DeleteLocalRef(pageSpread);
		env->DeleteLocalRef(renditionLayout);
		env->DeleteLocalRef(spineItem);

    } while ((spine = spine->Next()) != nullptr);

	return spineItemList;
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetTableOfContents(JNIEnv* env, jobject thiz, jint pckgPtr)
{
    auto navigationTable = pckg->TableOfContents();

	return loadNavigationTable(env, navigationTable);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetListOfFigures(JNIEnv* env, jobject thiz, jint pckgPtr)
{
    auto navigationTable = pckg->ListOfFigures();

	return loadNavigationTable(env, navigationTable);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetListOfIllustrations(JNIEnv* env, jobject thiz, jint pckgPtr)
{
    auto navigationTable = pckg->ListOfIllustrations();

	return loadNavigationTable(env, navigationTable);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetListOfTables(JNIEnv* env, jobject thiz, jint pckgPtr)
{
    auto navigationTable = pckg->ListOfTables();

	return loadNavigationTable(env, navigationTable);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetPageList(JNIEnv* env, jobject thiz, jint pckgPtr)
{
    auto navigationTable = pckg->PageList();

	return loadNavigationTable(env, navigationTable);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeReadStreamForRelativePath(JNIEnv* env, jobject thiz, jint pckgPtr, jstring jrelativePath)
{
	char *relativePath = env->GetStringUTFChars(jrelativePath, NULL);
    auto path = ePub3::string(pckg->BasePath()).append(relativePath);
    auto reader = pckg->ReaderForRelativePath( ePub3::string(relativePath));

	env->ReleaseStringUTFChars(jrelativePath, relativePath);

    if (reader == NULL) {
        PRINT("No archive found for path %s", path.c_str());
        return NULL;
    } else {
    	PRINT("Archive found for path %s", path.c_str());
    }

	int BUFFER_SIZE = 8192;
    char tmpBuffer[BUFFER_SIZE];

    //TODO start check for memory leak
    jobject jbuffer = env->CallStaticObjectMethod(javaObjectsFactoryClass, createBuffer_ID);

    ssize_t readBytes = reader->read(tmpBuffer, BUFFER_SIZE);
    while (readBytes > 0) {
    	jsize length = (jsize) readBytes;
    	jbyteArray jtmpBuffer = env->NewByteArray(readBytes);
        jbyte* jbyteBuffer = malloc(sizeof(jchar) * length);

        for (int i = 0; i < length; i ++) {
        	jbyteBuffer[i] = (jbyte)tmpBuffer[i];
        }
    	env->SetByteArrayRegion(jtmpBuffer, 0, length, jbyteBuffer);
    	jbuffer = env->CallStaticObjectMethod(javaObjectsFactoryClass, appendBytesToBuffer_ID,
        		jbuffer, jtmpBuffer);

		env->DeleteLocalRef(jtmpBuffer);
    	free(jbyteBuffer);
        readBytes = reader->read(tmpBuffer, BUFFER_SIZE);
//        PRINT("After readBytes: %d", readBytes);
    }
    //TODO end check for memory leak
	return jbuffer;
}

JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetProperty(JNIEnv* env, jobject thiz, jint pckgPtr,
		jstring jpropertyName, jstring jprefix)
{
    char* propertyName = env->GetStringUTFChars(jpropertyName, NULL);
    char* prefix = env->GetStringUTFChars(jprefix, NULL);

    return returnJstring(env, getProperty((&*pckg), propertyName, prefix, (&*pckg)));
}


#ifdef __cplusplus
}
#endif
