import pytest
import platform
import os
from KeysaverInterface import KeysaverImplementation
from KeysaverInterface import KeysaverService
from KeysaverInterface import KeysaverConfiguration
from KeysaverInterface import KeysaverStatus as KS

#todo
"""
4. Добавить комментарии к ошибкам (пример в test_get_services_count)
"""

if (platform.system()=='Windows'):
    path_to_dll=r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"
else:
    path_to_dll=r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"#Linux
#todo cheking for Linux + path

path_to_config_dir=r".\storage"
path_to_false_config_dir=r"..\\"


master_password= r"Test123."
false_master_password= r"Test1239999."
short_master_password= r"Test"

exist_service_name = r"test_service_0"
not_exist_service_name = r"test_service_-1"

exist_service_url=exist_service_name+'.com'
not_exist_service_url = not_exist_service_name+'.com'

exist_config_name = r"Default"
not_exist_config_name = r"Default NOT EXIST"

service_count = 5
config_count = 4
alphabet_count = 2

services_list = []
config_list = []

@pytest.fixture
#default keySaver with validate data
def instance():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, master_password)

@pytest.fixture
#keySaver with path = path_to_false_config_dir
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

def get_service(bool_url= True,bool_name = True, bool_config= True):
    """
    :param bool_url: True - if need exist url- exist_service_url,
                            else -  not_exist_service_url
    :param bool_name: True - if need exist name -  exist_service_name,
                            else - not_exist_service_name
    :param bool_config: True - if need exist config - r"Default",
                            else - r"Default NOT EXIST"
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
""" Test of init"""

# def test_init(instance):
#     result = instance.Init()
#     assert result == 0, f"init was crashed"
#
# def test_init_false_DB(instance_false_DB):
#     result = instance_false_DB.Init()
#     assert result == 1, f"init was crashed"

def test_init_close(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value], f"init or close was crashed"

#
# def test_init_false_DB_close(instance_false_DB):
#     result = []
#     result.append(instance_false_DB.Init())
#     result.append(instance_false_DB.Close())
#     assert result == [
#                       KS.M_DATABASE_NOT_FOUND.value,
#                       KS.S_OK.value],
#                       f"init false_DB or close was crashed"

#---------------------------------------------------------------------------------------------#
""" Test of master_password"""

#крашатся, если нет CLOSE в предыдущем тесте. Вылет программы приводит к некорректной работе
# def test_init_set_master_password(instance):
#     result = []
#     result.append(instance.Init())
#     result.append(instance.SetMasterPassword())
#     assert result == [
#                       KS.S_OK.value,
#                       KS.S_OK.value],
#                       f"set_master_password was crashed"

def test_init_set_master_password_close(instance):

    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value], \
                        f"set_master_password was crashed"

def test_init_set_false_master_password_close(instance_false_password):
    result = []
    result.append(instance_false_password.Init())
    result.append(instance_false_password.SetMasterPassword())
    result.append(instance_false_password.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.E_INVALID_MASTER_PASSWORD.value,
                        KS.S_OK.value],  \
                        f"init, set_master_password false or close was crashed"

def test_init_set_short_master_password_close(instance_short_password):
    result = []
    result.append(instance_short_password.Init())
    result.append(instance_short_password.SetMasterPassword())
    result.append(instance_short_password.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.E_TOO_SHORT_MASTER_PASSWORD.value,
                        KS.S_OK.value], \
                        f"init, set_master_password short or close was crashed"

#---------------------------------------------------------------------------------------------#

""" Test of generate_password"""

def test_generate_password(instance):

    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GeneratePassword(exist_service_name, 0)[0])
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value],  \
                        f"GeneratePassword was crashed"

def test_generate_password_no_service(instance):

    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GeneratePassword(not_exist_service_url, 0)[0])
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.E_SERVICE_NOT_EXISTS.value,
                        KS.S_OK.value],  \
                        f"GeneratePassword for not exist service was crashed"

def test_generate_password_invalid_image(instance):

    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GeneratePassword(exist_service_name, 10)[0])
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.E_INVALID_ARG.value,
                        KS.S_OK.value,],  \
                        f"GeneratePassword for not exist image was crashed"

#todo если БД поврежд и конфиг не найден - E_CONFIG_NOT_EXISTS
def test_generate_password_for_delete_db(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    os.rename(path_to_config_dir+'/database.bin', path_to_config_dir+'/database' )
    result.append(instance.GeneratePassword(exist_service_name, 10)[0])
    os.rename(path_to_config_dir + '/database', path_to_config_dir + '/database.bin')
    result.append(instance.Close())

#---------------------------------------------------------------------------------------------#

""" Test of call_function_GetAttributeCount """
def test_get_services_count(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.call_function_GetAttributeCount('Services'))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        (KS.S_OK.value, service_count),
                        KS.S_OK.value], \
                        f"get_services_count  was crashed"

def test_get_config_count(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.call_function_GetAttributeCount('Configurations'))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        (KS.S_OK.value, config_count),
                        KS.S_OK.value], \
                        f"get_config_count  was crashed"

def test_get_alphabet_count(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.call_function_GetAttributeCount('Alphabets'))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        (KS.S_OK.value, alphabet_count),
                        KS.S_OK.value], \
                        f"get_alphabet_count  was crashed"

#---------------------------------------------------------------------------------------------#

""" Test of call_function_GetAttributeList"""
""" пока что наделать с ними, не придумала"""
#todo проверка списков сделать 1. пустая БД, в нее записывать. 2. проверить, что в бд есть

def test_get_services_list(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.call_function_GetAttributeList('Services')

    result.append(len(result_list))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        service_count,
                        KS.S_OK.value], \
                        f"get_services_list  was crashed"


def test_get_config_list(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.call_function_GetAttributeList('Configurations')

    result.append(len(result_list))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        config_count,
                        KS.S_OK.value],\
                        f"get_config_list  was crashed"

def test_get_alphabet_list(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.call_function_GetAttributeList('Alphabets')

    result.append(len(result_list))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        alphabet_count,
                        KS.S_OK.value], \
                        f"get_alphabet_list  was crashed"

#---------------------------------------------------------------------------------------------#

""" Test of GetDatabaseName"""
#todo if will return name of database, change result == [0, 0, 0, 0] to checking nale
def test_get_exist_database_name(instance):

    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())

    result_list = instance.GetDatabaseName()

    result.append(result_list[0])
    print(result_list[1])
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value], \
                        f"get_exist_database_name  was crashed"

#---------------------------------------------------------------------------------------------#

""" Test of SyncDatabase"""

# def test_SyncDatabase(instance):
#
#---------------------------------------------------------------------------------------------#

""" Test of AddConfiguration WITHOUT SYNC DATA BASE"""

def test_add_new_configuration(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    configuration_identity = get_configuration()
    result.append(instance.AddConfiguration(configuration_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value], \
        f"AddConfiguration new was crashed"