# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oscar/Dokumente/UNI/6.Semester/MasterThesis/EnergyConsumptionFramework-Arduino/EnergyConsumptionFramework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oscar/Dokumente/UNI/6.Semester/MasterThesis/EnergyConsumptionFramework-Arduino/EnergyConsumptionFramework

# Utility rule file for partition_table-flash.

# Include the progress variables for this target.
include esp-idf/partition_table/CMakeFiles/partition_table-flash.dir/progress.make

esp-idf/partition_table/CMakeFiles/partition_table-flash:
	cd /home/oscar/Dokumente/UNI/6.Semester/MasterThesis/EnergyConsumptionFramework-Arduino/EnergyConsumptionFramework/esp-idf/partition_table && /usr/bin/cmake -E echo Warning:\ Command\ "partition_table-flash"\ is\ deprecated\ and\ will\ be\ removed\ in\ the\ next\ major\ release.\ \ \ \ \ \ \ \ \ Please\ use\ "partition-table-flash"\ instead.

partition_table-flash: esp-idf/partition_table/CMakeFiles/partition_table-flash
partition_table-flash: esp-idf/partition_table/CMakeFiles/partition_table-flash.dir/build.make

.PHONY : partition_table-flash

# Rule to build all files generated by this target.
esp-idf/partition_table/CMakeFiles/partition_table-flash.dir/build: partition_table-flash

.PHONY : esp-idf/partition_table/CMakeFiles/partition_table-flash.dir/build

esp-idf/partition_table/CMakeFiles/partition_table-flash.dir/clean:
	cd /home/oscar/Dokumente/UNI/6.Semester/MasterThesis/EnergyConsumptionFramework-Arduino/EnergyConsumptionFramework/esp-idf/partition_table && $(CMAKE_COMMAND) -P CMakeFiles/partition_table-flash.dir/cmake_clean.cmake
.PHONY : esp-idf/partition_table/CMakeFiles/partition_table-flash.dir/clean

esp-idf/partition_table/CMakeFiles/partition_table-flash.dir/depend:
	cd /home/oscar/Dokumente/UNI/6.Semester/MasterThesis/EnergyConsumptionFramework-Arduino/EnergyConsumptionFramework && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oscar/Dokumente/UNI/6.Semester/MasterThesis/EnergyConsumptionFramework-Arduino/EnergyConsumptionFramework /home/oscar/esp_vscode/esp-idf/components/partition_table /home/oscar/Dokumente/UNI/6.Semester/MasterThesis/EnergyConsumptionFramework-Arduino/EnergyConsumptionFramework /home/oscar/Dokumente/UNI/6.Semester/MasterThesis/EnergyConsumptionFramework-Arduino/EnergyConsumptionFramework/esp-idf/partition_table /home/oscar/Dokumente/UNI/6.Semester/MasterThesis/EnergyConsumptionFramework-Arduino/EnergyConsumptionFramework/esp-idf/partition_table/CMakeFiles/partition_table-flash.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : esp-idf/partition_table/CMakeFiles/partition_table-flash.dir/depend

