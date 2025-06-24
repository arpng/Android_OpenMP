## OpenMP

- Go to File → New → New Project, scroll all the way down and select Native C++.
- Give a desired name and save location to your project. I used Java and my Minimum SDK was API 30(“R”; Android 11). Build configuration language was Kotlin DSL.
- Hit Next, leave C++ standard at Toolchain Default and click on Finish.
- Once you create your project you should see a small popup on the bottom right of your screen about some Microsoft Defender configuration just click on “Automatically” and accept the user prompt that follows.
- I prefer using the “Project” view in the file tree window located in the left of Android Studio's UI, between the sidebar far left and the file editor. Default view is “Android” so just change it from the drop down menu if you want.
- Locate the native-lib.cpp file under “[project name]\app\src\main\cpp” in the file tree window and rename it to native-lib.c(right click on it → Refactor → Rename).
- Place your OpenMP C code inside this file. In order to make the link with the Java code from MainActivity.java(~cpp/java/com.example.[project name]) you need 3 things:
  - Import jni.h .
  - Your “main” function in your C code should have this signature:
  > JNIEXPORT jstring JNICALL Java_com_example_[project name]_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz)
  - Android studio might shorten it with a grey box to:
  > JNIEXPORT jstring JNICALL MainActivity.stringFromJNI(JNIEnv *env, jobject thiz)
  - If you click on the gray box it will show you the full name.
  - What you return should be a Java string given that in MainActivity.java the function stringFromJNI returns a string. Since you are working on C code you need to convert it by returning the following in your MainActivity.stringFromJNI(your “main” C function).
  > return (*env)->NewStringUTF(env, buffer);
- Go to CMakeLists.txt located in the cpp folder.
  - In the `add_library` section make sure that the file name is changed to native-lib.c(should be done automatically after renaming).
  - Add `find_package(OpenMP REQUIRED)` under the `add_library section`.
  - Finally in `target_link_libraries` add `OpenMP::OpenMP_C`.
- Click on the green play button at the top and Voila! your OpenMP code should be running on your device.

- If you wish to add other C files with functions that you want to call from Java then you need to make sure to:
  1. Add a `public native [return type] [func_name]();` line at MainActivity.java and call it wherever you want.
  2. The name of your main C function should be something like `Java_com_example_[project name]_MainActivity_[func_name]` or shortened to `MainActivity.[func_name]`.
  3. In CMakeLists.txt add your C file in the `add_library` section.

## OpenCL
- Create a Qualcomm account and download the OpenCL SDK from this link: https://qpm.qualcomm.com/#/main/tools/details/Adreno_OpenCL_SDK
- Create a C file inside cpp folder with your OpenCL code and then in MainActivity.java add a declaration of a Java function that will link to your C code(as explained before).
- Go to CMakeLists.txt located in the cpp folder.
  - In the `add_library` section add your file containing your OpenCL code.
  - Add `find_package(OpenCL REQUIRED)` under the `add_library` section.
  - Finally in `target_link_libraries` add `OpenCL::OpenCL`.
- Go to build.gradle.kts located in “[project name]\app”.
  - Inside `defaultConfig{}` add the following code. Make sure the paths are correct.

        externalNativeBuild {
                    cmake {
                        arguments += listOf(
                            "-DOpenCL_LIBRARY=C:/Qualcomm/Adreno_OpenCL_SDK.Core.2.1.Windows-x64/opencl-sdk/libs/Android/libOpenCL.so", // set to location of libOpenCL.so
                            "-DOpenCL_INCLUDE_DIR=C:/Qualcomm/Adreno_OpenCL_SDK.Core.2.1.Windows-x64/opencl-sdk/inc", // set to location of CL directory containing OpenCL headers
                            "-DANDROID_ABI=arm64-v8a")
                    }
        } 
  - At the bottom of the `android{}` section add this.
  
        // use the OpenCL library on-device
          packaging{
              jniLibs.excludes += "lib/arm64-v8a/libOpenCL.so"
          }
- Finally, go to AndroidManifest.xml located in “[project name]\app\src\main” and add the following code in the `<application    >` section, between tools:targetApi and the beginning of the `<activity   >` section.

      <uses-native-library
            android:name="libOpenCL.so"
            android:required="true" />
- If you have issues you could try adding the code below in the `defaultConfig{}` section of build.gradle.kts, right under the versionName variable.

      ndk {                                         
          abiFilters += "arm64-v8a"
      }
