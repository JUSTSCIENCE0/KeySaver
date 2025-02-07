package com.science.keysaver

import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.Spinner
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat

class AddService : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.add_service)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        val configurationSpinner: Spinner = findViewById(R.id.configuration_spinner_add)
        Implementation.fillConfigurationsList(this, configurationSpinner)
        Implementation.setupItemSelection(
            this,
            configurationSpinner,
            AddConfiguration::class,
            null
        )
    }

    override fun onResume() {
        super.onResume()

        val configurationSpinner: Spinner? = findViewById(R.id.configuration_spinner_add)
        if (configurationSpinner != null) {
            Implementation.fillConfigurationsList(this, configurationSpinner)
        }
    }

    fun onCancel(view: View?) {
        finish()
    }

    fun onConfirm(view: View?) {
        val urlEditText:  EditText = findViewById(R.id.service_url_add)
        val nameEditText: EditText = findViewById(R.id.service_display_name_add)
        val confSpinner:  Spinner  = findViewById(R.id.configuration_spinner_add)

        if (isEmpty(urlEditText) || isEmpty(nameEditText)) {
            Toast.makeText(this,
                getString(R.string.required_field_empty),
                Toast.LENGTH_SHORT).show()
            return
        }

        val service = ServiceDescr(
            urlEditText.text.toString(),
            nameEditText.text.toString(),
            confSpinner.selectedItem.toString()
        )

        if (Implementation.addService(this, service))
            finish()
    }
}