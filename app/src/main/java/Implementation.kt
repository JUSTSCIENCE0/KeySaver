package com.science.keysaver

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.IBinder
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.EditText
import android.widget.ImageButton
import android.widget.Spinner
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.core.app.NotificationCompat
import java.io.File
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

fun isEmpty(textBox: EditText) : Boolean {
    return textBox.text.toString().trim().isEmpty()
}

class BackgroundDBUpdate : Service() {
    override fun onCreate() {
        super.onCreate()
        startForeground(1, createNotification())
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        super.onStartCommand(intent, flags, startId)
        return START_STICKY
    }

    override fun onBind(intent: Intent?): IBinder? { return null }

    override fun onTaskRemoved(rootIntent: Intent?) {
        super.onTaskRemoved(rootIntent)

        Implementation.syncDB()
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

class ConfigurationDescr(
    var conf_id:             String,
    var length:              Int,
    var use_upper:           Boolean,
    var use_lower:           Boolean,
    var alphabet:            String,
    var use_special_chars:   Boolean,
    var special_chars_count: Int,
    var special_charset:     String,
    var use_digits:          Boolean,
    var digits_amount:       Int
)

class Implementation private constructor() {
    private class IntWrapper(var value: Int)
    private class StringWrapper(var value: String);

    private external fun keysaverInit(configPath: String): Int
    private external fun keysaverClose(): Int
    private external fun keysaverSetMasterPassword(masterPassword: String): Int
    private external fun keysaverGetServicesCount(servicesCount: IntWrapper): Int
    private external fun keysaverGetServicesList(servicesList: Array<String?>): Int
    private external fun keysaverGetService(serviceName: String, service: ServiceDescr): Int
    private external fun keysaverGetConfigurationsCount(configurationsCount: IntWrapper): Int
    private external fun keysaverGetConfigurationsList(configurationsList: Array<String?>): Int
    private external fun keysaverGetAlphabetsCount(alphabetsCount: IntWrapper): Int
    private external fun keysaverGetAlphabetsList(alphabetsList: Array<String?>): Int
    private external fun keysaverAddService(service: ServiceDescr): Int
    private external fun keysaverDeleteService(serviceName: String): Int
    private external fun keysaverEditService(oldServiceName: String, newService: ServiceDescr): Int
    private external fun keysaverAddConfiguration(confDescr: ConfigurationDescr): Int
    private external fun keysaverSyncDatabase(): Int
    private external fun keysaverGeneratePassword(
        serviceName: String, imageIndex: Int, result: StringWrapper): Int
    private external fun keysaverGetDatabaseName(fileName: StringWrapper): Int

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

        private fun readConfigurationsList(context: Context) : Array<String?> {
            val configurationsCount = IntWrapper(0)
            var result = KeysaverStatus.fromCode(
                impl.keysaverGetConfigurationsCount(configurationsCount))
            if (result.isError()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return arrayOfNulls<String>(1)
            }

            val configurationsList = arrayOfNulls<String>(configurationsCount.value + 1)

            if (configurationsCount.value > 0) {
                result = KeysaverStatus.fromCode(
                    impl.keysaverGetConfigurationsList(configurationsList))
                if (result.isError()) {
                    Toast.makeText(
                        context,
                        result.getDescription(context),
                        Toast.LENGTH_SHORT
                    ).show()
                    return arrayOfNulls<String>(1)
                }
            }

            return configurationsList
        }

        private fun updateDBAsync(context: Context) {
            val serviceIntent = Intent(context, BackgroundDBUpdate::class.java)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                context.startForegroundService(serviceIntent)
            } else {
                context.startService(serviceIntent)
            }
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
            val configurationsList = readConfigurationsList(context)

            val addOptionText = context.getString(R.string.add_smth)
            configurationsList[configurationsList.size - 1] = addOptionText

            val configurationAdapter = ArrayAdapter(
                context,
                android.R.layout.simple_spinner_item,
                configurationsList)
            configurationAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            spinner.adapter = configurationAdapter
        }

        fun fillAlphabetsList(context: Context, spinner: Spinner) {
            val alphabetsCount = IntWrapper(0)
            var result = KeysaverStatus.fromCode(impl.keysaverGetAlphabetsCount(alphabetsCount))
            if (result.isError()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return
            }

            val alphabetsList = arrayOfNulls<String>(alphabetsCount.value)

            if (alphabetsCount.value > 0) {
                result = KeysaverStatus.fromCode(impl.keysaverGetAlphabetsList(alphabetsList))
                if (result.isError()) {
                    Toast.makeText(
                        context,
                        result.getDescription(context),
                        Toast.LENGTH_SHORT
                    ).show()
                    return
                }
            }
            val alphabetsAdapter = ArrayAdapter(
                context,
                android.R.layout.simple_spinner_item,
                alphabetsList)
            alphabetsAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            spinner.adapter = alphabetsAdapter
        }

        fun getConfigurationIndex(context: Context, confName: String) : Int {
            val configurationsList = readConfigurationsList(context)
            return configurationsList.indexOf(confName)
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

            updateDBAsync(context)
            return true
        }

        fun deleteService(context: Context, serviceName: String) : Boolean {
            val result = KeysaverStatus.fromCode(impl.keysaverDeleteService(serviceName))
            if (!result.isSuccess()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return false
            }

            updateDBAsync(context)
            return true
        }

        fun editService(context: Context, serviceName: String, newService: ServiceDescr) : Boolean {
            val result = KeysaverStatus.fromCode(impl.keysaverEditService(serviceName, newService))
            if (!result.isSuccess()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return false
            }

            updateDBAsync(context)
            return true
        }

        fun addConfiguration(context: Context, config: ConfigurationDescr) : Boolean {
            val result = KeysaverStatus.fromCode(impl.keysaverAddConfiguration(config))
            if (!result.isSuccess()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return false
            }

            updateDBAsync(context)
            return true
        }

        fun getService(context: Context, serviceName: String) : ServiceDescr? {
            val resultService = ServiceDescr("", "", "")
            val result = KeysaverStatus.fromCode(
                impl.keysaverGetService(serviceName, resultService))
            if (!result.isSuccess()) {
                Toast.makeText(context,
                    result.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return null
            }

            return resultService
        }

        fun syncDB() : KeysaverStatus {
            return KeysaverStatus.fromCode(impl.keysaverSyncDatabase())
        }

        fun generatePassword(context: Context, serviceName: String, imageIndex: Int) : String {
            val result = StringWrapper("")

            val code = KeysaverStatus.fromCode(
                impl.keysaverGeneratePassword(serviceName, imageIndex, result))
            if (!code.isSuccess()) {
                Toast.makeText(context,
                    code.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return "error"
            }

            return result.value
        }

        fun shareDataBase(context: Context, saveFile: Uri) {
            var code = KeysaverStatus.fromCode(impl.keysaverSyncDatabase())
            if (!code.isSuccess()) {
                Toast.makeText(context,
                    code.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return
            }

            val dbName = StringWrapper("")
            code = KeysaverStatus.fromCode(impl.keysaverGetDatabaseName(dbName))
            if (!code.isSuccess()) {
                Toast.makeText(context,
                    code.getDescription(context),
                    Toast.LENGTH_SHORT).show()
                return
            }

            val dataBasePath = context.filesDir.absolutePath + dbName.value
            val sourceFile = File(dataBasePath)

            context.contentResolver.openOutputStream(saveFile)?.use { output ->
                sourceFile.inputStream().use { input ->
                    input.copyTo(output)
                }
            }

            Toast.makeText(context,
                context.getString(R.string.file_saved) + saveFile.path.toString(),
                Toast.LENGTH_SHORT).show()
        }

        private val impl : Implementation = Implementation()

        public const val DEFAULT_PASSWORD_LENGTH = 16
        public const val DEFAULT_SPECIAL_CHARS_COUNT = 2
        public const val DEFAULT_DIGITS_COUNT = 2

        init {
            System.loadLibrary("keysaver-core")
        }
    }
}