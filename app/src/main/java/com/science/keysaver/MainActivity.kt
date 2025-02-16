package com.science.keysaver

import android.app.Activity
import android.content.ClipData
import android.content.ClipboardManager
import android.content.Context
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.ImageButton
import android.widget.Spinner
import android.widget.Toast
import androidx.activity.result.ActivityResultLauncher
import androidx.activity.result.contract.ActivityResultContracts
import androidx.constraintlayout.widget.Group
import androidx.lifecycle.lifecycleScope
import com.science.keysaver.databinding.EnterMasterKeyBinding
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class MainActivity : AppCompatActivity() {

    private lateinit var binding: EnterMasterKeyBinding
    private lateinit var saveFileLauncher: ActivityResultLauncher<Intent>

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

        saveFileLauncher = registerForActivityResult(
            ActivityResultContracts.StartActivityForResult()) { result ->
            if (result.resultCode == Activity.RESULT_OK) {
                result.data?.data?.let { uri ->
                    Implementation.shareDataBase(this, uri)
                }
            }
        }

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

    fun onImageClick(view: View?) {
        if (null == view) return
        var imageNumber = -1

        when (view.id) {
            R.id.get_password_image_0 -> imageNumber = 0
            R.id.get_password_image_1 -> imageNumber = 1
            R.id.get_password_image_2 -> imageNumber = 2
            R.id.get_password_image_3 -> imageNumber = 3
            R.id.get_password_image_4 -> imageNumber = 4
            R.id.get_password_image_5 -> imageNumber = 5
            R.id.get_password_image_6 -> imageNumber = 6
            R.id.get_password_image_7 -> imageNumber = 7
            R.id.get_password_image_8 -> imageNumber = 8
        }

        val selectedService = findViewById<Spinner>(R.id.selected_service).selectedItem.toString()
        val password = Implementation.generatePassword(this, selectedService, imageNumber)

        val passwordEditText = findViewById<EditText>(R.id.get_password_result)
        passwordEditText.setText(password)
    }

    fun onCopyPassword(view: View?) {
        val passwordEditText = findViewById<EditText>(R.id.get_password_result)
        if (isEmpty(passwordEditText))
            return

        val clipboard = getSystemService(Context.CLIPBOARD_SERVICE) as ClipboardManager
        val clip = ClipData.newPlainText("copied_text", passwordEditText.text.toString())
        clipboard.setPrimaryClip(clip)
        Toast.makeText(this, getString(R.string.password_copied), Toast.LENGTH_SHORT).show()
    }

    fun onShareDataBase(view: View?) {
        val createFileIntent = Intent(Intent.ACTION_CREATE_DOCUMENT).apply {
            addCategory(Intent.CATEGORY_OPENABLE)
            type = "application/octet-stream"
            putExtra(Intent.EXTRA_TITLE, "database.bin")
        }
        saveFileLauncher.launch(createFileIntent)
    }
}