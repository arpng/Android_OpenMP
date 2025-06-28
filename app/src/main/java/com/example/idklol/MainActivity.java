package com.example.idklol;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.LinearLayout;
import android.widget.TextView;

import android.Manifest;
import android.app.Activity;
import android.os.Environment;
import android.widget.Toast;
import androidx.core.app.ActivityCompat;

import java.io.File;

//import com.example.idklol.databinding.ActivityMainBinding;
import com.example.nativelib.NativeLib;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'idklol' library on application startup.
    static {
        System.loadLibrary("idklol");
    }

//    private ActivityMainBinding binding;

    private static final int REQUEST_CODE_STORAGE = 1001;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//        binding = ActivityMainBinding.inflate(getLayoutInflater());
//        setContentView(binding.getRoot());
//
//        // Example of a call to a native method
//        TextView tv = binding.sampleText;
//        tv.setText(stringFromJNIC());
//        tv.setText(stringFromJNICLib());

        // Request runtime permission (for Android 6.0+)
        ActivityCompat.requestPermissions(this,
                new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
                REQUEST_CODE_STORAGE);

        // Build the full path to the image in Download folder
        File downloadDir = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS);
        File imageFile = new File(downloadDir, "sample.png"); // change to your actual file name


        File outFile = new File(downloadDir, "grayscale-out-opencl.png");
        String outputPath = outFile.getAbsolutePath();



        LinearLayout linearLayout = new LinearLayout(this);
        linearLayout.setOrientation(LinearLayout.VERTICAL);

        // Create the first TextView
        TextView textView1 = new TextView(this);
        textView1.setText(stringFromJNIC());
        textView1.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT));

        // Create the second TextView
        TextView textView2 = new TextView(this);
//        textView2.setText(stringFromJNIOclTest());
        textView2.setText(stringFromJNIOmpPi());
        textView2.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT));

        // Create the third TextView
        TextView textView3 = new TextView(this);
//        textView3.setText(stringFromJNICGPU());
//        textView3.setText(stringFromJNICLib());
        textView3.setText(stringFromJNIOpenClInfo());
        textView3.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT));

        // Create the fourth TextView
        TextView textView4 = new TextView(this);
//        NativeLib obj = new NativeLib();
//        textView4.setText(obj.stringFromJNILib());
        textView4.setText(stringFromJNIOpenCLVectorAdd());
        textView4.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT));

        // Create the fifth TextView
        TextView textView5 = new TextView(this);
//        textView5.setText(stringFromJNIPrimesOclTest());
//        textView5.setText(stringFromJNIOpenCLVectorAdd());
//        textView5.setText(stringFromJNIOmpSum());
//
//        if (imageFile.exists()) {
//            String imagePath = imageFile.getAbsolutePath();
//            Toast.makeText(this, "Processing: " + imagePath, Toast.LENGTH_SHORT).show();
//
//            // Call native function with image path
//            textView5.setText(stringFromJNIOclImage(imagePath, outputPath));
//        } else {
//            Toast.makeText(this, "File not found in Downloads!", Toast.LENGTH_LONG).show();
//        }
//        textView5.setText(stringFromJNIOpenClPi());
//        textView5.setText(stringFromJNIOpenClInfo());
        textView5.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT));

        // Create the sixth TextView
        TextView textView6 = new TextView(this);
//        textView6.setText(stringFromJNIOmpVectorAdd());
//        textView6.setText(stringFromJNIOmpPi());
//        textView6.setText(stringFromJNIOclFibonacci());
        textView6.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT));

        // Add TextViews to the LinearLayout
        linearLayout.addView(textView1);
        linearLayout.addView(textView2);
        linearLayout.addView(textView3);
        linearLayout.addView(textView4);
        linearLayout.addView(textView5);
        linearLayout.addView(textView6);

        // Set the layout as the content view
        setContentView(linearLayout);

    }

    /**
     * A native method that is implemented by the 'idklol' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNIC();
    public native String stringFromJNIOpenCLVectorAdd();
    public native String stringFromJNIPrimesOclTest();
    public native String stringFromJNICLib();
    public native String stringFromJNICGPU();
    public native String stringFromJNIOmpSum();
    public native String stringFromJNIOmpVectorAdd();
    public native String stringFromJNIOmpPi();
    public native String stringFromJNIOclImage(String imagePath, String outputPath);
    public native String stringFromJNIOpenClPi();
    public native String stringFromJNIOpenClInfo();
    public native String stringFromJNIOclFibonacci();
}