import pytest
from KeysaverInterface import KeysaverImplementation
from KeysaverInterface import KeysaverService
from KeysaverInterface import KeysaverConfiguration

#todo
"""

1. ENUM - для ошибок, переделать в кодах ошибок
2. Убрать русский текст
3. переделать get_service - Enum - на имена, URL и на конфиг(пока не придумано) 
4. Добавить комментарии к ошибкам (пример в test_get_services_count)
"""

path_to_dll=r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"
#todo проверка ОС
#todo для Линух проверка ОС+относительный путь

path_to_config_dir=r"D:\Class\KeySaver\Test_Windows_Linux\storage"
path_to_false_config_dir=r"C:\Users\Balzhit\Pictures\Camera Roll"

master_password= r"Test123."
false_master_password= r"Test1239999."
short_master_password= r"Test"

exist_service_url=r"https://example.com"
not_exist_service_url = r"https://porn_hub.com"

exist_service_name = r"test_service_0"
not_exist_service_name = r"My Service new"

exist_config_name = r"Default"
not_exist_config_name = r"Default NOT EXIST"

service_count = 5
config_count = 4
alphabet_count = 2

services_list = []
config_list = []

@pytest.fixture
#дефолтный keySaver с нормальными данными
def instance():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, master_password)

@pytest.fixture
# keySaver с неправильным путем до БД
def instance_false_DB():
    return KeysaverImplementation(path_to_dll, path_to_false_config_dir, master_password)

@pytest.fixture
# keySaver с неправильным паролем
def instance_false_password():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, false_master_password)

@pytest.fixture
# keySaver с коротким паролем
def instance_short_password():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, short_master_password)

def get_service(bool_url= True,bool_name = True, bool_config= True):
    """

    :param bool_url: True - если нужен существующий exist_service_url,
                            иначе -  not_exist_service_url
    :param bool_name: True - если нужен существующий exist_service_name,
                            иначе - not_exist_service_name
    :param bool_config: True - если нужен существующий r"Default",
                            иначе - r"Default NOT EXIST"
    :return: KeysaverService
    """
    return KeysaverService(url = exist_service_url if bool_url else not_exist_service_url,
                           name= exist_service_name if bool_name else not_exist_service_name,
                           conf_id= r"Default" if bool_config else r"Default NOT EXIST")

def get_configuration():

   return KeysaverConfiguration(
        id_name=r"conf_20_True_True_0_True_3_True_4",
        length=20,
        use_upper=True,
        use_lower=True,
        alphabet=0,
        use_special_chars=True,
        special_chars_count=3,
        special_charset=r"!@#$%^&*()_-+=/?.,<>\'\";:[]{}",
        use_digits=True,
        digits_amount=4)

#---------------------------------------------------------------------------------------------#
""" Блок тестов init"""

# def test_init(instance):
#     result = instance.Init()
#     assert result == 0, f"init failed with result {result}"
#
# def test_init_false_DB(instance_false_DB):
#     result = instance_false_DB.Init()
#     assert result == 1, f"init failed with result {result}"

def test_init_close(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.Close())
    assert result == [0, 0],  f"init or close failed with result {result}"
#
# def test_init_false_DB_close(instance_false_DB):
#     result = []
#     result.append(instance_false_DB.Init())
#     result.append(instance_false_DB.Close())
#     assert result == [1, 0],  f"init false_DB or close failed with result {result}"

#---------------------------------------------------------------------------------------------#
""" Блок тестов master_password"""

#крашатся, если нет CLOSE в предыдущем тесте. Вылет программы приводит к некорректной работе
# def test_init_set_master_password(instance):
#     result = []
#     result.append(instance.Init())
#     result.append(instance.SetMasterPassword())
#     assert result == [0, 0],  f"set_master_password failed with result {result}"

def test_init_set_master_password_close(instance):

    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.Close())
    assert result == [0, 0, 0],  f"set_master_password failed with result {result}"

def test_init_set_false_master_password_close(instance_false_password):
    result = []
    result.append(instance_false_password.Init())
    result.append(instance_false_password.SetMasterPassword())
    result.append(instance_false_password.Close())
    assert result == [0, -1, 0],  f"init, set_master_password false or close failed with result {result}"

def test_init_set_short_master_password_close(instance_short_password):
    result = []
    result.append(instance_short_password.Init())
    result.append(instance_short_password.SetMasterPassword())
    result.append(instance_short_password.Close())
    assert result == [0, -2, 0],  f"init, set_master_password short or close failed with result {result}"

#---------------------------------------------------------------------------------------------#

""" Блок тестов generate_password"""
#todo если БД поврежд и конфиг не найден - E_CONFIG_NOT_EXISTS
def test_generate_password(instance):

    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GeneratePassword(exist_service_name, 0)[0])
    result.append(instance.Close())
    assert result == [0, 0, 0, 0],  f"GeneratePassword failed with result {result}"

def test_generate_password_no_service(instance):

    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GeneratePassword(not_exist_service_url, 0)[0])
    result.append(instance.Close())
    assert result == [0, 0, -1004, 0],  f"GeneratePassword for not exist service failed with result {result}"

def test_generate_password_invalid_image(instance):

    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GeneratePassword(exist_service_name, 10)[0])
    result.append(instance.Close())
    assert result == [0, 0, -1003, 0],  f"GeneratePassword for not exist image failed with result {result}"

#---------------------------------------------------------------------------------------------#

""" Блок тестов AddService БЕЗ SYNC DATA BASE"""
#todo переделать get_service + проверить, все ли покрыла(не все)
def test_add_new_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(False, False, True)
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [0, 0, 0, 0],  f"AddService new failed with result {result}"

def test_add_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service()
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [0, 0, -3, 0],  f"AddService exist service failed with result {result}"

def test_add_exist_url_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(True, False, True)
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [0, 0, -4, 0],  f"AddService service with exist url failed with result {result}"

def test_add_new_service_with_not_exist_config(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(False, False, False)
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [0, 0, -1005, 0],  f"AddService service with config_not_found failed with result {result}"

#---------------------------------------------------------------------------------------------#

""" Блок тестов DeleteService БЕЗ SYNC DATA BASE"""

def test_delete_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.DeleteService(exist_service_name))
    result.append(instance.Close())
    assert result == [0, 0, 0, 0],  f"DeleteService for exist_service failed with result {result}"


def test_delete_not_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.DeleteService(not_exist_service_name))
    result.append(instance.Close())
    assert result == [0, 0, -1004, 0], f"DeleteService for not_exist_service failed with result {result}"

#---------------------------------------------------------------------------------------------#

""" Блок тестов EditService БЕЗ SYNC DATA BASE"""
#todo если переименуешь в уже существующее имя - E_SERVICE_ALREADY_EXISTS - отредачить 1 сервис в вид, похожий на другой существующий
def test_edit_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(False, False, True)
    result.append(instance.EditService(exist_service_name, service_identity))
    result.append(instance.Close())
    assert result == [0, 0, 0, 0], f"EditService for exist_service failed with result {result}"

def test_edit_not_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service()
    result.append(instance.EditService(not_exist_service_name, service_identity))
    result.append(instance.Close())
    assert result == [0, 0, -1004, 0], f"EditService for not_exist_service failed with result {result}"

def test_edit_service_not_exist_config(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(True, True, False)
    result.append(instance.EditService(exist_service_name, service_identity))
    result.append(instance.Close())
    assert result == [0, 0, -1005, 0], f"EditService for exist_service with not_exist_config failed with result {result}"

#---------------------------------------------------------------------------------------------#

""" Блок тестов call_function_GetAttributeCount """
""" пока что наделать с ними, не придумала"""
def test_get_services_count(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.call_function_GetAttributeCount('Services'))
    result.append(instance.Close())
    assert result == [
        0,  # instance.Init()
        0,  # instance.SetMasterPassword()
        (0, service_count),  # instance.call_function_GetAttributeCount('Services')
        0  # instance.Close()
    ], f"get_services_count  failed with result {result}"

def test_get_config_count(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.call_function_GetAttributeCount('Configurations'))
    result.append(instance.Close())
    assert result == [0, 0, (0, config_count), 0], f"get_config_count  failed with result {result}"

def test_get_alphabet_count(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.call_function_GetAttributeCount('Alphabets'))
    result.append(instance.Close())
    assert result == [0, 0, (0, alphabet_count), 0], f"get_alphabet_count  failed with result {result}"

#---------------------------------------------------------------------------------------------#

""" Блок тестов GetServices """
#todo ПЕРЕПИСАТЬ, сменились имена сервисов
def test_get_services(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService()
    result.append(instance.GetService(exist_service_name))
    result.append(instance.Close())
    assert result== [0, 0, (0, service_identity), 0], f"get_services failed with result {result}"

def test_get_not_exist_services(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService()
    result.append(instance.GetService(not_exist_service_name)[0])
    result.append(instance.Close())
    assert result== [0, 0, -1004, 0], f"get_services failed with result {result}"
    assert instance.GetService(exist_service_name)[1] == service_identity, \
                                        f"not_same url failed with result {result}"

#---------------------------------------------------------------------------------------------#

""" Блок тестов call_function_GetAttributeList"""
""" пока что наделать с ними, не придумала"""
#todo проверка списков сделать 1. пустая БД, в нее записывать. 2. проверить, что в бд есть

def test_get_services_list(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.call_function_GetAttributeList('Services')

    result.append(len(result_list))
    result.append(instance.Close())
    assert result == [0, 0, service_count, 0], f"get_services_list  failed with result {result}"


def test_get_config_list(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.call_function_GetAttributeList('Configurations')

    result.append(len(result_list))
    result.append(instance.Close())
    assert result == [0, 0, config_count, 0], f"get_config_list  failed with result {result}"

def test_get_alphabet_list(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.call_function_GetAttributeList('Alphabets')

    result.append(len(result_list))
    result.append(instance.Close())
    assert result == [0, 0, alphabet_count, 0], f"get_alphabet_list  failed with result {result}"

#---------------------------------------------------------------------------------------------#

""" Блок тестов GetDatabaseName"""
#todo если будет возвращать название, заменить result == [0, 0, 0, 0] на сравнение с названием
def test_get_exist_database_name(instance):
    """ не возвращается название файла"""
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.GetDatabaseName()

    result.append(result_list[0])
    print(result_list[1])
    result.append(instance.Close())
    assert result == [0, 0, 0, 0], f"get_exist_database_name  failed with result {result}"

#---------------------------------------------------------------------------------------------#

""" Блок тестов SyncDatabase"""

# def test_SyncDatabase(instance):
#
#---------------------------------------------------------------------------------------------#

""" Блок тестов AddConfiguration БЕЗ SYNC DATA BASE"""

def test_add_new_configuration(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    configuration_identity = get_configuration()
    result.append(instance.AddConfiguration(configuration_identity))
    result.append(instance.Close())
    assert result == [0, 0, 0, 0],  f"AddConfiguration new failed with result {result}"