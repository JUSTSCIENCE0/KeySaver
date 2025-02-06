package com.science.keysaver

import android.os.Bundle
import android.view.View
import android.widget.Spinner
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat

class AddConfiguration : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.add_configuration)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        val alphabetsSpinner: Spinner = findViewById(R.id.add_configuration_alphabets)
        Implementation.fillAlphabetsList(this, alphabetsSpinner)
    }

    fun onCancel(view: View?) {
        finish()
    }

    fun onConfirm(view: View?) {
        // TODO - implementation

        finish()
    }
}