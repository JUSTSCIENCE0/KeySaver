package com.science.keysaver

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Context
import android.content.Intent
import android.os.Build
import android.os.IBinder
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.ImageButton
import android.widget.Spinner
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.core.app.NotificationCompat
import kotlin.reflect.KClass

enum class KeysaverStatus(val code: Int) {
    // Success
    S_OK(0),

    // Messages
    M_DATABASE_NOT_FOUND(1),

    // Errors
    E_INVALID_MASTER_PASSWORD(-1),
    E_TOO_SHORT_MASTER_PASSWORD(-2),
    E_SERVICE_ALREADY_EXISTS(-3),
    E_SERVICE_URL_ALREADY_EXISTS(-4),
    E_CONFIG_ALREADY_EXISTS(-5),

    UNKNOWN(Int.MAX_VALUE);

    companion object {
        fun fromCode(code: Int): KeysaverStatus {
            return entries.find { it.code == code } ?: UNKNOWN
        }
    }

    fun getDescription(context: Context): String {
        return when (this) {
            S_OK -> "ok"
            E_TOO_SHORT_MASTER_PASSWORD -> context.getString(R.string.short_master_password)
            E_INVALID_MASTER_PASSWORD -> context.getString(R.string.invalid_master_password)
            E_SERVICE_ALREADY_EXISTS -> context.getString(R.string.service_already_exists)
            E_SERVICE_URL_ALREADY_EXISTS -> context.getString(R.string.service_url_already_exists)
            E_CONFIG_ALREADY_EXISTS -> context.getString(R.string.config_already_exists)
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

class BackgroundDBUpdate : Service() {
    override fun onCreate() {
        super.onCreate()
        startForeground(1, createNotification())
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        Thread {
            Implementation.syncDB()
            stopSelf()
        }.start()

        return START_NOT_STICKY
    }

    override fun onBind(intent: Intent?): IBinder? { return null }

    override fun onTaskRemoved(rootIntent: Intent?) {
        super.onTaskRemoved(rootIntent)

        Implementation.close()
        stopSelf()
    }

    private fun createNotification(): Notification {
        val channelId = "keysaver_db_update"
        val manager = getSystemService(NotificationManager::class.java)

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(channelId, "Keysaver DB Update", NotificationManager.IMPORTANCE_LOW)
            manager.createNotificationChannel(channel)
        }

        return NotificationCompat.Builder(this, channelId)
            .setContentTitle("Keysaver DB Update")
            .setContentText("Synchronize database...")
            .build()
    }
}

class ServiceDescr(
    var url:     String,
    var name:    String,
    var conf_id: String)

class Implementation private constructor() {
    private class IntWrapper(var value: Int)

    private external fun keysaverInit(configPath: String): Int
    private external fun keysaverClose(): Int
    private external fun keysaverSetMasterPassword(masterPassword: String): Int
    private external fun keysaverGetServicesCount(servicesCount: IntWrapper): Int
    private external fun keysaverGetServicesList(servicesList: Array<String?>): Int
    private external fun keysaverGetConfigurationsCount(configurationsCount: IntWrapper): Int
    private external fun keysaverGetConfigurationsList(configurationsList: Array<String?>): Int
    private external fun keysaverAddService(service: ServiceDescr): Int
    private external fun keysaverSyncDatabase(): Int

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
            val servicesCount = IntWrapper(0)
            var result = KeysaverStatus.fromCode(impl.keysaverGetServicesCount(servicesCount))
            if (result.isError()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                servicesCount.value = 0
            }

            var servicesList = arrayOfNulls<String>(servicesCount.value + 1)

            if (servicesCount.value > 0) {
                result = KeysaverStatus.fromCode(impl.keysaverGetServicesList(servicesList))
                if (result.isError()) {
                    Toast.makeText(
                        context,
                        result.getDescription(context),
                        Toast.LENGTH_SHORT
                    ).show()
                    servicesCount.value = 0
                    servicesList = arrayOfNulls<String>(1)
                }
            }

            val addOptionText = context.getString(R.string.add_smth)
            servicesList[servicesCount.value] = addOptionText

            val serviceAdapter = ArrayAdapter(
                context,
                android.R.layout.simple_spinner_item,
                servicesList)
            serviceAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            spinner.adapter = serviceAdapter
        }

        fun fillConfigurationsList(context: Context, spinner: Spinner) {
            val configurationsCount = IntWrapper(0)
            var result = KeysaverStatus.fromCode(
                impl.keysaverGetConfigurationsCount(configurationsCount))
            if (result.isError()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                configurationsCount.value = 0
            }

            var configurationsList = arrayOfNulls<String>(configurationsCount.value + 1)

            if (configurationsCount.value > 0) {
                result = KeysaverStatus.fromCode(
                    impl.keysaverGetConfigurationsList(configurationsList))
                if (result.isError()) {
                    Toast.makeText(
                        context,
                        result.getDescription(context),
                        Toast.LENGTH_SHORT
                    ).show()
                    configurationsCount.value = 0
                    configurationsList = arrayOfNulls<String>(1)
                }
            }

            val addOptionText = context.getString(R.string.add_smth)
            configurationsList[configurationsCount.value] = addOptionText

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

            if (KeysaverStatus.M_DATABASE_NOT_FOUND == result) {
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

        fun close() { impl.keysaverClose() }

        fun setMasterPassword(password: String) : KeysaverStatus {
            // TODO: validate master password

            return KeysaverStatus.fromCode(impl.keysaverSetMasterPassword(password))
        }

        fun addService(context: Context, service: ServiceDescr) : Boolean {
            val result = KeysaverStatus.fromCode(impl.keysaverAddService(service))
            if (!result.isSuccess()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return false
            }

            // async update DB file
            val serviceIntent = Intent(context, BackgroundDBUpdate::class.java)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                context.startForegroundService(serviceIntent)
            } else {
                context.startService(serviceIntent)
            }

            return true
        }

        fun syncDB() : KeysaverStatus {
            return KeysaverStatus.fromCode(impl.keysaverSyncDatabase())
        }

        private val impl : Implementation = Implementation()

        init {
            System.loadLibrary("keysaver-core")
        }
    }
}