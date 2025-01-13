// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include "keysaver/interface.h"

#include <string>

namespace Keysaver {
    KeysaverStatus init(const std::string& configPath);
    KeysaverStatus set_master_password(const std::string& masterPassword);
}