import pytest
import platform
from KeysaverInterface import KeysaverImplementation
from KeysaverInterface import KeysaverConfiguration
from KeysaverInterface import KeysaverStatus as KS
from KeysaverInterface import ConfigurationStatus as CS


if (platform.system() == 'Windows'):
    path_to_dll = r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"
else:
    path_to_dll = r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"  # Linux
# todo cheking for Linux + path

path_to_config_dir = r".\storage"
path_to_false_config_dir = r"..\\"

master_password = r"Test123."

@pytest.fixture
# default keySaver with validate data
def instance():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, master_password)



# ---------------------------------------------------------------------------------------------#

""" Test of AddConfiguration WITHOUT SYNC DATA BASE"""

def test_add_new_configuration(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    configuration_identity = KeysaverConfiguration(CS.NOT_EXIST)

    result.append(instance.AddConfiguration(configuration_identity))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        KS.S_OK.value,  # instance.AddConfiguration(configuration_identity)
        KS.S_OK.value  # instance.Close()
    ], f"AddConfiguration new was crashed"

def test_add_exist_configuration(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    configuration_identity = KeysaverConfiguration(CS.EXIST)
    result.append(instance.AddConfiguration(configuration_identity))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        KS.E_CONFIG_ALREADY_EXISTS.value,  # instance.AddConfiguration(configuration_identity)
        KS.S_OK.value  # instance.Close()
    ], f"AddConfiguration not exist was crashed"

def test_add_configuration_invalid_pass_length(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    configuration_identity = KeysaverConfiguration(CS.INVALID_PASSWORD_LENGTH)
    result.append(instance.AddConfiguration(configuration_identity))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        KS.E_INVALID_PASSWORD_LENGTH.value,  # instance.AddConfiguration(configuration_identity)
        KS.S_OK.value  # instance.Close()
    ], f"AddConfiguration invalid_pass_length was crashed"
