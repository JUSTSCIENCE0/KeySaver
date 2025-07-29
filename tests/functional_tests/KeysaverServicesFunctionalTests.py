import pytest
import platform
from KeysaverInterface import KeysaverImplementation
from KeysaverInterface import KeysaverService
from KeysaverInterface import KeysaverStatus as KS
#todo
"""3. переделать get_service - Enum - на имена, URL и на конфиг(пока не придумано) """
if (platform.system()=='Windows'):
    path_to_dll=r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"
else:
    path_to_dll=r"..\..\build\bin\RelWithDebInfo\keysaver-core.dll"#Linux
#todo cheking for Linux + path

path_to_config_dir=r".\storage"
path_to_false_config_dir=r"..\\"

master_password= r"Test123."

exist_service_name = r"test_service_0"
exist_service_name_1 = r"test_service_1"
not_exist_service_name = r"test_service_-1"

exist_service_url=exist_service_name+'.com'
not_exist_service_url = not_exist_service_name+'.com'

@pytest.fixture
#default keySaver with validate data
def instance():
    return KeysaverImplementation(path_to_dll, path_to_config_dir, master_password)

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


#---------------------------------------------------------------------------------------------#
""" Test of GetServices """
def test_get_services(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(True, True, True)
    result.append(instance.GetService(exist_service_name))
    result.append(instance.Close())
    assert result== [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        (KS.S_OK.value, service_identity),
                        KS.S_OK.value], \
                        f"get_services was crashed"

def test_get_not_exist_services(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.GetService(not_exist_service_name)[0])
    result.append(instance.Close())
    assert result== [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.E_SERVICE_NOT_EXISTS.value,
                        KS.S_OK.value], \
                        f"get_services for not exist service was crashed"

#---------------------------------------------------------------------------------------------#

""" Test of AddService WITHOUT SYNC DATA BASE"""
#todo переделать get_service + проверить, все ли покрыла(не все)
def test_add_new_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(False, False, True)
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value],  \
                        f"AddService new was crashed"

def test_add_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service()
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.E_SERVICE_ALREADY_EXISTS.value,
                        KS.S_OK.value], \
                        f"AddService exist service was crashed"

def test_add_exist_url_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(True, False, True)
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.E_SERVICE_URL_ALREADY_EXISTS.value,
                        KS.S_OK.value], \
                        f"AddService service with exist url was crashed"

def test_add_new_service_with_not_exist_config(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(False, False, False)
    result.append(instance.AddService(service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.E_CONFIG_NOT_EXISTS.value,
                        KS.S_OK.value], \
                        f"AddService service with config_not_found was crashed"

#---------------------------------------------------------------------------------------------#

"""  Test of DeleteService WITHOUT SYNC DATA BASE"""

def test_delete_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.DeleteService(exist_service_name))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value], \
                        f"DeleteService for exist_service was crashed"

def test_delete_not_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    result.append(instance.DeleteService(not_exist_service_name))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.E_SERVICE_NOT_EXISTS.value,
                        KS.S_OK.value], \
                        f"DeleteService for not_exist_service  was crashed"

#---------------------------------------------------------------------------------------------#

""" Test of EditService WITHOUT SYNC DATA BASE"""

def test_edit_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(False, False, True)
    result.append(instance.EditService(exist_service_name, service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.S_OK.value], \
                        f"EditService for exist_service was crashed"

def test_edit_not_exist_service(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service()
    result.append(instance.EditService(not_exist_service_name, service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.E_SERVICE_NOT_EXISTS.value,
                        KS.S_OK.value], \
                        f"EditService for not_exist_service  was crashed"

def test_edit_service_not_exist_config(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(True, True, False)
    result.append(instance.EditService(exist_service_name, service_identity))
    result.append(instance.Close())
    assert result == [
                        KS.S_OK.value,
                        KS.S_OK.value,
                        KS.E_CONFIG_NOT_EXISTS.value,
                        KS.S_OK.value], \
                        f"EditService for exist_service with not_exist_config was crashed"

def test_edit_exist_service_like_another_exist_servise(instance):
    result = []
    result.append(instance.Init())
    result.append(instance.SetMasterPassword())
    service_identity = get_service(True, True, True)
    result.append(instance.EditService(exist_service_name_1, service_identity))
    result.append(instance.Close())
    assert result == [
        KS.S_OK.value,
        KS.S_OK.value,
        KS.E_SERVICE_ALREADY_EXISTS.value,
        KS.S_OK.value], \
        f"EditService in already exist servise"
