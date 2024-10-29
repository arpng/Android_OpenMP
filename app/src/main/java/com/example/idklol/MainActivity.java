package com.example.idklol;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.LinearLayout;
import android.widget.TextView;

//import com.example.idklol.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'idklol' library on application startup.
    static {
        System.loadLibrary("idklol");
    }

//    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//        binding = ActivityMainBinding.inflate(getLayoutInflater());
//        setContentView(binding.getRoot());
//
//        // Example of a call to a native method
//        TextView tv = binding.sampleText;
////        tv.setText(stringFromJNIC());
//        tv.setText(stringFromJNICLib());

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
         textView2.setText(stringFromJNIOclTest());
        textView2.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT));

        // Create the first TextView
        TextView textView3 = new TextView(this);
//        textView3.setText(stringFromJNICGPU());
        textView3.setText(stringFromJNICLib());
        textView3.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT));

        // Add TextViews to the LinearLayout
        linearLayout.addView(textView1);
        linearLayout.addView(textView2);
        linearLayout.addView(textView3);

        // Set the layout as the content view
        setContentView(linearLayout);

    }

    /**
     * A native method that is implemented by the 'idklol' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNIC();
    public native String stringFromJNIOclTest();
    public native String stringFromJNICLib();
//    public native String stringFromJNICGPU();

}