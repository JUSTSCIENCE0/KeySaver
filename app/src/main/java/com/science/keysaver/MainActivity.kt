package com.science.keysaver

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.ImageButton
import android.widget.Spinner
import com.science.keysaver.databinding.EnterMasterKeyBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: EnterMasterKeyBinding

    private fun prepareGetPassword() {
        val selectedServiceSpinner: Spinner = findViewById(R.id.selected_service)
        val setupServiceButton: ImageButton = findViewById(R.id.setup_service_button)

        Implementation.fillServicesList(this, selectedServiceSpinner)
        Implementation.setupItemSelection(
            this,
            selectedServiceSpinner,
            AddService::class,
            setupServiceButton)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = EnterMasterKeyBinding.inflate(layoutInflater)
        setContentView(binding.root)

        Implementation.init(this)
    }

    override fun onResume() {
        super.onResume()

        val selectedServiceSpinner: Spinner? = findViewById(R.id.selected_service)
        if (selectedServiceSpinner != null) {
            Implementation.fillServicesList(this, selectedServiceSpinner)
        }
    }

    fun onConfirmMasterPassword(view: View?) {
        val mpInput : EditText = findViewById(R.id.master_password)
        val masterPassword = mpInput.text.toString()

        if (Implementation.setMasterPassword(this, masterPassword)) {
            setContentView(R.layout.get_password)
            prepareGetPassword()
        }
    }

    fun onSetupService(view: View?) {
        val selectedService = findViewById<Spinner>(R.id.selected_service).selectedItem.toString()

        val intent = Intent(this, SetupService::class.java)
        intent.putExtra("service", selectedService)
        startActivity(intent)
    }
}