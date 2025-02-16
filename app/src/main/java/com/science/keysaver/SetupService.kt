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
        val serviceDescr = Implementation.getService(this, m_service.toString())
        if (serviceDescr == null) {
            finish()
            return
        }

        val confIndex = Implementation.getConfigurationIndex(this, serviceDescr.conf_id)
        if (confIndex < 0) {
            finish()
            return
        }

        val serviceNameTB : EditText = findViewById(R.id.setup_service_name)
        serviceNameTB.setText(m_service)

        configurationSpinner.post {
            configurationSpinner.setSelection(confIndex)
        }
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
        val builder = AlertDialog.Builder(this)
        builder.setTitle(getString(R.string.warning))
        builder.setMessage(getString(R.string.want_continue))

        val serviceDescr = ServiceDescr("", "", "");
        val serviceNameTB : EditText = findViewById(R.id.setup_service_name)
        val configurationSpinner: Spinner = findViewById(R.id.configuration_spinner_setup)

        if (isEmpty(serviceNameTB)) {
            Toast.makeText(this,
                getString(R.string.required_field_empty),
                Toast.LENGTH_SHORT).show()
            return
        }

        builder.setPositiveButton(getString(R.string.yes)) { dialog, _ ->
            serviceDescr.name = serviceNameTB.text.toString()
            serviceDescr.conf_id = configurationSpinner.selectedItem.toString()

            if (Implementation.editService(
                    this@SetupService, m_service.toString(), serviceDescr)) {
                Toast.makeText(
                    this,
                    getString(R.string.service_updated_successfull),
                    Toast.LENGTH_SHORT
                ).show()
                dialog.dismiss()
                finish()
            }
            else {
                dialog.dismiss()
            }
        }

        builder.setNegativeButton(getString(R.string.no)) { dialog, _ ->
            dialog.dismiss()
        }

        val alertDialog = builder.create()
        alertDialog.show()
    }

    fun onDelete(view: View?) {
        val builder = AlertDialog.Builder(this)

        builder.setTitle(getString(R.string.warning))
        builder.setMessage(getString(R.string.want_continue))

        builder.setPositiveButton(getString(R.string.yes)) { dialog, _ ->
            if (Implementation.deleteService(this@SetupService, m_service.toString())) {
                Toast.makeText(
                    this,
                    getString(R.string.service_deleted_successfull),
                    Toast.LENGTH_SHORT
                ).show()
                dialog.dismiss()
                finish()
            }
            else {
                dialog.dismiss()
            }
        }

        builder.setNegativeButton(getString(R.string.no)) { dialog, _ ->
            dialog.dismiss()
        }

        val alertDialog = builder.create()
        alertDialog.show()
    }
}