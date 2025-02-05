package com.science.keysaver

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.ImageButton
import android.widget.Spinner
import android.widget.Toast
import androidx.constraintlayout.widget.Group
import androidx.lifecycle.lifecycleScope
import com.science.keysaver.databinding.EnterMasterKeyBinding
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

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

    override fun onDestroy() {
        super.onDestroy()

        Implementation.syncDB()
        Implementation.close()
    }

    fun onConfirmMasterPassword(view: View?) {
        val mpInput : EditText = findViewById(R.id.master_password)
        val masterPassword = mpInput.text.toString()

        val loadBar : Group = findViewById(R.id.wait_db)
        val functional : Group = findViewById(R.id.first_layout_functional)

        loadBar.visibility = View.VISIBLE
        functional.visibility = View.INVISIBLE

        var result : KeysaverStatus;
        lifecycleScope.launch {
            withContext(Dispatchers.IO) {
                result = Implementation.setMasterPassword(masterPassword)
            }

            if (!result.isSuccess()) {
                Toast.makeText(this@MainActivity,
                    result.getDescription(this@MainActivity),
                    Toast.LENGTH_SHORT).show()
            }
            else {
                setContentView(R.layout.get_password)
                prepareGetPassword()
            }
            loadBar.visibility = View.INVISIBLE
            functional.visibility = View.VISIBLE
        }
    }

    fun onSetupService(view: View?) {
        val selectedService = findViewById<Spinner>(R.id.selected_service).selectedItem.toString()

        val intent = Intent(this, SetupService::class.java)
        intent.putExtra("service", selectedService)
        startActivity(intent)
    }
}