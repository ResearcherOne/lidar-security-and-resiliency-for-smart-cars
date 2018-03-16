#/*
# * Copyright (C) 2014  RoboPeak
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program.  If not, see <http://www.gnu.org/licenses/>.
# *
# */
#
HOME_TREE := ../../

MODULE_NAME := $(notdir $(CURDIR))

include $(HOME_TREE)/mak_def.inc

CXXSRC += main.cpp robot_base.cpp robot_base.hpp ../03_remote_scan_and_save/rplidar_module.cpp ../03_remote_scan_and_save/rplidar_module.hpp ../03_remote_scan_and_save/postgres_module.cpp ../03_remote_scan_and_save/postgres_module.hpp ../03_remote_scan_and_save/LIDAR_data_structures.cpp ../03_remote_scan_and_save/LIDAR_data_structures.hpp ../03_remote_scan_and_save/util_module.hpp ../03_remote_scan_and_save/util_module.cpp ../../../../cpp/06-writing-xbox-joystick-module/xbox_joystick.hpp ../../../../cpp/06-writing-xbox-joystick-module/xbox_joystick.cpp
C_INCLUDES += -I$(CURDIR)/../../sdk/include

EXTRA_OBJ := 
LD_LIBS += -lstdc++ -lpthread -lpqxx -lpq

all: build_app

include $(HOME_TREE)/mak_common.inc

clean: clean_app
