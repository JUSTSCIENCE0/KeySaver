package com.science.keysaver

import android.content.Context
import android.content.Intent
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.ImageButton
import android.widget.Spinner
import android.widget.Toast
import kotlin.reflect.KClass

enum class KeysaverError(val code: Int) {
    OK(0),

    TOO_SHORT_MASTER_PASSWORD(-1),
    INVALID_MASTER_PASSWORD(-2),

    UNKNOWN(Int.MAX_VALUE);

    companion object {
        fun fromCode(code: Int): KeysaverError {
            return entries.find { it.code == code } ?: UNKNOWN
        }
    }

    fun getDescription(context: Context): String {
        return when (this) {
            OK -> "ok"
            TOO_SHORT_MASTER_PASSWORD -> context.getString(R.string.short_master_key)
            INVALID_MASTER_PASSWORD -> context.getString(R.string.invalid_master_key)
            else -> context.getString(R.string.unknown_error)
        }
    }
}

class Implementation private constructor() {
    private external fun keysaverSetMasterKey(unicodePassword: ShortArray): Int

    companion object {
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

        fun setMasterPassword(context: Context, password: String) : Boolean {
            val unicodeMP = Implementation.stringToUnicodeInt16Array(password)

            val result = impl.keysaverSetMasterKey(unicodeMP)
            if (0 != result) {
                Toast.makeText(context,
                    KeysaverError.fromCode(result).getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return false
            }

            return true
        }

        private fun stringToUnicodeInt16Array(input: String): ShortArray {
            return input.map { it.code.toShort() }.toShortArray()
        }

        private val impl : Implementation = Implementation()

        init {
            System.loadLibrary("keysaver-core")
        }
    }
}