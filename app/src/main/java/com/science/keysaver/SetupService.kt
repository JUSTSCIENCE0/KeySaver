package com.science.keysaver

import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.Spinner
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import android.widget.Toast

class SetupService : AppCompatActivity() {
    private var m_service : String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.setup_service)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        val configurationSpinner: Spinner = findViewById(R.id.configuration_spinner_setup)
        Implementation.fillConfigurationsList(this, configurationSpinner)
        Implementation.setupItemSelection(
            this,
            configurationSpinner,
            AddConfiguration::class,
            null
        )

        m_service = intent.getStringExtra("service")
        // TODO: read service config from db
        val serviceNameTB : EditText = findViewById(R.id.setup_service_name)
        serviceNameTB.setText(m_service)
    }

    override fun onResume() {
        super.onResume()

        val configurationSpinner: Spinner? = findViewById(R.id.configuration_spinner_setup)
        if (configurationSpinner != null) {
            Implementation.fillConfigurationsList(this, configurationSpinner)
        }
    }

    fun onCancel(view: View?) {
        finish()
    }

    fun onConfirm(view: View?) {
        // TODO - implementation

        finish()
    }

    fun onDelete(view: View?) {
        val builder = AlertDialog.Builder(this)

        builder.setTitle(getString(R.string.warning))
        builder.setMessage(getString(R.string.want_continue))

        builder.setPositiveButton(getString(R.string.yes)) { dialog, _ ->
            Toast.makeText(this, "Вы выбрали 'Да'", Toast.LENGTH_SHORT).show()
            dialog.dismiss()
            finish()
        }

        builder.setNegativeButton(getString(R.string.no)) { dialog, _ ->
            Toast.makeText(this, "Вы выбрали 'Нет'", Toast.LENGTH_SHORT).show()
            dialog.dismiss()
        }

        val alertDialog = builder.create()
        alertDialog.show()
    }
}