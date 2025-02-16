package com.science.keysaver

import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.Spinner
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.SwitchCompat
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
        val confNameText: EditText = findViewById(R.id.add_configuration_name)
        val pwdLenText: EditText = findViewById(R.id.add_configuration_password_length)
        val upperCaseSwitch: SwitchCompat = findViewById(R.id.add_configuration_upper_case)
        val lowerCaseSwitch: SwitchCompat = findViewById(R.id.add_configuration_lower_case)
        val alphabetsText: Spinner = findViewById(R.id.add_configuration_alphabets)
        val useSpecsetSwitch: SwitchCompat = findViewById(R.id.add_configuration_add_specset)
        val specsetCountText: EditText = findViewById(R.id.add_configuration_spec_count)
        val specsetText: EditText = findViewById(R.id.add_configuration_specset)
        val useDigitsSwitch: SwitchCompat = findViewById(R.id.add_configuration_add_digits)
        val digitsCountText: EditText = findViewById(R.id.add_configuration_digits_count)

        if (isEmpty(confNameText) ||
            isEmpty(pwdLenText) ||
            isEmpty(pwdLenText) ||
            isEmpty(specsetCountText) ||
            isEmpty(specsetText) ||
            isEmpty(digitsCountText)
        ) {
            Toast.makeText(this,
                getString(R.string.required_field_empty),
                Toast.LENGTH_SHORT).show()
            return
        }

        val config = ConfigurationDescr(
            confNameText.text.toString(),
            pwdLenText.text.toString().toInt(),
            upperCaseSwitch.isChecked,
            lowerCaseSwitch.isChecked,
            alphabetsText.selectedItem.toString(),
            useSpecsetSwitch.isChecked,
            specsetCountText.text.toString().toInt(),
            specsetText.text.toString(),
            useDigitsSwitch.isChecked,
            digitsCountText.text.toString().toInt()
        )

        if (Implementation.addConfiguration(this, config))
            finish()
    }

    fun onSelectCase(view: View?) {
        val upperCaseSwitch: SwitchCompat = findViewById(R.id.add_configuration_upper_case)
        val lowerCaseSwitch: SwitchCompat = findViewById(R.id.add_configuration_lower_case)

        if (view?.id == R.id.add_configuration_upper_case) {
            if (!upperCaseSwitch.isChecked) {
                lowerCaseSwitch.isChecked = true
            }
        }
        else if (view?.id == R.id.add_configuration_lower_case) {
            if (!lowerCaseSwitch.isChecked) {
                upperCaseSwitch.isChecked = true
            }
        }

    }
}