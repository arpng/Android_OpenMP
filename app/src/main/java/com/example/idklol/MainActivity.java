package com.example.idklol;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.idklol.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'idklol' library on application startup.
    static {
        System.loadLibrary("idklol");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNIC());

    }

    /**
     * A native method that is implemented by the 'idklol' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native String stringFromJNIC();
}