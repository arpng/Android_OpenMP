plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "com.example.idklol"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.example.idklol"
        minSdk = 30
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"

        ndk {                                         //Optional
            abiFilters += "arm64-v8a"
        }

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

//        Needed for OpenCL
        externalNativeBuild {
            cmake {
                arguments += listOf(
                    "-DOpenCL_LIBRARY=C:/Qualcomm/Adreno_OpenCL_SDK.Core.2.1.Windows-x64/opencl-sdk/libs/Android/libOpenCL.so", // set to location of libOpenCL.so
                    "-DOpenCL_INCLUDE_DIR=C:/Qualcomm/Adreno_OpenCL_SDK.Core.2.1.Windows-x64/opencl-sdk/inc", // set to location of CL directory containing OpenCL headers
                    "-DANDROID_ABI=arm64-v8a")
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
    buildFeatures {
        viewBinding = true
    }

    // use the OpenCL library on-device
    packaging{
        jniLibs.excludes += "lib/arm64-v8a/libOpenCL.so"
    }
}

dependencies {

    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)
    implementation(files("C:/Users/aris3/Desktop/nativelib-debug.aar"))
    testImplementation(libs.junit)
    androidTestImplementation(libs.ext.junit)
    androidTestImplementation(libs.espresso.core)
}