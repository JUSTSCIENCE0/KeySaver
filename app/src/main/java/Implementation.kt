package com.science.keysaver

import android.content.Context
import android.content.Intent
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.ImageButton
import android.widget.Spinner
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import kotlin.reflect.KClass

enum class KeysaverStatus(val code: Int) {
    // Success
    S_OK(0),

    // Messages
    M_CONFIG_NOT_FOUND(1),

    // Errors
    E_NOT_IMPLEMENTED(-1),
    E_NOT_INITIALIZED(-2),
    TOO_SHORT_MASTER_PASSWORD(-3),
    INVALID_MASTER_PASSWORD(-4),

    UNKNOWN(Int.MAX_VALUE);

    companion object {
        fun fromCode(code: Int): KeysaverStatus {
            return entries.find { it.code == code } ?: UNKNOWN
        }
    }

    fun getDescription(context: Context): String {
        return when (this) {
            S_OK -> "ok"
            TOO_SHORT_MASTER_PASSWORD -> context.getString(R.string.short_master_password)
            INVALID_MASTER_PASSWORD -> context.getString(R.string.invalid_master_password)
            else -> context.getString(R.string.unknown_error)
        }
    }

    fun isSuccess() : Boolean {
        return when (this) {
            S_OK -> true
            else -> false
        }
    }

    fun isError() : Boolean {
        return code < 0
    }
}

class Implementation private constructor() {
    private external fun keysaverInit(configPath: String) : Int
    private external fun keysaverSetMasterPassword(masterPassword: String): Int

    companion object {
        private fun showWelcomeMessage(context: Context) {
            val dialog = AlertDialog.Builder(context)
                .setTitle(context.getString(R.string.welcome))
                .setMessage(context.getString(R.string.welcome_message))
                .setPositiveButton("OK") { dialog, _ ->
                    dialog.dismiss()
                }
                .create()

            dialog.show()
        }

        fun fillServicesList(context: Context, spinner: Spinner) {
            val servicesList = mutableListOf<String>()

            // TODO: fill list from db
            servicesList.add("google.com")
            servicesList.add("yandex.ru")

            val addOptionText = context.getString(R.string.add_smth)
            servicesList.add(addOptionText)

            val serviceAdapter = ArrayAdapter(
                context,
                android.R.layout.simple_spinner_item,
                servicesList)
            serviceAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            spinner.adapter = serviceAdapter
        }

        fun fillConfigurationsList(context: Context, spinner: Spinner) {
            val configurationsList = mutableListOf<String>()

            // TODO: fill list from db
            configurationsList.add("Default")
            configurationsList.add("conf1")
            configurationsList.add("cons2")

            val addOptionText = context.getString(R.string.add_smth)
            configurationsList.add(addOptionText)

            val configurationAdapter = ArrayAdapter(
                context,
                android.R.layout.simple_spinner_item,
                configurationsList)
            configurationAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            spinner.adapter = configurationAdapter
        }

        fun <T : Any> setupItemSelection(
            context: Context,
            spinner: Spinner,
            newActivity: KClass<T>,
            setupButton: ImageButton?) {
            val addOptionText = context.getString(R.string.add_smth)
            spinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
                override fun onItemSelected(
                    parentView: AdapterView<*>?,
                    view: View?,
                    position: Int, id: Long) {
                    val selectedItem = parentView?.getItemAtPosition(position).toString()

                    if (selectedItem == addOptionText) {
                        if (null != setupButton)
                            setupButton.visibility = View.GONE

                        val intent = Intent(context, newActivity.java)
                        context.startActivity(intent)
                    }
                    else {
                        if (null != setupButton)
                            setupButton.visibility = View.VISIBLE
                    }
                }

                override fun onNothingSelected(parentView: AdapterView<*>?) {
                    Toast.makeText(
                        context,
                        "Nothing selected",
                        Toast.LENGTH_SHORT).show()
                }
            }
        }

        fun init(context: Context) : Boolean {
            val filesDirPath = context.filesDir.absolutePath
            val result = KeysaverStatus.fromCode(impl.keysaverInit(filesDirPath))
            if (result.isSuccess()) return true

            if (result.isError()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return false
            }

            if (KeysaverStatus.M_CONFIG_NOT_FOUND == result) {
                showWelcomeMessage(context)
                return true
            }

            Toast.makeText(
                context,
                context.getString(R.string.unknown_error),
                Toast.LENGTH_SHORT
            ).show()
            return false
        }

        fun setMasterPassword(context: Context, password: String) : Boolean {
            // TODO: validate master password

            val result = KeysaverStatus.fromCode(impl.keysaverSetMasterPassword(password))
            if (!result.isSuccess()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return false
            }

            return true
        }

        private val impl : Implementation = Implementation()

        init {
            System.loadLibrary("keysaver-core")
        }
    }
}