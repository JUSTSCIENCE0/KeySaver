import pytest
import platform
import re
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

password = b'gM?mWTZei88mE#Nj' # instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, 0)
# password_list = []

special_chars = rb"!@#\$%\^&\*\(\)_\-\+=/\?\.,<>'\";:\[\]\{\}"
special_class = b"!@#\$%\^&\*\(\)_\-\+=/\?\.,<>'\";:\[\]\{\}"
default_conf_pattern = rb"^(?=(?:.*[A-Za-z]){12,})(?=(?:.*\d){2})(?=(?:.*[" + special_chars + rb"]){2})[A-Za-z\d" + special_chars + rb"]{16}$"


# todo check passwords by regex
@pytest.fixture
# default keySaver with validate data
def instance():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, master_password)

# @pytest.fixture(scope="session", autouse=True)
# def global_passwords_setup():
#     KS_object = KeysaverImplementation(path_to_dll, path_to_config_dir, master_password)
#     KS_object.Init()
#     KS_object.SetMasterPassword()
#     for i in range(0, 9):
#         password_list.append(KS_object.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, i)[1])
#     KS_object.Close()
#

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

def test_generate_password_regex(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverInterface.KeysaverService(url=SS.EXIST_SERVICE_URL_0.value,
                                                         name=SS.EXIST_SERVICE_NAME_0.value,
                                                         conf_id=SS.EXIST_CONFIGURATION.value)
    result.append(instance.EditService(SS.EXIST_SERVICE_NAME_0.value, service_identity))

    for i in range(0, 9):
        res = instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, i)
        if (res[0]==0 and re.fullmatch(default_conf_pattern, res[1])): result.append(KS.S_OK.value)
        else:result.append(KS.E_UNEXPECTED_EXCEPTION.value)
    result.append(instance.Close())

    assert result == [
        KS.S_OK.value,  # instance.Init()
        KS.S_OK.value,  # instance.SetMasterPassword()
        KS.S_OK.value,  # instance.EditService(SS.EXIST_SERVICE_NAME_0.value, service_identity)
        KS.S_OK.value,  # instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, i)
        KS.S_OK.value,
        KS.S_OK.value,
        KS.S_OK.value,
        KS.S_OK.value,
        KS.S_OK.value,
        KS.S_OK.value,
        KS.S_OK.value,
        KS.S_OK.value,
        KS.S_OK.value  # instance.Close()
    ], f"GeneratePassword for not exist image was crashed"



