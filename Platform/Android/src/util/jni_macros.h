#ifndef __RSDK_JNI_MACROS_H__
#define __RSDK_JNI_MACROS_H__

#define INIT_CLASS(class, class_name) { \
          jclass localRefCls = env->FindClass(class_name); \
          if (localRefCls != NULL) \
            {  \
              class = env->NewGlobalRef (localRefCls);  \
              env->DeleteLocalRef (localRefCls);  \
              PRINT("Class %s found.\n", class_name); \
            } else { \
            	PRINT("ERROR: Class %s not found !\n", class_name); \
              res = JNI_FALSE; \
              goto end; \
            } }

#define INIT_METHOD_ID(mtd_id, class, class_name, mtd_name, mtd_sig) \
      mtd_id = env->GetMethodID(class, mtd_name, mtd_sig); \
      if (mtd_id == NULL) \
        { \
    	  PRINT("ERROR: Method %s not found in class %s!\n", mtd_name, class_name); \
          res = JNI_FALSE; \
          goto end;  \
        } else { \
        	PRINT("Method %s found in class %s.\n", mtd_name, class_name); \
        }

#define INIT_STATIC_METHOD_ID(mtd_id, class, class_name, mtd_name, mtd_sig) \
      mtd_id = env->GetStaticMethodID(class, mtd_name, mtd_sig); \
      if (mtd_id == NULL) \
        { \
    	  PRINT("ERROR: Static method %s not found in class %s!\n", mtd_name, class_name); \
          res = JNI_FALSE; \
          goto end;  \
        } else { \
        	PRINT("Static method %s found in class %s.\n", mtd_name, class_name); \
        }

	//      classConstructorMID = env->GetMethodID (class, "<init>", "(I)V");

#define INIT_FIELD_ID(field_id, class, class_name, field_name, field_type) \
          field_id = env->GetFieldID(class, field_name, field_type); \
          if (field_id == NULL) \
            { \
        	  PRINT("ERROR: Field %s not found in class %s !\n", field_name, class_name "\n"); \
              res = JNI_FALSE; \
              goto end; \
            } else { \
            	PRINT("Field %s found in class %s.\n", field_name, class_name); \
            }

#endif /* #ifndef __RSDK_JNI_MACROS_H__ */
