import pytest
import platform

from KeysaverInterface import KeysaverImplementation
from KeysaverInterface import KeysaverConfiguration
from KeysaverInterface import KeysaverService as KS
from KeysaverInterface import ServiseStatus as SS
from KeysaverInterface import ConfigurationStatus as CS

if (platform.system() == 'Windows'):
    path_to_dll = r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"
else:
    path_to_dll = r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"  # Linux
# todo cheking for Linux + path

path_to_config_dir = r".\storage"
path_to_false_config_dir = r"..\\"

master_password = r"Test123."

service_count = 5
config_count = 4
alphabet_count = 2

@pytest.fixture
# default keySaver with validate data
def instance():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, master_password)

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
