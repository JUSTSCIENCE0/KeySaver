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

password = b'gM?mWTZei88mE#Nj'  # instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, 0)


def build_pattern(length=16, num_digits=2, num_specials=2, special_chars=None,
                  use_upper = True, use_lower = True, use_digits = True):

    if special_chars is None:
        special_chars = b"!@#$%^&*()_-+=/?.,<>'\";:[]{}"

    if (use_digits!=True): num_digits = 0
    letter_classes = b""
    if use_upper:
        letter_classes += b"A-Z"
    if use_lower:
        letter_classes += b"a-z"

    if not letter_classes and (length - num_digits - num_specials) > 0:
        raise ValueError("Невозможно создать шаблон: буквы запрещены, но их количество требуется.")

    special_class = b''.join([b"\\" + bytes([c]) if chr(c) in r"\-^[]{}" else bytes([c]) for c in special_chars])
    special_class_str = special_class.decode('ascii')

    min_letters = length - num_digits - num_specials

    letter_class_str = letter_classes.decode('ascii')

    parts = [b"^"]

    if letter_classes:
        parts.append(fr"(?=(?:.*[{letter_class_str}]){{{min_letters},}})".encode())
        parts.append(fr"(?=(?:.*\d){{{num_digits}}})".encode())
        parts.append(fr"(?=(?:.*[{special_class_str}]){{{num_specials}}})".encode())
        parts.append(fr"[{letter_class_str}\d{special_class_str}]{{{length}}}$".encode())

    return b"".join(parts)

special_chars = rb"!@#\$%\^&\*\(\)_\-\+=/\?\.,<>'\";:\[\]\{\}"
#default_conf_pattern = rb"^(?=(?:.*[A-Za-z]){12,})(?=(?:.*\d){2})(?=(?:.*[" + special_chars + rb"]){2})[A-Za-z\d" + special_chars + rb"]{16}$"


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


def test_generate_password_regex_default(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverInterface.KeysaverService(url=SS.EXIST_SERVICE_URL_0.value,
                                                         name=SS.EXIST_SERVICE_NAME_0.value,
                                                         conf_id=SS.EXIST_CONFIGURATION.value)
    result.append(instance.EditService(SS.EXIST_SERVICE_NAME_0.value, service_identity))

    for i in range(0, 9):
        res = instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, i)
        if res[0] == 0 and re.fullmatch(build_pattern(), res[1]):
            result.append(KS.S_OK.value)
        else:
            result.append(KS.E_UNEXPECTED_EXCEPTION.value)
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
    ], f"GeneratePassword for default configuration was crashed"


def test_generate_password_regex_ENG_EXIST_conf(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverInterface.KeysaverService(url=SS.EXIST_SERVICE_URL_0.value,
                                                         name=SS.EXIST_SERVICE_NAME_0.value,
                                                         conf_id=SS.EXIST_EDIT_CONFIGURATION.value)
    result.append(instance.EditService(SS.EXIST_SERVICE_NAME_0.value, service_identity))

    for i in range(0, 9):
        res = instance.GeneratePassword(SS.EXIST_SERVICE_NAME_0.value, i)
        if res[0] == 0 and re.fullmatch(build_pattern(length=20,
                                                      use_lower=True,
                                                      use_upper=True,
                                                      use_digits=True,
                                                      num_specials=3,
                                                      num_digits=4), res[1]):
            result.append(KS.S_OK.value)
        else:
            result.append(res[0])
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
    ], f"GeneratePassword for exist configuration was crashed"
