package com.science.keysaver

import android.annotation.SuppressLint
import android.os.Bundle
import android.provider.ContactsContract.CommonDataKinds.Im
import android.view.View
import android.widget.EditText
import android.widget.Spinner
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AlertDialog
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

    @SuppressLint("SetTextI18n")
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

        if (isEmpty(confNameText)) {
            confNameText.setText("Name")
        }
        if (isEmpty(pwdLenText)) {
            pwdLenText.setText(Implementation.DEFAULT_PASSWORD_LENGTH.toString())
        }
        if (isEmpty(specsetCountText)) {
            specsetCountText.setText(Implementation.DEFAULT_SPECIAL_CHARS_COUNT.toString())
        }
        if (isEmpty(specsetText)) {
            specsetText.setText(getString(R.string.default_characters_set))
        }
        if (isEmpty(digitsCountText)) {
            digitsCountText.setText(Implementation.DEFAULT_DIGITS_COUNT.toString())
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

    fun onSetSpecialUsage(view: View?) {
        val useSpecsetSwitch: SwitchCompat = findViewById(R.id.add_configuration_add_specset)

        if (!useSpecsetSwitch.isChecked) {
            val specsetCountText: EditText = findViewById(R.id.add_configuration_spec_count)
            val specsetText: EditText = findViewById(R.id.add_configuration_specset)

            specsetCountText.setText("")
            specsetCountText.setHint(Implementation.DEFAULT_SPECIAL_CHARS_COUNT.toString())

            specsetText.setText(getString(R.string.default_characters_set))
        }
    }

    fun onSetDigitsUsage(view: View?) {
        val useDigitsSwitch: SwitchCompat = findViewById(R.id.add_configuration_add_digits)

        if (!useDigitsSwitch.isChecked) {
            val digitsCountText: EditText = findViewById(R.id.add_configuration_digits_count)

            digitsCountText.setText("")
            digitsCountText.setHint(Implementation.DEFAULT_DIGITS_COUNT.toString())
        }
    }

    fun onAddConfigurationHelp(view: View?) {
        val dialog = AlertDialog.Builder(this)
            .setTitle(getString(R.string.info))
            .setMessage(getString(R.string.add_configuration_help))
            .setPositiveButton("OK") { dialog, _ ->
                dialog.dismiss()
            }
            .create()

        dialog.show()
    }
}