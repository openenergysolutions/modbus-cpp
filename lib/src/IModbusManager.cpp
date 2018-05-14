/*
 * Copyright 2017-2018 Duke Energy Corporation and Open Energy Solutions, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "modbus/IModbusManager.h"

#include <algorithm>
#include <thread>

#include "ModbusManagerImpl.h"
#include "logging/Logger.h"

namespace modbus
{

std::unique_ptr<IModbusManager> IModbusManager::create(std::shared_ptr<Logger> logger)
{
    return IModbusManager::create(std::move(logger), std::thread::hardware_concurrency());
}

std::unique_ptr<IModbusManager> IModbusManager::create(std::shared_ptr<Logger> logger, unsigned int number_of_threads)
{
    number_of_threads = std::max(1u, number_of_threads);
    return std::make_unique<ModbusManagerImpl>(logger, number_of_threads);
}

} // namespace modbus
