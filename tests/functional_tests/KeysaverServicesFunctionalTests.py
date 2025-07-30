import pytest
import platform
from KeysaverInterface import KeysaverImplementation
from KeysaverInterface import KeysaverService
from KeysaverInterface import KeysaverStatus as KS
from KeysaverInterface import ServiseStatus as SS


if (platform.system()=='Windows'):
    path_to_dll=r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"
else:
    path_to_dll=r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"#Linux
#todo cheking for Linux + path

path_to_config_dir=r".\storage"
path_to_false_config_dir=r"..\\"

master_password= r"Test123."

@pytest.fixture
#default keySaver with validate data
def instance():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, master_password)


#---------------------------------------------------------------------------------------------#
""" Test of GetServices """
def test_get_services(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService(SS.EXIST_SERVICE_URL_0.value,
                                       SS.EXIST_SERVICE_NAME_0.value,
                                       SS.EXIST_CONFIGURATION.value)
    result.append(instance.GetService(SS.EXIST_SERVICE_NAME_0.value))
    result.append(instance.Close())
    assert result== [
                        KS.S_OK.value,                      #instance.Init()
                        KS.S_OK.value,                      #SetMasterPassword()
                        (KS.S_OK.value, service_identity),  #instance.GetService(SS.EXIST_SERVICE_NAME_0.value)
                        KS.S_OK.value                       # instance.Close()
                        ],f"get_services was crashed"

def test_get_not_exist_services(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GetService(SS.NOT_EXIST_SERVICE_NAME.value)[0])
    result.append(instance.Close())
    assert result== [
                        KS.S_OK.value,                  # instance.Init()
                        KS.S_OK.value,                  # SetMasterPassword()
                        KS.E_SERVICE_NOT_EXISTS.value,  #instance.GetService(SS.NOT_EXIST_SERVICE_NAME.value)
                        KS.S_OK.value                   # instance.Close()
                         ],f"get_services for not exist service was crashed"

#---------------------------------------------------------------------------------------------#

""" Test of AddService WITHOUT SYNC DATA BASE"""
def test_add_new_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService(SS.NOT_EXIST_SERVICE_URL.value,
                                       SS.NOT_EXIST_SERVICE_NAME.value,
                                       SS.EXIST_CONFIGURATION.value)
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,  # instance.Init()
                        KS.S_OK.value,  # SetMasterPassword()
                        KS.S_OK.value,  #instance.AddService(service_identity)
                        KS.S_OK.value   # instance.Close()
                        ],f"AddService new was crashed"

def test_add_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService(SS.EXIST_SERVICE_URL_0.value,
                                       SS.EXIST_SERVICE_NAME_0.value,
                                       SS.EXIST_CONFIGURATION.value)
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,                      # instance.Init()
                        KS.S_OK.value,                      # SetMasterPassword()
                        KS.E_SERVICE_ALREADY_EXISTS.value,  #instance.AddService(service_identity)
                        KS.S_OK.value                       # instance.Close()
                         ],f"AddService exist service was crashed"

def test_add_exist_url_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService(SS.EXIST_SERVICE_URL_0.value,
                                       SS.NOT_EXIST_SERVICE_NAME.value,
                                       SS.EXIST_CONFIGURATION.value)
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,                          # instance.Init()
                        KS.S_OK.value,                          # SetMasterPassword()
                        KS.E_SERVICE_URL_ALREADY_EXISTS.value,  #instance.AddService(service_identity)
                        KS.S_OK.value                           # instance.Close()
                        ],f"AddService service with exist url was crashed"

def test_add_new_service_with_not_exist_config(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService(SS.NOT_EXIST_SERVICE_URL.value,
                                       SS.NOT_EXIST_SERVICE_NAME.value,
                                       SS.NOT_EXIST_CONFIGURATION.value)
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,                  # instance.Init()
                        KS.S_OK.value,                  # SetMasterPassword()
                        KS.E_CONFIG_NOT_EXISTS.value,   #instance.AddService(service_identity)
                        KS.S_OK.value                   # instance.Close()
                         ],f"AddService service with config_not_found was crashed"

#---------------------------------------------------------------------------------------------#

"""  Test of DeleteService WITHOUT SYNC DATA BASE"""

def test_delete_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.DeleteService(SS.EXIST_SERVICE_NAME_0.value))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,  # instance.Init()
                        KS.S_OK.value,  # SetMasterPassword()
                        KS.S_OK.value,  #instance.AddService(service_identity)
                        KS.S_OK.value   # instance.Close()
                         ],f"DeleteService for exist_service was crashed"

def test_delete_not_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.DeleteService(SS.NOT_EXIST_SERVICE_NAME.value))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,                  # instance.Init()
                        KS.S_OK.value,                  # SetMasterPassword()
                        KS.E_SERVICE_NOT_EXISTS.value,  # instance.DeleteService(SS.NOT_EXIST_SERVICE_NAME.value)
                        KS.S_OK.value                   # instance.Close()
                         ],f"DeleteService for not_exist_service  was crashed"

#---------------------------------------------------------------------------------------------#

""" Test of EditService WITHOUT SYNC DATA BASE"""

def test_edit_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService(SS.NOT_EXIST_SERVICE_URL.value,
                                       SS.NOT_EXIST_SERVICE_NAME.value,
                                       SS.EXIST_CONFIGURATION.value)
    result.append(instance.EditService(SS.EXIST_SERVICE_NAME_0.value, service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,  # instance.Init()
                        KS.S_OK.value,  # SetMasterPassword()
                        KS.S_OK.value,  # instance.EditService(SS.EXIST_SERVICE_NAME_0.value, service_identity)
                        KS.S_OK.value   # instance.Close()
                         ],f"EditService for exist_service was crashed"

def test_edit_not_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService(SS.EXIST_SERVICE_URL_0.value,
                                       SS.NOT_EXIST_SERVICE_NAME.value,
                                       SS.EXIST_CONFIGURATION.value)
    result.append(instance.EditService(SS.NOT_EXIST_SERVICE_NAME.value, service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,                  # instance.Init()
                        KS.S_OK.value,                  # SetMasterPassword()
                        KS.E_SERVICE_NOT_EXISTS.value,  # instance.EditService(SS.NOT_EXIST_SERVICE_NAME.value, service_identity)
                        KS.S_OK.value                   # instance.Close()
                         ],f"EditService for not_exist_service  was crashed"

def test_edit_service_not_exist_config(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService(SS.EXIST_SERVICE_URL_0.value,
                                       SS.EXIST_SERVICE_NAME_0.value,
                                       SS.NOT_EXIST_CONFIGURATION.value)
    result.append(instance.EditService(SS.EXIST_SERVICE_NAME_0.value, service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,                  # instance.Init()
                        KS.S_OK.value,                  # SetMasterPassword()
                        KS.E_CONFIG_NOT_EXISTS.value,   # instance.EditService(SS.EXIST_SERVICE_NAME_0.value, service_identity)
                        KS.S_OK.value                   # instance.Close()
                         ],f"EditService for exist_service with not_exist_config was crashed"

def test_edit_exist_service_like_another_exist_servise(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService(SS.EXIST_SERVICE_URL_0.value,
                                       SS.EXIST_SERVICE_NAME_0.value,
                                       SS.EXIST_CONFIGURATION.value)
    result.append(instance.EditService(SS.EXIST_SERVICE_NAME_1.value, service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,                      # instance.Init()
                        KS.S_OK.value,                      # instance.SetMasterPassword()
                        KS.E_SERVICE_ALREADY_EXISTS.value,  # instance.EditService(SS.EXIST_SERVICE_NAME_1.value, service_identity)
                        KS.S_OK.value                       # instance.Close()
                         ],f"EditService in already exist servise"

#---------------------------------------------------------------------------------------------#
""" Test of AddService WITH SYNC DATA BASE"""

def test_add_new_service_with_sync_database(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = KeysaverService(SS.NOT_EXIST_SERVICE_URL.value,
                                       SS.NOT_EXIST_SERVICE_NAME.value,
                                       SS.EXIST_CONFIGURATION.value)
    result.append(instance.AddService(service_identity))
    result.append(instance.SyncDatabase())
    result.append(instance.Close())

    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GetService(SS.NOT_EXIST_SERVICE_NAME.value))
    result.append(instance.DeleteService(SS.NOT_EXIST_SERVICE_NAME.value))
    result.append(instance.SyncDatabase())
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,                      # instance.Init()
                        KS.S_OK.value,                      # SetMasterPassword()
                        KS.S_OK.value,                      # instance.AddService(service_identity)
                        KS.S_OK.value,                      # instance.SyncDatabase()
                        KS.S_OK.value,                      # instance.Close()

                        KS.S_OK.value,                      # instance.Init()
                        KS.S_OK.value,                      # SetMasterPassword()
                        (KS.S_OK.value, service_identity),  # instance.GetService(SS.NOT_EXIST_SERVICE_NAME)
                        KS.S_OK.value,                      # instance.DeleteService(SS.NOT_EXIST_SERVICE_NAME)
                        KS.S_OK.value,                      # instance.SyncDatabase()
                        KS.S_OK.value,                      # instance.Close()
                        ],f"AddService new service with SyncDatabase was crashed"

#---------------------------------------------------------------------------------------------#
""" Test of DeleteService WITH SYNC DATA BASE"""

def test_delete_exist_service_with_sync_database(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = instance.GetService(SS.EXIST_SERVICE_NAME_0.value)[1]
    result.append(instance.DeleteService(SS.EXIST_SERVICE_NAME_0.value))
    result.append(instance.SyncDatabase())
    result.append(instance.Close())

    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GetService(SS.EXIST_SERVICE_NAME_0.value)[0])
    result.append(instance.AddService(service_identity))
    result.append(instance.SyncDatabase())
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,                  # instance.Init()
                        KS.S_OK.value,                  # SetMasterPassword()
                        KS.S_OK.value,                  # instance.DeleteService(SS.EXIST_SERVICE_NAME_0.value)
                        KS.S_OK.value,                  # instance.SyncDatabase()
                        KS.S_OK.value,                  # instance.Close()

                        KS.S_OK.value,                  # instance.Init()
                        KS.S_OK.value,                  # SetMasterPassword()
                        KS.E_SERVICE_NOT_EXISTS.value,  # instance.GetService(SS.EXIST_SERVICE_NAME_0.value)
                        KS.S_OK.value,                  # instance.AddService(service_identity)
                        KS.S_OK.value,                  # instance.SyncDatabase()
                        KS.S_OK.value,                  # instance.Close()
                        ],f"DeleteService for exist_service was crashed"

