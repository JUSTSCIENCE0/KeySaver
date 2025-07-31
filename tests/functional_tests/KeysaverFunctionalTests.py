import pytest
import platform
from KeysaverInterface import KeysaverImplementation
from KeysaverInterface import KeysaverConfiguration
from KeysaverInterface import KeysaverStatus as KS

if (platform.system() == 'Windows'):
    path_to_dll = r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"
else:
    path_to_dll = r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"  # Linux
# todo cheking for Linux + path

path_to_config_dir = r".\storage"
path_to_false_config_dir = r"..\\"

master_password = r"Test123."
false_master_password = r"Test1239999."
short_master_password = r"Test"

service_count = 5
config_count = 4
alphabet_count = 2

services_list = []
config_list = []


@pytest.fixture
# default keySaver with validate data
def instance():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, master_password)


@pytest.fixture
# keySaver with path = path_to_false_config_dir
def instance_false_DB():
    return KeysaverImplementation(path_to_dll, path_to_false_config_dir, master_password)


@pytest.fixture
# keySaver with false password
def instance_false_password():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, false_master_password)


@pytest.fixture
# keySaver with short password
def instance_short_password():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, short_master_password)


# ---------------------------------------------------------------------------------------------#
""" Test of init"""


# def test_init(instance):
#     result = instance.Init()
#     assert result == KS.S_OK.value,
#                       f"init was crashed"
#
# def test_init_false_DB(instance_false_DB):
#     result = instance_false_DB.Init()
#     assert result == KS.M_DATABASE_NOT_FOUND.value,
#                       f"init was crashed"

def test_init_close(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value  # instance.Close()
    ], f"init or close was crashed"


#
# def test_init_false_DB_close(instance_false_DB):
#     result = []
#     result.append(instance_false_DB.Init())
#     result.append(instance_false_DB.Close())
#     assert result == [
#                       KS.M_DATABASE_NOT_FOUND.value,  # instance_false_DB.Init()
#                       KS.S_OK.value                 # instance.Close()
#                       ], f"init false_DB or close was crashed"

# ---------------------------------------------------------------------------------------------#
""" Test of master_password"""


# крашатся, если нет CLOSE в предыдущем тесте. Вылет программы приводит к некорректной работе
# def test_init_set_master_password(instance):
#     result = []
#     result.append(instance.Init())
#     result.append(instance.SetMasterPassword())
#     assert result == [
#                       KS.S_OK.value,  # instance.Init()
#                       KS.S_OK.value # instance.SetMasterPassword()
#                       ], f"set_master_password was crashed"

def test_init_set_master_password_close(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        KS.S_OK.value  # instance.Close()
    ], f"set_master_password was crashed"


def test_init_set_false_master_password_close(instance_false_password):
    result = []
    result.append(instance_false_password.Init())
    result.append(instance_false_password.SetMasterPassword())
    result.append(instance_false_password.Close())
    assert result == [
        KS.S_OK.value,  # instance_false_password.Init()
        KS.E_INVALID_MASTER_PASSWORD.value,  # instance_false_password.SetMasterPassword()
        KS.S_OK.value  # instance_false_password.Close()
    ], f"init, set_master_password false or close was crashed"


def test_init_set_short_master_password_close(instance_short_password):
    result = []
    result.append(instance_short_password.Init())
    result.append(instance_short_password.SetMasterPassword())
    result.append(instance_short_password.Close())
    assert result == [
        KS.S_OK.value,  # instance_short_password.Init()
        KS.E_TOO_SHORT_MASTER_PASSWORD.value,  # instance_short_password.SetMasterPassword()
        KS.S_OK.value  # instance_short_password.Close()
    ], f"init, set_master_password short or close was crashed"


# ---------------------------------------------------------------------------------------------#

""" Test of call_function_GetAttributeCount """


def test_get_services_count(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.call_function_GetAttributeCount('Services'))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        (KS.S_OK.value, service_count),  # instance.call_function_GetAttributeCount('Services')
        KS.S_OK.value  # instance.Close()
    ], f"get_services_count  was crashed"


def test_get_config_count(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.call_function_GetAttributeCount('Configurations'))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        (KS.S_OK.value, config_count),  # instance.call_function_GetAttributeCount('Configurations')
        KS.S_OK.value  # instance.Close()
    ], f"get_config_count  was crashed"


def test_get_alphabet_count(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.call_function_GetAttributeCount('Alphabets'))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        (KS.S_OK.value, alphabet_count),  # instance.call_function_GetAttributeCount('Alphabets')
        KS.S_OK.value  # instance.Close()
    ], f"get_alphabet_count  was crashed"


# ---------------------------------------------------------------------------------------------#

""" Test of call_function_GetAttributeList"""
""" пока что наделать с ними, не придумала"""


# todo проверка списков сделать 1. пустая БД, в нее записывать. 2. проверить, что в бд есть

def test_get_services_list(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.call_function_GetAttributeList('Services')

    result.append(len(result_list))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        service_count,  # len(result_list)
        KS.S_OK.value  # instance.Close()
    ], f"get_services_list  was crashed"


def test_get_config_list(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.call_function_GetAttributeList('Configurations')

    result.append(len(result_list))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        config_count,  # len(result_list)
        KS.S_OK.value  # instance.Close()
    ], f"get_config_list  was crashed"


def test_get_alphabet_list(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.call_function_GetAttributeList('Alphabets')

    result.append(len(result_list))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        alphabet_count,  # len(result_list)
        KS.S_OK.value  # instance.Close()
    ], f"get_alphabet_list  was crashed"


# ---------------------------------------------------------------------------------------------#

""" Test of GetDatabaseName"""


# todo if will return name of database, change result == [0, 0, 0, 0] to checking nale
def test_get_exist_database_name(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.GetDatabaseName()

    result.append(result_list[0])
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        KS.S_OK.value,  # result_list[0] - instance.GetDatabaseName() result
        KS.S_OK.value  # instance.Close()
    ], f"get_exist_database_name  was crashed"


# ---------------------------------------------------------------------------------------------#
