from enum import Enum
import ctypes
import os

class KeysaverStatus (Enum):
    #SUCCESS
    S_OK = 0
    S_IS_FOUND = 0

    # Messages
    M_DATABASE_NOT_FOUND = 1

    # Errors
    E_INVALID_MASTER_PASSWORD    = -1
    E_TOO_SHORT_MASTER_PASSWORD  = -2
    E_SERVICE_ALREADY_EXISTS     = -3
    E_SERVICE_URL_ALREADY_EXISTS = -4
    E_CONFIG_ALREADY_EXISTS      = -5
    E_INVALID_PASSWORD_LENGTH    = -6
    E_INVALID_SPECIAL_CHAR_COUNT = -7
    E_INVALID_DIGITS_AMOUNT      = -8
    E_WITHOUT_ANY_CASE           = -9

    # Internal Errors
    E_NOT_IMPLEMENTED       = -1000
    E_NOT_INITIALIZED       = -1001
    E_INTERNAL_OPENSSL_FAIL = -1002
    E_INVALID_ARG           = -1003
    E_SERVICE_NOT_EXISTS    = -1004
    E_CONFIG_NOT_EXISTS     = -1005
    E_DB_WRITE_ERROR        = -1006
    E_DB_READ_ERROR         = -1007
    E_DB_CORRUPTED          = -1008
    E_UNSUPPORTED_ALPHABET  = -1009
    E_UNEXPECTED_EXCEPTION  = -1010
    E_INVALID_ORDER         = -1011
    E_TOO_LONG_STRING       = -1012

class KeysaverService(ctypes.Structure):
    _max_size_of_string = 100
    _fields_ = [
        ("url", ctypes.c_char  * _max_size_of_string),
        ("name", ctypes.c_char  * _max_size_of_string),
        ("conf_id", ctypes.c_char  * _max_size_of_string),
    ]

    # service = KeysaverService(url=r"https://example.com",
    #                           name=r"My Service",
    #                           conf_id=r"Default")

    def __init__(self, url="", name="", conf_id=""):
        super().__init__()
        self.url = url.encode("utf-8")[:self._max_size_of_string - 1]
        self.name = name.encode("utf-8")[:self._max_size_of_string - 1]
        self.conf_id = conf_id.encode("utf-8")[:self._max_size_of_string - 1]

        self.url += b'\x00' * (self._max_size_of_string - len(self.url))
        self.name += b'\x00' * (self._max_size_of_string - len(self.name))
        self.conf_id += b'\x00' * (self._max_size_of_string - len(self.conf_id))

    def __eq__(self, other):
        if not isinstance(other, KeysaverService):
            return NotImplemented
        return (
                self.url[:] == other.url[:] and
                self.name[:] == other.name[:] and
                self.conf_id[:] == other.conf_id[:]
        )

class KeysaverConfiguration(ctypes.Structure):
    """    Название конфига строится из conf_+ 'length'+_+'use_upper'+...
            Для  use_upper, use_lower, use_special_chars, use_digits - используются True или False


    """

    _fields_ = [
        ("id_name", ctypes.c_char_p),
        ("length", ctypes.c_uint),
        ("use_upper", ctypes.c_bool),
        ("use_lower", ctypes.c_bool),
        ("alphabet", ctypes.c_int),
        ("use_special_chars", ctypes.c_bool),
        ("special_chars_count", ctypes.c_uint),
        ("special_charset", ctypes.c_char_p),
        ("use_digits", ctypes.c_bool),
        ("digits_amount", ctypes.c_uint),
    ]

    # config = KeysaverConfiguration(
    #                                 id_name=r"Новый_тест_конфиг",
    #                                length=20,
    #                                use_upper=True,
    #                                use_lower=True,
    #                                alphabet=1,
    #                                use_special_chars=True,
    #                                special_chars_count=3,
    #                                special_charset=r".`,;",
    #                                use_digits=True,
    #                                digits_amount=3)

    def __init__(
            self,
            id_name="",
            length=0,
            use_upper=False,
            use_lower=False,
            alphabet=0,
            use_special_chars=False,
            special_chars_count=0,
            special_charset="",
            use_digits=False,
            digits_amount=0
    ):
        super().__init__()
        self.id_name = id_name.encode("utf-8") if id_name else None
        self.length = length
        self.use_upper = use_upper
        self.use_lower = use_lower
        self.alphabet = alphabet
        self.use_special_chars = use_special_chars
        self.special_chars_count = special_chars_count
        self.special_charset = special_charset.encode("utf-8") if special_charset else None
        self.use_digits = use_digits
        self.digits_amount = digits_amount

    def __eq__(self, other):
        if not isinstance(other, KeysaverConfiguration):
            return NotImplemented
        return (
                self.id_name[:] == other.id_name[:] and
                self.length == other.length and
                self.use_upper == other.use_upper and
                self.use_lower == other.use_lower and
                self.alphabet == other.alphabet and
                self.use_special_chars == other.use_special_chars and
                self.special_chars_count == other.special_chars_count and
                self.special_charset[:] == other.special_charset[:] and
                self.use_digits == other.use_digits and
                self.digits_amount == other.digits_amount
        )

class KeysaverImplementation:
    def __init__(self, path_to_dll, path_to_config_dir, master_password):
        self.path_to_dll = path_to_dll
        self.path_to_config_dir = path_to_config_dir
        self.max_size_of_string = 100
        self.windows_lib = ctypes.CDLL(os.path.abspath(path_to_dll))
        self.master_password = master_password

        """
        path_to_dll - путь до C DLL
        path_to_config_dir: путь до каталога с конфигами(БД)
        max_size_of_string = максимальная длина строки
        windows_lib: это ctypes.CDLL(os.path.abspath(path_to_dll))
        master_password  - мастер-ключ для инициализации и расшифрования БД
        """

    def Init(self):
        """
        :param need_close: надо ли вызвать Close, по дефолту True - вызов Close
        """
        self.windows_lib.keysaverInit.argtypes = [ctypes.c_char_p]
        self.windows_lib.keysaverInit.restype = ctypes.c_int
        result = self.windows_lib.keysaverInit(self.path_to_config_dir.encode("utf-8"))
        print("Result of Init:", result)
        return result

    def Close(self):

        self.windows_lib.keysaverClose.restype = ctypes.c_int
        result = self.windows_lib.keysaverClose()
        print("Result of Close:", result)
        return result

    def SetMasterPassword(self):
        """
        расшифровывает БД из path_to_config_dir
        """
        self.windows_lib.keysaverSetMasterPassword.argtypes = [ctypes.c_char_p]
        self.windows_lib.keysaverSetMasterPassword.restype = ctypes.c_int
        result = self.windows_lib.keysaverSetMasterPassword(self.master_password.encode("utf-8"))
        print("SetMasterPassword return code:", result)
        return result

    def AddService(self, service):
        """
        добавляет сервис в БД по path_to_config_dir
        на вход нужен экземпляр класса KeysaverService
        """

        self.windows_lib.keysaverAddService.argtypes = [KeysaverService]
        self.windows_lib.keysaverAddService.restype = ctypes.c_int
        result = self.windows_lib.keysaverAddService(service)
        print("Result of AddService:", result)
        return result

    def DeleteService(self, service_name):
        """
           удаляет сервис из БД по path_to_config_dir
           на вход нужно название сервиса на удаление - пример: r"youtube"
        """
        self.windows_lib.keysaverDeleteService.argtypes = [ctypes.c_char_p]
        self.windows_lib.keysaverDeleteService.restype = ctypes.c_int
        result = self.windows_lib.keysaverDeleteService(service_name.encode("utf-8"))
        print("DeleteService return code:", result)
        return result

    def SyncDatabase(self):
        """
        записывает изменения в БД по path_to_config_dir
        """
        self.windows_lib.keysaverSyncDatabase.restype = ctypes.c_int
        result = self.windows_lib.keysaverSyncDatabase()
        print("Result of DB sync", result)
        return result

    def EditService(self, old_service_name, service):
        """
        изменяет сервис в БД по path_to_config_dir
        на вход нужен экземпляр класса KeysaverService и название сервиса
        на изменение - пример: r"youtube"
        """
        self.windows_lib.keysaverEditService.argtypes = [ctypes.c_char_p, KeysaverService]
        self.windows_lib.keysaverEditService.restype = ctypes.c_int
        result = self.windows_lib.keysaverEditService(old_service_name.encode("utf-8"), service)
        print("Result of EditService:", result)
        return result

    def AddConfiguration(self, configuration):
        """
            изменяет конфигурацию в БД по path_to_config_dir
            на вход нужен экземпляр класса KeysaverConfiguration
        """
        self.windows_lib.keysaverAddConfiguration.argtypes = [KeysaverConfiguration]
        self.windows_lib.keysaverAddConfiguration.restype = ctypes.c_int
        result = self.windows_lib.keysaverAddConfiguration(configuration)
        print("Result of AddConfiguration:", result)
        return result

    def GetService(self, service_name):
        """
            возвращает сервис из БД по path_to_config_dir
            на вход надо название сервиса на изменение - пример: r"youtube"
            возвращает экземпляр класса KeysaverService
        """
        self.windows_lib.keysaverGetService.argtypes = [ctypes.c_char_p, ctypes.POINTER(KeysaverService)]
        self.windows_lib.keysaverGetService.restype = ctypes.c_int

        service = KeysaverService()
        service_ptr = ctypes.pointer(service)

        result = self.windows_lib.keysaverGetService(service_name.encode("utf-8"), service_ptr)
        print("Result of GetService:", result)
        return result, service

    def GetDatabaseName(self):
        """
        возвращает текущее имя файла ДБ из path_to_config_dir
        записывает его по db_name
        """
        self.windows_lib.keysaverGetDatabaseName.argtypes = [ctypes.c_char_p]
        self.windows_lib.keysaverGetDatabaseName.restype = ctypes.c_int
        result_buffer = ctypes.create_string_buffer(self.max_size_of_string)
        result_buffer = ctypes.cast(result_buffer, ctypes.c_char_p)

        result = self.windows_lib.keysaverGetDatabaseName(self.path_to_config_dir.encode("utf-8"), result_buffer)
        print("Result of GetDatabaseName:", result)

        return result, result_buffer

    def GeneratePassword(self, serviceName, imageIndex):
        """
        Генерация пароля для serviceName и imageIndex
        :param serviceName: строка r"youtube"
        :param imageIndex: int
        """
        self.windows_lib.keysaverGeneratePassword.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p]
        self.windows_lib.keysaverGeneratePassword.restype = ctypes.c_int

        result_buffer = ctypes.create_string_buffer(self.max_size_of_string)
        result_buffer = ctypes.cast(result_buffer, ctypes.c_char_p)
        result = self.windows_lib.keysaverGeneratePassword(serviceName.encode("utf-8"), imageIndex, result_buffer)
        print("Result of GeneratePassword:", result)
        return result, result_buffer.value

    def call_function_GetAttributeCount(self, attribute):
        """
        call_function для GetServicesCount,GetConfigurationsCount и GetAlphabetsCount
        attribute - 'Services', 'Configurations' или 'Alphabets' соответственно
        возвращает число int
        """
        func_name = 'keysaverGet'+attribute+'Count'
        function = getattr(self.windows_lib, func_name)
        function.argtypes = [ctypes.POINTER(ctypes.c_int)]
        function.restype = ctypes.c_int

        value = ctypes.c_int()
        result = function(ctypes.byref(value))
        print("Result of "+func_name+":", result)
        print("Count:", value.value)
        return result, value.value

    def call_function_GetAttributeList(self, attribute):
        """
            call_function для GetServicesList,GetConfigurationsList и GetAlphabetsList
            attribute - 'Services', 'Configurations' или 'Alphabets' соответственно
            возвращает list[]
        """
        item_count = self.call_function_GetAttributeCount(attribute)
        config_list = [ctypes.create_string_buffer(self.max_size_of_string) for _ in range(item_count)]
        config_list_ptrs = (ctypes.c_char_p * item_count)(
            *[ctypes.cast(buf, ctypes.c_char_p) for buf in config_list])

        func_name = 'keysaverGet' + attribute + 'List'
        function = getattr(self.windows_lib, func_name)
        function.argtypes = [ctypes.POINTER(ctypes.c_char_p)]
        function.restype = ctypes.c_int
        result = function(config_list_ptrs)
        print("Result of "+func_name+":", result)

        if result == 0:
            print("Список:")
            return_list =[]
            for s in config_list:
                if s:  # Проверка на NULL
                    s = bytes(s).split(b'\x00', 1)[0].decode('utf-8')
                    return_list.append(s)
                    print(s)
            return return_list
        else:
            print(f"Ошибка: код {result}")


