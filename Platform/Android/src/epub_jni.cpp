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

	end:

    return JNI_VERSION_1_6;
}

// JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved);

jstring returnJstring(JNIEnv *env, char* str, bool freeNative=true) {
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

void readPackages(shared_ptr<ePub3::Container> container)
{
    auto packages = container->Packages();

    for (auto package = packages.begin(); package != packages.end(); ++package) {

        // [_packages addObject:[[[LOXPackage alloc] initWithSdkPackage:*package] autorelease]];
        PRINT("_container.Version: %p\n", container->Version().c_str());
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

void loadChildren(JNIEnv* env, jobject jparent, shared_ptr<ePub3::NavigationElement> parent)
{
	auto children = parent->Children();
	PRINT("children:        %s\n", typeid(children).name());
	for (auto child = children.begin(); child != children.end(); ++child) {
		auto instance = &*child;
		PRINT("child:           %s\n", typeid(child).name());
		PRINT("instance:        %s\n", typeid(instance).name());
		auto navigationElement = instance->get();
		PRINT("instance.get():  %s\n", typeid(navigationElement).name());
		if (ePub3::NavigationPoint *navigationPoint = dynamic_cast<ePub3::NavigationPoint*>(navigationElement)) {
			PRINT("navigationPoint: %s\n", typeid(navigationPoint).name());

			jstring title = returnJstring(env, navigationPoint->Title().c_str(), false);
			jstring content = returnJstring(env, navigationPoint->Content().c_str(), false);
			jobject jchild = env->CallStaticObjectMethod(javaObjectsFactoryClass, createNavigationPoint_ID,
					title, content);
			env->DeleteLocalRef(title);
			env->DeleteLocalRef(content);
			env->CallStaticVoidMethod(javaObjectsFactoryClass, addElementToParent_ID,
					jparent, jchild);
			loadChildren(env, jchild, *instance);
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

		loadChildren(env, jnavigationTable, navigationTable);

		env->DeleteLocalRef(type);
		env->DeleteLocalRef(title);
		env->DeleteLocalRef(sourceHref);
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
//	PRINT("_container.Version: %p\n", _container->Version().c_str());

	jobject jContainer = env->CallStaticObjectMethod(javaObjectsFactoryClass,
			createContainer_ID, (jint) _container.get(), jPath);

    auto packages = _container->Packages();

    for (auto package = packages.begin(); package != packages.end(); ++package) {

        // [_packages addObject:[[[LOXPackage alloc] initWithSdkPackage:*package] autorelease]];
        PRINT("_container.Version: %p\n", _container->Version().c_str());
        PRINT("package type: %p\n", package);
        env->CallStaticVoidMethod(javaObjectsFactoryClass, addPackageToContainer_ID,
        		jContainer, package);

    }

	env->ReleaseStringUTFChars(jPath, nativePath);

	return jContainer;
}

JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->Title().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetSubtitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->Subtitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetShortTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->ShortTitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetCollectionTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->CollectionTitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetEditionTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->EditionTitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetExpandedTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->ExpandedTitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetFullTitle(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->FullTitle().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetUniqueID(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->UniqueID().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetURLSafeUniqueID(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->URLSafeUniqueID().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetPackageID(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->PackageID().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetType(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->Type().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetVersion(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->Version().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetISBN(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->ISBN().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetLanguage(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->Language().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetCopyrightOwner(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->CopyrightOwner().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetSource(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->Source().c_str();
	return returnJstring(env, data);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetAuthors(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->Authors().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jstring JNICALL
Java_com_readium_model_epub3_Package_nativeGetModificationDate(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	char *data = (*pckg)->ModificationDate().c_str();
	return returnJstring(env, data, false);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetSubjects(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	jobject stringList = env->CallStaticObjectMethod(javaObjectsFactoryClass,
			createStringList_ID);
	auto subjects = (*pckg)->Subjects();
    for (auto subject = subjects.begin(); subject != subjects.end(); ++subject) {
		char *data = subject->c_str();
		jstring jsubject = returnJstring(env, data);
		env->CallStaticVoidMethod(javaObjectsFactoryClass, addPackageToContainer_ID,
				stringList, jsubject);
		env->DeleteLocalRef(jsubject);
    }
	return stringList;
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetSpineItems(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);
	jobject spineItemList = env->CallStaticObjectMethod(javaObjectsFactoryClass,
			createSpineItemList_ID);

    auto spine = (*pckg)->FirstSpineItem();
    do {
    	jstring idRef = returnJstring(env, spine->Idref().c_str());
        auto manifestItem = spine->ManifestItem();
    	jstring href = returnJstring(env, manifestItem->BaseHref().c_str(), false);
    	const char* _page_spread;
        if(spine->Spread() == ePub3::PageSpread::Left) {
            _page_spread = "page-spread-left";
        } else if(spine->Spread() == ePub3::PageSpread::Right) {
            _page_spread = "page-spread-right";
        } else {
            _page_spread = "";
        }
    	jstring pageSpread = returnJstring(env, _page_spread);
    	const char *_renditionLayout = "renditionLayout";
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
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);

    auto navigationTable = (*pckg)->TableOfContents();

	return loadNavigationTable(env, navigationTable);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetListOfFigures(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);

    auto navigationTable = (*pckg)->ListOfFigures();

	return loadNavigationTable(env, navigationTable);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetListOfIllustrations(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);

    auto navigationTable = (*pckg)->ListOfIllustrations();

	return loadNavigationTable(env, navigationTable);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetListOfTables(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);

    auto navigationTable = (*pckg)->ListOfTables();

	return loadNavigationTable(env, navigationTable);
}
JNIEXPORT jobject JNICALL
Java_com_readium_model_epub3_Package_nativeGetPageList(JNIEnv* env, jobject thiz, jint pckgPtr)
{
	auto pckg = ((shared_ptr<ePub3::Package>*)pckgPtr);

    auto navigationTable = (*pckg)->PageList();

	return loadNavigationTable(env, navigationTable);
}

#ifdef __cplusplus
}
#endif
