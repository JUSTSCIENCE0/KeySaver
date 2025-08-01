import pytest
import platform

import KeysaverInterface
from KeysaverInterface import KeysaverImplementation
from KeysaverInterface import KeysaverStatus as KS
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
false_master_password = r"Test1239999."
short_master_password = r"Test"

password = b'gM?mWTZei88mE#Nj'

@pytest.fixture
# default keySaver with validate data
def instance():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, master_password)

# @pytest.fixture(scope="session", autouse=True)
# def global_passwords_setup():
#
#     # Можно вызвать здесь нужную функцию
#     # my_setup_function()

# ---------------------------------------------------------------------------------------------#

""" Test of default generate_password"""


def test_generate_password(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, 0)[0])
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        KS.S_OK.value,  # instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, 0)[0]
        KS.S_OK.value  # instance.Close()
    ], f"GeneratePassword was crashed"


def test_generate_password_no_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GeneratePassword(SS.NOT_EXIST_SERVICE_NAME.value, 0)[0])
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        KS.E_SERVICE_NOT_EXISTS.value,  # instance.GeneratePassword(SS.NOT_EXIST_SERVICE_NAME.value, 0)[0]
        KS.S_OK.value  # instance.Close()
    ], f"GeneratePassword for not exist service was crashed"


def test_generate_password_invalid_image(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, 10)[0])
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        KS.E_INVALID_ARG.value,  # instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, 10)[0]
        KS.S_OK.value  # instance.Close()
    ], f"GeneratePassword for not exist image was crashed"


def test_generate_password_for_ENG_conf(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverInterface.KeysaverService(url=SS.EXIST_SERVICE_URL_0.value,
                                                         name=SS.EXIST_SERVICE_NAME_0.value,
                                                         conf_id=SS.EXIST_CONFIGURATION.value)

    result.append(instance.EditService(SS.EXIST_SERVICE_NAME_0.value, service_identity))
    result.append(instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, 0))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        KS.S_OK.value,  # instance.EditService(SS.EXIST_SERVICE_NAME_0.value, service_identity)
        (KS.S_OK.value, password),
        KS.S_OK.value  # instance.Close()
    ], f"GeneratePassword for not exist image was crashed"


